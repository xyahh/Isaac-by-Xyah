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
	friend Cyan;

public:
	Renderer() {}
	~Renderer() {}

#ifdef CYAN_DEBUG_COLLISION
	void XM_CALLCONV DrawCollisionRect(DX FXMVECTOR Position, DX FXMVECTOR Size) const;
#endif
	void XM_CALLCONV DrawSprite(DX FXMVECTOR Position, DX FXMVECTOR Size,
		DX FXMVECTOR Color, u_int TexID, DX GXMVECTOR CurrentSprite, DX HXMVECTOR TotalSprite) const;
	void XM_CALLCONV DrawTexRect(DX FXMVECTOR Position, DX FXMVECTOR Size, DX FXMVECTOR Color, u_int TexID) const;
	void XM_CALLCONV DrawShadow(DX FXMVECTOR Position, DX FXMVECTOR Size, DX FXMVECTOR Color) const;

	u_int CreatePngTexture(const STD string& filePath) const;
	void DeleteTexture(u_int texID) const;


private:

	bool Initialize(int windowSizeX, int windowSizeY);
	void XM_CALLCONV DrawTexture(DX FXMVECTOR Position, DX FXMVECTOR Size, DX FXMVECTOR Color, u_int TexID) const;
	bool ReadFile(char* filename, STD string *target) const;
	void AddShader(u_int ShaderProgram, const char* pShaderText, u_int ShaderType) const;
	u_int CompileShaders(char* filenameVS, char* filenameFS) const;
	void CreateVertexBufferObjects();
	DX XMVECTOR XM_CALLCONV GetGLPos(DX FXMVECTOR Position) const;
	DX XMVECTOR XM_CALLCONV GetGLSize(DX FXMVECTOR Size) const;

private:

	bool m_Initialized = false;

	u_int m_WindowSizeX = 0;
	u_int m_WindowSizeY = 0;

	u_int m_TexShadow = 0;

#ifdef CYAN_DEBUG_COLLISION
	u_int m_DebugRect = 0;
#endif

	u_int m_VBOTexRect = 0;
	u_int m_TextureRectShader = 0;
	u_int m_TextureRectSeqShader = 0;
};