#version 330 core

layout(location = 0) in vec4 pos;
layout(location = 1) in vec3 color;
layout(location = 2) in vec2 tex;

out vec4 vertexBasePos;
out vec4 vertexScreenPos;
out vec3 vertexColor;
out vec2 vertexTex;

uniform float u_Time;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;
uniform mat4 u_MVP;

void main()
{
    vec4 newPos = u_MVP * pos;
    vertexBasePos = pos;
    vertexScreenPos = newPos;
    vertexColor = color;
    vertexTex = tex;
    gl_Position = newPos;
}