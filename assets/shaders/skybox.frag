#version 430 core

out vec4 outColor;

in vec3 vertexPos; // used as the direction vector

uniform samplerCube u_Skybox;

void main()
{
	outColor = texture(u_Skybox, vertexPos);
}