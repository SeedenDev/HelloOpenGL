#version 330 core

layout(location = 0) in vec4 pos;
layout(location = 1) in vec3 color;
layout(location = 2) in vec2 tex;
layout(location = 3) in vec3 normal;

out vec4 vertexBasePos;
out vec3 vertexWorldPos;
out vec4 vertexScreenPos;
out vec3 vertexColor;
out vec2 vertexTex;
out vec3 vertexBaseNormal;
out vec3 vertexComputedNormal;

uniform float u_Time;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;
uniform mat4 u_MVP;

void main()
{
    vec4 newPos = u_MVP * pos;
    vertexBasePos = pos;
    vec4 worldPos = u_Model * pos;
    vertexWorldPos = worldPos.xyz;
    vertexScreenPos = newPos;
    vertexColor = color;
    vertexTex = tex;
    vertexBaseNormal = normal;
    vertexComputedNormal = mat3(transpose(inverse(u_Model))) * normal;
    gl_Position = newPos;
}