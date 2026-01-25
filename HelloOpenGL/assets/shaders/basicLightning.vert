#version 330 core

layout(location = 0) in vec4 pos;
layout(location = 1) in vec3 color;
layout(location = 2) in vec2 tex;
layout(location = 3) in vec3 normal;

out vec4 vertexBasePos;
out vec3 vertexWorldPos;
out vec4 vertexScreenPos;
out vec3 vertexColor; // useless because mat color
out vec2 vertexTex;
out vec3 vertexBaseNormal;
out vec3 vertexComputedNormal;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;
uniform mat4 u_MVP;

void main()
{
    vertexBasePos = pos;
    vertexWorldPos = vec3(u_Model * pos);
    vertexScreenPos = u_MVP * pos;
    vertexBaseNormal = normal;
    vertexComputedNormal = mat3(transpose(inverse(u_Model))) * normal;
    vertexColor = color;
    vertexTex = tex;
    gl_Position = vertexScreenPos;
}