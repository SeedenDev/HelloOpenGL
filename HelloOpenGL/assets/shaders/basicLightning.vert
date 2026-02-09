#version 330 core

layout(location = 0) in vec4 pos;
//layout(location = 1) in vec4 color; // for universal use of the same VAO (lit/unlit scenarios) but useless for this lightning shader (=> use of material properties)
layout(location = 2) in vec2 tex;
layout(location = 3) in vec3 normal; 

out vec4 vertexBasePos;
out vec3 vertexWorldPos;
out vec4 vertexScreenPos;
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
    vertexTex = tex;
    gl_Position = vertexScreenPos;
}