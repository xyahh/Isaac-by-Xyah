#version 330

out vec4 FragColor;

in vec2 v_TexPosition;

uniform sampler2D u_Texture;
uniform float u_TotalSeqX;
uniform float u_TotalSeqY;
uniform float u_CurrSeqX;
uniform float u_CurrSeqY;
uniform vec4 u_Color;
uniform float u_Depth;

void main()
{
	vec2 newTexPos;
	newTexPos.x = u_CurrSeqX/u_TotalSeqX + v_TexPosition.x/u_TotalSeqX;
	newTexPos.y = u_CurrSeqY/u_TotalSeqY + v_TexPosition.y/u_TotalSeqY;
	FragColor = texture2D(u_Texture, newTexPos)* u_Color;
}
