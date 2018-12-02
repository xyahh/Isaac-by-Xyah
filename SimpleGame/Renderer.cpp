#include "stdafx.h"
#include "Renderer.h"

#include <fstream>
#include "Dependencies/GL/glew.h"
#include "LoadPng.h"
#include "World.h"

DX XMVECTOR XM_CALLCONV Renderer::GetGLPos(DX FXMVECTOR Position) const
{
	float Scale = World::GetScale();
	return DX Multiply
	(
		Position,
		{
			2.f * Scale / m_WindowSizeX,
			2.f * Scale / m_WindowSizeY,
			2.f * Scale / m_WindowSizeY 
		}
	);
}

DX XMVECTOR XM_CALLCONV Renderer::GetGLSize(DX FXMVECTOR Size) const
{
	float Scale = World::GetScale();
	return DX Multiply
	(
		Size,
		{ 
			Scale / m_WindowSizeX, 
			Scale / m_WindowSizeY,
			Scale / m_WindowSizeY
		}
	);
}

bool Renderer::Initialize(int windowSizeX, int windowSizeY)
{
	//Set window size
	m_WindowSizeX = windowSizeX;
	m_WindowSizeY = windowSizeY;

	//Load shaders
	m_TextureRectShader = CompileShaders("./Shaders/TextureRect.vs", "./Shaders/TextureRect.fs");
	m_TextureRectSeqShader = CompileShaders("./Shaders/TextureRectSeq.vs", "./Shaders/TextureRectSeq.fs");

	//Load shadow texture
	m_TexShadow = CreatePngTexture("./Resources/shadow.png");
#ifdef CYAN_DEBUG_COLLISION
	m_DebugRect = CreatePngTexture("./Resources/debug_rect.png");
#endif

	//Create VBOs
	CreateVertexBufferObjects();

	return (m_TextureRectSeqShader > 0 && m_VBOTexRect > 0);
}

void Renderer::CreateVertexBufferObjects()
{
	float texRect[]
		=
	{
		-1.f, -1.f, 0.f, 0.f, 1.f,
		-1.f, +1.f, 0.f, 0.f, 0.f,
		+1.f, +1.f, 0.f, 1.f, 0.f, //Triangle1
		-1.f, -1.f, 0.f, 0.f, 1.f,
		+1.f, +1.f, 0.f, 1.f, 0.f,
		+1.f, -1.f, 0.f, 1.f, 1.f //Triangle2
	};

	glGenBuffers(1, &m_VBOTexRect);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOTexRect);
	glBufferData(GL_ARRAY_BUFFER, sizeof(texRect), texRect, GL_STATIC_DRAW);
}

u_int Renderer::CreatePngTexture(const STD string& filePath) const
{
	u_int temp;
	glGenTextures(1, &temp);

	//Load Pngs
	// Load file and decode image.
	STD vector<unsigned char> image;
	unsigned width, height;
	unsigned error = lodepng::decode(image, width, height, filePath);

	glBindTexture(GL_TEXTURE_2D, temp);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &image[0]);

	return temp;
}

void Renderer::DeleteTexture(u_int texID) const
{
	glDeleteTextures(1, &texID);
}

void Renderer::AddShader(u_int ShaderProgram, const char* pShaderText, u_int ShaderType) const
{
	//쉐이더 오브젝트 생성
	u_int ShaderObj = glCreateShader(ShaderType);

	if (ShaderObj == 0) {
		fprintf(stderr, "Error creating shader type %d\n", ShaderType);
	}

	const GLchar* p[1];
	p[0] = pShaderText;
	GLint Lengths[1];
	Lengths[0] = strlen(pShaderText);
	//쉐이더 코드를 쉐이더 오브젝트에 할당
	glShaderSource(ShaderObj, 1, p, Lengths);

	//할당된 쉐이더 코드를 컴파일
	glCompileShader(ShaderObj);

	GLint success;
	// ShaderObj 가 성공적으로 컴파일 되었는지 확인
	glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);
	if (!success) {
		GLchar InfoLog[1024];

		//OpenGL 의 shader log 데이터를 가져옴
		glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
		fprintf(stderr, "Error compiling shader type %d: '%s'\n", ShaderType, InfoLog);
		printf("%s \n", pShaderText);
	}

	// ShaderProgram 에 attach!!
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

bool Renderer::ReadFile(char* filename, STD string *target) const
{
	STD ifstream file(filename);
	if (file.fail())
	{
		STD cout << filename << " file loading failed.. \n";
		file.close();
		return false;
	}
	STD string line;
	while (getline(file, line)) {
		target->append(line.c_str());
		target->append("\n");
	}
	return true;
}

u_int Renderer::CompileShaders(char* filenameVS, char* filenameFS) const
{
	u_int ShaderProgram = glCreateProgram(); //빈 쉐이더 프로그램 생성

	if (ShaderProgram == 0) { //쉐이더 프로그램이 만들어졌는지 확인
		fprintf(stderr, "Error creating shader program\n");
	}

	STD string vs, fs;

	//shader.vs 가 vs 안으로 로딩됨
	if (!ReadFile(filenameVS, &vs)) {
		printf("Error compiling vertex shader\n");
		return -1;
	};

	//shader.fs 가 fs 안으로 로딩됨
	if (!ReadFile(filenameFS, &fs)) {
		printf("Error compiling fragment shader\n");
		return -1;
	};

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

#ifdef CYAN_DEBUG_COLLISION
void XM_CALLCONV Renderer::DrawCollisionRect(DX FXMVECTOR Position, DX FXMVECTOR Size) const
{
	DX XMVECTOR GLSize = GetGLSize(Size);

	DX XMVECTOR GLPos = GetGLPos(Position);
	DX XMVECTOR BlueRect = GLPos;
	DX XMVECTOR GreenRect = GLPos;
	DX XMVECTOR YellowRect = GLPos;

	DX SetY(&BlueRect, DX GetY(BlueRect) + DX GetZ(BlueRect));
	DX XMVECTOR BlueSize = DX Swizzle(GLSize, 0, 2, 2, 3);
	DX SetY(&GreenRect, DX GetY(BlueRect) + DX GetZ(GLSize));
	DX SetY(&YellowRect, DX GetY(BlueRect) - DX GetZ(GLSize));

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

void XM_CALLCONV Renderer::DrawTexRect(DX FXMVECTOR Position, DX FXMVECTOR Size, DX FXMVECTOR Color, u_int TexID) const
{
	DX XMVECTOR GLPos = GetGLPos(Position);
	DX XMVECTOR GLSize = GetGLSize(Size);
	DrawTexture(GLPos, GLSize, Color, TexID);
}

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

	u_int shader = m_TextureRectSeqShader;

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
	glUniform1f(u_CurrSeqY, DX GetY(CurrentSprite));
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