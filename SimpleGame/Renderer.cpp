#include "stdafx.h"
#include "Renderer.h"

#include <atlimage.h>
#include <fstream>

#include "Dependencies/GL/glew.h"

/* CImage to Unsigned Char Vector */
STD vector<unsigned char> GetImageBits(const CImage & Image)
{
	STD vector<GLubyte> ImageBits;
	BITMAP BMP;
	GetObject(Image, sizeof(BITMAP), &BMP);
	BITMAPINFOHEADER BMPHeader{ 0 };
	BMPHeader.biSize = sizeof(BITMAPINFOHEADER);
	BMPHeader.biWidth = BMP.bmWidth;
	BMPHeader.biHeight = BMP.bmHeight;
	BMPHeader.biPlanes = 1;
	BMPHeader.biBitCount = Image.GetBPP();
	BMPHeader.biCompression = (BI_RGB);

	HDC hDC = ::GetDC(NULL);
	GetDIBits(hDC, Image, 0, BMP.bmHeight, NULL, (LPBITMAPINFO)&BMPHeader, DIB_RGB_COLORS);
	ImageBits.resize(BMPHeader.biSizeImage);
	GetDIBits(hDC, Image, 0, BMP.bmHeight, &(ImageBits[0]), (LPBITMAPINFO)&BMPHeader, DIB_RGB_COLORS);
	ReleaseDC(NULL, hDC);
	return ImageBits;
}

SSE_VECTOR SSE_CALLCONV Renderer::GLTransform(SSE_VECTOR_PARAM1 v, u_int LayerGrouping) const
{
	SSE_VECTOR Result = Multiply(v,  LoadFloat3(Ratio));
	switch (LayerGrouping)
	{
	case LayerGroup::Background:
	{
		float Depth = FARTHEST;
		SetZ(&Result, Depth);
		break;
	}
	case LayerGroup::Middleground:
	{
		float Depth = GetY(Result);
		SetY(&Result, Depth + GetZ(Result));
		SetZ(&Result, Depth);
		break;
	}
	case LayerGroup::Foreground:
	{
		float Depth = NEAREST;
		SetZ(&Result, Depth);
		break;
	}
	}
	return  Result;
}

bool Renderer::Initialize()
{
	//Load shaders
	m_TextureRectShader = CompileShaders("./Shaders/TextureRect.vs", "./Shaders/TextureRect.fs");
	m_TextureSpriteShader = CompileShaders("./Shaders/TextureRectSeq.vs", "./Shaders/TextureRectSeq.fs");

	//Create VBOs
	CreateVertexBufferObjects();

	return (m_TextureSpriteShader > 0 && m_VBOTexRect > 0);
}

void Renderer::RecalculateRatio()
{
	Ratio.x = 2.f * Scale / WinX;
	Ratio.y = 2.f * Scale / WinY;
	Ratio.z = Ratio.y;
}

void Renderer::CreateVertexBufferObjects()
{
	float TexRect[] =
	{
		-0.5f, -0.5f, 0.f, 0.f, 0.f,
		-0.5f, +0.5f, 0.f, 0.f, 1.f,
		+0.5f, +0.5f, 0.f, 1.f, 1.f, //Triangle1
		-0.5f, -0.5f, 0.f, 0.f, 0.f,
		+0.5f, +0.5f, 0.f, 1.f, 1.f,
		+0.5f, -0.5f, 0.f, 1.f, 0.f //Triangle2
	};

	glGenBuffers(1, &m_VBOTexRect);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOTexRect);
	glBufferData(GL_ARRAY_BUFFER, sizeof(TexRect), TexRect, GL_STATIC_DRAW);
}

u_int Renderer::GenerateTexture(const STD string& filePath) const
{
	u_int TexID;

	CImage Image;
	STD wstring Path{ filePath.begin(), filePath.end() };
	Image.Load(Path.c_str());

	glGenTextures(1, &TexID);
	glBindTexture(GL_TEXTURE_2D, TexID);

	int BitsPerPixel = Image.GetBPP() / 8;

	GLenum Format = GL_BGR_EXT;
	if (BitsPerPixel == 4)
		Format = GL_BGRA_EXT;

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, BitsPerPixel, Image.GetWidth(), Image.GetHeight(), 0, Format, GL_UNSIGNED_BYTE, GetImageBits(Image).data());
	return TexID;
}

void Renderer::DeleteTexture(u_int TexID) const
{
	glDeleteTextures(1, &TexID);
}

void Renderer::UpdateWindow(int X, int Y)
{
	WinX = X;
	WinY = Y;
	RecalculateRatio();
}

void Renderer::UpdateScale(float N)
{
	Scale = N;
	RecalculateRatio();
}

void Renderer::AddShader(u_int ShaderProgram, const STD string& pShaderText, u_int ShaderType) const
{
	//���̴� ������Ʈ ����
	u_int ShaderObj = glCreateShader(ShaderType);

	if (ShaderObj == 0) {
		fprintf(stderr, "Error creating shader type %d\n", ShaderType);
	}

	GLchar const* files[] = { pShaderText.c_str() };
	GLint lengths[] = { static_cast<GLint>(pShaderText.size()) };
	//���̴� �ڵ带 ���̴� ������Ʈ�� �Ҵ�
	glShaderSource(ShaderObj, 1, files, lengths);

	//�Ҵ�� ���̴� �ڵ带 ������
	glCompileShader(ShaderObj);

	GLint success;
	// ShaderObj �� ���������� ������ �Ǿ����� Ȯ��
	glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);
	if (!success) 
	{
		GLchar InfoLog[1024];
		//OpenGL �� shader log �����͸� ������
		glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
		fprintf(stderr, "Error compiling shader type %d: '%s'\n", ShaderType, InfoLog);
		printf("%s \n", pShaderText.c_str());
	}

	// ShaderProgram �� attach!!
	glAttachShader(ShaderProgram, ShaderObj);
}

