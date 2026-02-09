#version 330 core

layout(location = 0) in vec4 pos;
//layout(location = 1) in vec4 color; // kept if needed later but for now: unused
layout(location = 2) in vec2 tex;
//layout(location = 3) in vec3 normal; //for universal use of the same VAO (lit/unlit scenarios)

out vec4 vertexColor;
out vec2 vertexTex;

uniform vec4 u_DynamicColor = vec4(1);
uniform mat4 u_MVP;

void main()
{
    vertexColor = u_DynamicColor;
    vertexTex = tex;
    gl_Position = u_MVP * pos;
}