#version 330 core

layout(location = 0) in vec4 pos;
layout(location = 1) in vec3 color;
        
out vec3 vertexColor;
out vec3 vertexPos;

uniform float u_Time;

void main()
{
    vec4 newPos = pos;
    newPos.x += sin(u_Time);
    newPos.y *= cos(u_Time);
    gl_Position = newPos;
    vertexPos = pos.xyz;
    vertexColor = color;
}