bool Renderer::ReadFile(const STD string& filename, STD string *target) const
{
	STD ifstream file(filename);
	STD string line;
	if (!file)
		return (STD cout << filename << " file loading failed.. \n" << STD endl, false);
	while (getline(file, line)) 
		target->append(line +  "\n");
	return true;
}

u_int Renderer::CompileShaders(const STD string& filenameVS, const STD string&  filenameFS) const
{
	u_int ShaderProgram = glCreateProgram(); //�� ���̴� ���α׷� ����

	if (ShaderProgram == 0) 
	{ //���̴� ���α׷��� ����������� Ȯ��
		fprintf(stderr, "Error creating shader program\n");
	}

	STD string vs, fs;

	//shader.vs �� vs ������ �ε���
	if (!ReadFile(filenameVS, &vs)) return (printf("Error compiling vertex shader\n"), -1);

	//shader.fs �� fs ������ �ε���
	if (!ReadFile(filenameFS, &fs)) return (printf("Error compiling fragment shader\n"), -1);

	// ShaderProgram �� vs.c_str() ���ؽ� ���̴��� �������� ����� attach��
	AddShader(ShaderProgram, vs.c_str(), GL_VERTEX_SHADER);

	// ShaderProgram �� fs.c_str() �����׸�Ʈ ���̴��� �������� ����� attach��
	AddShader(ShaderProgram, fs.c_str(), GL_FRAGMENT_SHADER);

	GLint Success = 0;
	GLchar ErrorLog[1024] = { 0 };

	//Attach �Ϸ�� shaderProgram �� ��ŷ��
	glLinkProgram(ShaderProgram);

	//��ũ�� �����ߴ��� Ȯ��
	glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &Success);

	if (Success == 0) {
		// shader program �α׸� �޾ƿ�
		glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
		STD cout << filenameVS << ", " << filenameFS << " Error linking shader program\n" << ErrorLog;
		return -1;
	}

	glValidateProgram(ShaderProgram);
	glGetProgramiv(ShaderProgram, GL_VALIDATE_STATUS, &Success);
	if (!Success) {
		glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
		STD cout << filenameVS << ", " << filenameFS << " Error validating shader program\n" << ErrorLog;
		return -1;
	}

	glUseProgram(ShaderProgram);
	STD cout << filenameVS << ", " << filenameFS << " Shader compiling is done.\n";

	return ShaderProgram;
}

void Renderer::Prepare()
{
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClearDepth(FARTHEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
}

void SSE_CALLCONV Renderer::DrawSprite( SSE_VECTOR_PARAM1 Position,  SSE_VECTOR_PARAM1 Size,
	const  FLOAT4& Color, u_int TexID, const  UINT2& CurrentSprite, const  UINT2& TotalSprite,
	u_int LayerGrouping) const
{
	FLOAT3 GLPos = StoreFloat3(GLTransform(Position, LayerGrouping));
	FLOAT2 GLSize = StoreFloat2(GLTransform(Size, LayerGrouping));

	u_int shader = m_TextureSpriteShader;
	glUseProgram(shader);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	u_int u_Trans = glGetUniformLocation(shader, "u_Trans");
	u_int u_Size = glGetUniformLocation(shader, "u_Size");
	u_int u_Color = glGetUniformLocation(shader, "u_Color");
	u_int u_TotalSeqX = glGetUniformLocation(shader, "u_TotalSeqX");
	u_int u_TotalSeqY = glGetUniformLocation(shader, "u_TotalSeqY");
	u_int u_CurrSeqX = glGetUniformLocation(shader, "u_CurrSeqX");
	u_int u_CurrSeqY = glGetUniformLocation(shader, "u_CurrSeqY");

	glUniform1f(glGetUniformLocation(shader, "u_Depth"), 0.5f);
	int texUniform = glGetUniformLocation(m_TextureRectShader, "u_Texture");
	glUniform1i(texUniform, 0);

	int attribPosition = glGetAttribLocation(m_TextureRectShader, "a_Position");
	int attribTexture = glGetAttribLocation(m_TextureRectShader, "a_TexPos");

	glEnableVertexAttribArray(attribPosition);
	glEnableVertexAttribArray(attribTexture);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBOTexRect);
	glVertexAttribPointer(attribPosition, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, 0);
	glVertexAttribPointer(attribTexture, 2, GL_FLOAT, GL_FALSE,  sizeof(float) * 5, (void*)(sizeof(float) * 3));

	glUniform3f(u_Trans, GLPos.x, GLPos.y, GLPos.z);
	glUniform2f(u_Size, GLSize.x, GLSize.y);
	glUniform4f(u_Color, Color.x, Color.y, Color.z, Color.w);
	glUniform1f(u_CurrSeqX,  static_cast<float>(CurrentSprite.x));
	glUniform1f(u_CurrSeqY,  static_cast<float>(TotalSprite.y - CurrentSprite.y - 1));
	glUniform1f(u_TotalSeqX, static_cast<float>(TotalSprite.x));
	glUniform1f(u_TotalSeqY, static_cast<float>(TotalSprite.y));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, TexID);

	u_int u_Texture = glGetUniformLocation(shader, "u_Texture");
	glUniform1i(u_Texture, 0);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(attribPosition);
	glDisableVertexAttribArray(attribTexture);

	glDisable(GL_BLEND);
}