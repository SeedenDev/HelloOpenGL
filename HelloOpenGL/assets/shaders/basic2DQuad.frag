#version 330 core

out vec4 outColor;

in vec2 vertexTex;

uniform vec4 u_DynamicColor = vec4(1);
uniform sampler2D u_Texture;

void main()
{
	outColor = u_DynamicColor * texture(u_Texture, vertexTex);
}