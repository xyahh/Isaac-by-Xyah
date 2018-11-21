#version 330

in vec3 a_Position;
in vec2 a_TexPosition;

out vec2 v_TexPosition;

uniform vec3 u_Trans;
uniform vec2 u_Size;


void main()
{
	vec4 newPosition;
	newPosition.xy = a_Position.xy*u_Size.xy + u_Trans.xy;
	newPosition.z = u_Trans.z;
	newPosition.w= 1;
	gl_Position = newPosition;

	v_TexPosition = a_TexPosition;
}
