#version 330 core

layout(location = 0) in vec2 pos;
layout(location = 1) in vec2 tex;

out vec2 vertexTex;

uniform mat4 u_Model = mat4(1);

void main()
{
	vertexTex = tex;
	gl_Position = u_Model * vec4(pos, 0, 1);
}