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

 SSE_VECTOR SSE_CALLCONV Renderer::GetGLPos( SSE_VECTOR_PARAM1 Position) const
{
	return  Multiply
	(
		Position,
		{
			2.f * Scale / WinX,
			2.f * Scale / WinY,
			2.f * Scale / WinY
		}
	);
}

 SSE_VECTOR SSE_CALLCONV Renderer::GetGLSize( SSE_VECTOR_PARAM1 Size) const
{
	return  Multiply
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

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, BitsPerPixel, Image.GetWidth(), Image.GetHeight(), 0, Format, GL_UNSIGNED_BYTE, GetImageBits(Image).data());
	return TexID;
	
}

void Renderer::DeleteTexture(u_int TexID) const
{
	glDeleteTextures(1, &TexID);
}

void Renderer::UpdateWindow(float X, float Y)
{
	WinX = X;
	WinY = Y;
}

void Renderer::UpdateScale(float N)
{
	Scale = N;
}

void Renderer::AddShader(u_int ShaderProgram, const STD string& pShaderText, u_int ShaderType) const
{
	//쉐이더 오브젝트 생성
	u_int ShaderObj = glCreateShader(ShaderType);

	if (ShaderObj == 0) {
		fprintf(stderr, "Error creating shader type %d\n", ShaderType);
	}

	GLchar const* files[] = { pShaderText.c_str() };
	GLint lengths[] = { static_cast<GLint>(pShaderText.size()) };
	//쉐이더 코드를 쉐이더 오브젝트에 할당
	glShaderSource(ShaderObj, 1, files, lengths);

	//할당된 쉐이더 코드를 컴파일
	glCompileShader(ShaderObj);

	GLint success;
	// ShaderObj 가 성공적으로 컴파일 되었는지 확인
	glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);
	if (!success) 
	{
		GLchar InfoLog[1024];
		//OpenGL 의 shader log 데이터를 가져옴
		glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
		fprintf(stderr, "Error compiling shader type %d: '%s'\n", ShaderType, InfoLog);
		printf("%s \n", pShaderText.c_str());
	}

	// ShaderProgram 에 attach!!
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
	u_int ShaderProgram = glCreateProgram(); //빈 쉐이더 프로그램 생성

	if (ShaderProgram == 0) 
	{ //쉐이더 프로그램이 만들어졌는지 확인
		fprintf(stderr, "Error creating shader program\n");
	}

	STD string vs, fs;

	//shader.vs 가 vs 안으로 로딩됨
	if (!ReadFile(filenameVS, &vs)) return (printf("Error compiling vertex shader\n"), -1);

	//shader.fs 가 fs 안으로 로딩됨
	if (!ReadFile(filenameFS, &fs)) return (printf("Error compiling fragment shader\n"), -1);

	// ShaderProgram 에 vs.c_str() 버텍스 쉐이더를 컴파일한 결과를 attach함
	AddShader(ShaderProgram, vs.c_str(), GL_VERTEX_SHADER);

	// ShaderProgram 에 fs.c_str() 프레그먼트 쉐이더를 컴파일한 결과를 attach함
	AddShader(ShaderProgram, fs.c_str(), GL_FRAGMENT_SHADER);

	GLint Success = 0;
	GLchar ErrorLog[1024] = { 0 };

	//Attach 완료된 shaderProgram 을 링킹함
	glLinkProgram(ShaderProgram);

	//링크가 성공했는지 확인
	glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &Success);

	if (Success == 0) {
		// shader program 로그를 받아옴
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

#ifdef CYAN_DEBUG_COLLISION
void SSE_CALLCONV Renderer::DrawCollisionRect( SSE_VECTOR_PARAM1 Position,  SSE_VECTOR_PARAM1 Size) const
{
	 SSE_VECTOR GLSize = GetGLSize(Size);

	 SSE_VECTOR GLPos = GetGLPos(Position);
	 SSE_VECTOR BlueRect = GLPos;
	 SSE_VECTOR GreenRect = GLPos;
	 SSE_VECTOR YellowRect = GLPos;

	 SSE_VECTOR BlueSize =  Swizzle(GLSize, 0, 2, 2, 3);
	float BlueRectY =  GetY(BlueRect) +  GetZ(BlueRect);
	float BlueSizeY =  GetY(BlueSize);
	 SetY(&BlueRect, BlueRectY );
	 SetY(&GreenRect, BlueRectY + BlueSizeY);
	 SetY(&YellowRect, BlueRectY - BlueSizeY);

	float Depth =  GetY(GLPos);

	 SetZ(&GLPos, Depth);
	 SetZ(&BlueRect, Depth);
	 SetZ(&GreenRect, Depth);
	 SetZ(&YellowRect, Depth);

	DrawTexture(YellowRect, GLSize, { 1.f, 1.f, 0.f, 1.f }, m_DebugRect);
	DrawTexture(GLPos, GLSize, { 1.f, 0.f, 0.f, 1.f }, m_DebugRect);
	DrawTexture(BlueRect, BlueSize, { 0.f, 0.f, 1.f, 1.f }, m_DebugRect);
	DrawTexture(GreenRect, GLSize, { 0.f, 1.f, 0.f, 1.f }, m_DebugRect);
}
#endif

void SSE_CALLCONV Renderer::DrawTexRect(const FLOAT3& Position, const FLOAT2& Size, const FLOAT4& Color, u_int TexID) const
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glUseProgram(m_TextureRectShader);

	glUniform3f(glGetUniformLocation(m_TextureRectShader, "u_Trans"), Position.x, Position.y, Position.z);
	glUniform2f(glGetUniformLocation(m_TextureRectShader, "u_Size"), Size.x, Size.y);
	glUniform4f(glGetUniformLocation(m_TextureRectShader, "u_Color"), Color.x, Color.y, Color.z, Color.w);
	int texUniform = glGetUniformLocation(m_TextureRectShader, "u_Texture");
	glUniform1i(texUniform, 0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, TexID);

	int attribPosition = glGetAttribLocation(m_TextureRectShader, "a_Position");
	int attribTexture = glGetAttribLocation(m_TextureRectShader, "a_TexPos");

	glEnableVertexAttribArray(attribPosition);
	glEnableVertexAttribArray(attribTexture);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBOTexRect);
	glVertexAttribPointer(attribPosition, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, 0);
	glVertexAttribPointer(attribTexture, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)(3 * sizeof(float)));

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(attribPosition);
	glDisableVertexAttribArray(attribTexture);

	glDisable(GL_BLEND);
}

