#pragma once
/*
Copyright 2017 Lee Taek Hee (Korea Polytech University)
This program is free software: you can redistribute it and/or modify
it under the terms of the What The Hell License. Do it plz.
This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY.
*/

enum LayerGroup
{
	Background,
	Middleground,
	Foreground,
};

class Renderer
{
	friend Cyan;

public:
	Renderer() {}
	~Renderer() {}

	void Prepare();

	void SSE_CALLCONV DrawShadow(SSE_VECTOR_PARAM1 Position, SSE_VECTOR_PARAM1 Size, float Alpha) const;
	void SSE_CALLCONV DrawSprite( SSE_VECTOR_PARAM1 Position, SSE_VECTOR_PARAM1 Size,
		const  FLOAT4& Color, u_int TexID, const  UINT2& CurrentSprite, const  UINT2& TotalSprite,
		u_int LayerGrouping) const;

	u_int GenerateTexture(const STD string& filePath) const;
	void DeleteTexture(u_int texID) const;

	void UpdateWindow(int X, int Y);
	void UpdateScale(float N);

private:
	void SSE_CALLCONV DrawTexRect(const FLOAT3& Position, const FLOAT2& Size, const FLOAT4& Color, u_int TexID) const;

	SSE_VECTOR SSE_CALLCONV GLTransform(SSE_VECTOR_PARAM1 v, u_int LayerGrouping) const;

	bool Initialize();
	void RecalculateRatio();
	bool ReadFile(const STD string& filename, STD string *target) const;
	void AddShader(u_int ShaderProgram, const STD string& pShaderText, u_int ShaderType) const;
	u_int CompileShaders(const STD string&  filenameVS, const STD string& filenameFS) const;
	void CreateVertexBufferObjects();
	
private:
	float Scale;
	int WinX, WinY;
	FLOAT3 Ratio;

	u_int m_TexShadow = 0;
	u_int m_VBOTexRect = 0;
	u_int m_TextureRectShader = 0;
	u_int m_TextureSpriteShader = 0;
};