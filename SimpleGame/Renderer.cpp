#include "stdafx.h"
#include "Renderer.h"

#include <atlimage.h>
#include <fstream>

#include "Dependencies/GL/glew.h"
#include "CyanEngine.h"
#include "World.h"


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

DX XMVECTOR XM_CALLCONV Renderer::GetGLPos(DX FXMVECTOR Position) const
{
	float Scale = Engine.GetWorld().GetScale();
	float WinX, WinY;
	Engine.GetFramework().GetWindowSizef(&WinX, &WinY);
	return DX Multiply
	(
		Position,
		{
			2.f * Scale / WinX,
			2.f * Scale / WinY,
			2.f * Scale / WinY
		}
	);
}

DX XMVECTOR XM_CALLCONV Renderer::GetGLSize(DX FXMVECTOR Size) const
{
	float Scale = Engine.GetWorld().GetScale();
	float WinX, WinY;
	Engine.GetFramework().GetWindowSizef(&WinX, &WinY);
	return DX Multiply
	(
		Size,
		{ 
			Scale / WinX,
			Scale / WinY,
			Scale / WinY
		}
	);
}

bool Renderer::Initialize()
{
	//Load shaders
	m_TextureRectShader = CompileShaders("./Shaders/TextureRect.vs", "./Shaders/TextureRect.fs");
	m_TextureSpriteShader = CompileShaders("./Shaders/TextureRectSeq.vs", "./Shaders/TextureRectSeq.fs");

	//Load shadow texture
	m_TexShadow = GenerateTexture("./Resources/shadow.png");
#ifdef CYAN_DEBUG_COLLISION
	m_DebugRect = GenerateTexture("./Resources/debug_rect.png");
#endif

	//Create VBOs
	CreateVertexBufferObjects();

	return (m_TextureSpriteShader > 0 && m_VBOTexRect > 0);
}

void Renderer::CreateVertexBufferObjects()
{
	float TexRect[]
		=
	{
		-1.f, -1.f, 0.f, 0.f, 0.f,
		-1.f, +1.f, 0.f, 0.f, 1.f,
		+1.f, +1.f, 0.f, 1.f, 1.f, //Triangle1
		-1.f, -1.f, 0.f, 0.f, 0.f,
		+1.f, +1.f, 0.f, 1.f, 1.f,
		+1.f, -1.f, 0.f, 1.f, 0.f //Triangle2
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

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, BitsPerPixel, Image.GetWidth(), Image.GetHeight(), 0, Format, GL_UNSIGNED_BYTE, GetImageBits(Image).data());
	return TexID;
	
}

