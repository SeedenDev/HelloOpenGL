#version 330 core

layout(location = 0) in vec4 pos;
layout(location = 1) in vec3 color;
layout(location = 2) in vec2 tex;

out vec4 vertexBasePos;
out vec4 vertexRealPos;
out vec3 vertexColor;
out vec2 vertexTex;

uniform float u_Time;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

void main()
{
    vec4 newPos = u_Projection * u_View * u_Model * pos;
    vertexBasePos = pos;
    vertexRealPos = newPos;
    vertexColor = color;
    vertexTex = tex;
    gl_Position = newPos;
}