void SSE_CALLCONV Renderer::DrawShadow(SSE_VECTOR_PARAM1 Position, SSE_VECTOR_PARAM1 Size, float Alpha) const
{
	FLOAT3 GLPos = StoreFloat3(GetGLPos(Position));
	FLOAT2 GLSize = StoreFloat2(GetGLSize(Size));
	GLPos.z = FARTHEST;
	DrawTexRect(GLPos, GLSize, { 1.f, 1.f, 1.f, 1.f }, m_TexShadow);
}

void SSE_CALLCONV Renderer::DrawSprite( SSE_VECTOR_PARAM1 Position,  SSE_VECTOR_PARAM1 Size,
	const  FLOAT4& Color, u_int TexID, const  UINT2& CurrentSprite, const  UINT2& TotalSprite,
	u_int LayerGrouping) const
{
	 FLOAT3 GLPos = StoreFloat3(GetGLPos(Position));
	 FLOAT2 GLSize = StoreFloat2(GetGLSize(Size));

	switch (LayerGrouping)
	{
	case LayerGroup::Background:
	{
		GLPos.y += GLPos.z;
		GLPos.z = FARTHEST;
		break;
	}
	case LayerGroup::Middleground:
	{
		float Y = GLPos.y;
		GLPos.y += GLPos.z;
		GLPos.z = Y;
		break;
	}
	case LayerGroup::Foreground:
	{
		GLPos.y += GLPos.z;
		GLPos.z = NEAREST;
		break;
	}
	}

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
	glUniform1f(u_CurrSeqX, CurrentSprite.x);
	glUniform1f(u_CurrSeqY, TotalSprite.y - CurrentSprite.y - 1);
	glUniform1f(u_TotalSeqX, TotalSprite.x);
	glUniform1f(u_TotalSeqY, TotalSprite.y);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, TexID);

	u_int u_Texture = glGetUniformLocation(shader, "u_Texture");
	glUniform1i(u_Texture, 0);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(attribPosition);
	glDisableVertexAttribArray(attribTexture);

	glDisable(GL_BLEND);
}