void Renderer::DeleteTexture(u_int TexID) const
{
	glDeleteTextures(1, &TexID);
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

void XM_CALLCONV Renderer::DrawTexture(DX FXMVECTOR Position, DX FXMVECTOR Size, DX FXMVECTOR Color, u_int TexID) const
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	//Program select
	glUseProgram(m_TextureRectShader);

	//Shadow to render last (thus Z depth will be "FARTHEST")
	glUniform3f(glGetUniformLocation(m_TextureRectShader, "u_Trans"), DX GetX(Position), DX GetY(Position), DX GetZ(Position));
	glUniform2f(glGetUniformLocation(m_TextureRectShader, "u_Size"), DX GetX(Size), DX GetY(Size));
	glUniform4f(glGetUniformLocation(m_TextureRectShader, "u_Color"), DX GetX(Color), DX GetY(Color), DX GetZ(Color), DX GetW(Color));
	int texUniform = glGetUniformLocation(m_TextureRectShader, "u_Texture");
	glUniform1i(texUniform, 0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, TexID);

	int attribPosition = glGetAttribLocation(m_TextureRectShader, "a_Position");
	int attribTexture = glGetAttribLocation(m_TextureRectShader, "a_TexPos");

	glEnableVertexAttribArray(attribPosition);
	glEnableVertexAttribArray(attribTexture);

	//Render shadow first
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOTexRect);
	glVertexAttribPointer(attribPosition, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, 0);
	glVertexAttribPointer(attribTexture, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)(3 * sizeof(float)));

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(attribPosition);
	glDisableVertexAttribArray(attribTexture);

	glDisable(GL_BLEND);
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
	glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
	glClearDepth(FARTHEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

#ifdef CYAN_DEBUG_COLLISION
void XM_CALLCONV Renderer::DrawCollisionRect(DX FXMVECTOR Position, DX FXMVECTOR Size) const
{
	DX XMVECTOR GLSize = GetGLSize(Size);

	DX XMVECTOR GLPos = GetGLPos(Position);
	DX XMVECTOR BlueRect = GLPos;
	DX XMVECTOR GreenRect = GLPos;
	DX XMVECTOR YellowRect = GLPos;

	DX XMVECTOR BlueSize = DX Swizzle(GLSize, 0, 2, 2, 3);
	float BlueRectY = DX GetY(BlueRect) + DX GetZ(BlueRect);
	float BlueSizeY = DX GetY(BlueSize);
	DX SetY(&BlueRect, BlueRectY );
	DX SetY(&GreenRect, BlueRectY + BlueSizeY);
	DX SetY(&YellowRect, BlueRectY - BlueSizeY);

	float Depth = DX GetY(GLPos);

	DX SetZ(&GLPos, Depth);
	DX SetZ(&BlueRect, Depth);
	DX SetZ(&GreenRect, Depth);
	DX SetZ(&YellowRect, Depth);

	DrawTexture(YellowRect, GLSize, { 1.f, 1.f, 0.f, 1.f }, m_DebugRect);
	DrawTexture(GLPos, GLSize, { 1.f, 0.f, 0.f, 1.f }, m_DebugRect);
	DrawTexture(BlueRect, BlueSize, { 0.f, 0.f, 1.f, 1.f }, m_DebugRect);
	DrawTexture(GreenRect, GLSize, { 0.f, 1.f, 0.f, 1.f }, m_DebugRect);
}
#endif

void XM_CALLCONV Renderer::DrawShadow(DX FXMVECTOR Position, DX FXMVECTOR Size, DX FXMVECTOR Color) const
{
	DX XMVECTOR GLPos = GetGLPos(Position);
	DX XMVECTOR GLSize = GetGLSize(Size);
	DX SetZ(&GLPos, FARTHEST);
	DrawTexture(GLPos, GLSize, { 1.f, 1.f, 1.f, DX GetW(Color) }, m_TexShadow);
}

void XM_CALLCONV Renderer::DrawSprite(DX FXMVECTOR Position, DX FXMVECTOR Size,
	DX FXMVECTOR Color, u_int TexID, DX GXMVECTOR CurrentSprite, DX HXMVECTOR TotalSprite) const
{
	DX XMVECTOR GLPos = GetGLPos(Position);
	DX XMVECTOR GLSize = GetGLSize(Size);

	u_int shader = m_TextureSpriteShader;

	//Program select
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

	//Render Object
	glUniform3f(u_Trans, DX GetX(GLPos), DX GetY(GLPos) + DX GetZ(GLPos), DX GetY(GLPos));
	glUniform2f(u_Size, DX GetX(GLSize), DX GetY(GLSize));
	glUniform4f(u_Color, DX GetX(Color), DX GetY(Color), DX GetZ(Color), DX GetW(Color));
	glUniform1f(u_CurrSeqX, DX GetX(CurrentSprite));
	glUniform1f(u_CurrSeqY, DX GetY(TotalSprite) - DX GetY(CurrentSprite) - 1);
	glUniform1f(u_TotalSeqX, DX GetX(TotalSprite));
	glUniform1f(u_TotalSeqY, DX GetY(TotalSprite));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, TexID);

	u_int u_Texture = glGetUniformLocation(shader, "u_Texture");
	glUniform1i(u_Texture, 0);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(attribPosition);
	glDisableVertexAttribArray(attribTexture);

	glDisable(GL_BLEND);
}