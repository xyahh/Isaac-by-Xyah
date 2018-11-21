#pragma once
/*
Copyright 2017 Lee Taek Hee (Korea Polytech University)

This program is free software: you can redistribute it and/or modify
it under the terms of the What The Hell License. Do it plz.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WSTD vectorANTY.
*/


class Renderer
{
public:
	Renderer() {}
	~Renderer() {}

	bool Initialize(int windowSizeX, int windowSizeY);

#ifdef CYAN_DEBUG_COLLISION
	void XM_CALLCONV DrawCollisionRect(DX FXMVECTOR Position, DX FXMVECTOR Size);
#endif
	void XM_CALLCONV DrawSprite(DX FXMVECTOR Position, DX FXMVECTOR Size, DX FXMVECTOR Color, u_int TexID, DX GXMVECTOR SpriteInfo);
	void XM_CALLCONV DrawTexRect(DX FXMVECTOR Position, DX FXMVECTOR Size, DX FXMVECTOR Color, u_int TexID);
	void XM_CALLCONV DrawShadow(DX FXMVECTOR Position, DX FXMVECTOR Size, DX FXMVECTOR Color);

	u_int CreatePngTexture(const STD string& filePath);
	void DeleteTexture(u_int texID);


private:


	void XM_CALLCONV DrawTexture(DX FXMVECTOR Position, DX FXMVECTOR Size, DX FXMVECTOR Color, u_int TexID);
	bool ReadFile(char* filename, STD string *target);
	void AddShader(u_int ShaderProgram, const char* pShaderText, u_int ShaderType);
	u_int CompileShaders(char* filenameVS, char* filenameFS);
	void CreateVertexBufferObjects();

	DX XMVECTOR XM_CALLCONV GetGLPosition(DX FXMVECTOR Position);
	
private:	
	bool m_Initialized = false;
	
	u_int m_WindowSizeX = 0;
	u_int m_WindowSizeY = 0;

	u_int m_TexShadow = 0;

#ifdef CYAN_DEBUG_COLLISION
	u_int m_DebugRect = 0;
#endif

	u_int m_VBORect = 0;
	u_int m_SolidRectShader = 0;

	u_int m_VBOTexRect = 0;
	u_int m_TextureRectShader = 0;
	u_int m_TextureRectSeqShader = 0;
};

extern Renderer RenderDevice;