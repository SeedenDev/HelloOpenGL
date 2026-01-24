#version 330 core

layout(location = 0) in vec4 pos;
layout(location = 1) in vec3 color;

out vec4 vertexBasePos;
out vec4 vertexScreenPos;
out vec3 vertexColor;

uniform mat4 u_MVP;

void main()
{
    vec4 newPos = u_MVP * pos;
    vertexBasePos = pos;
    vertexScreenPos = newPos;
    vertexColor = color;
    gl_Position = newPos;
}