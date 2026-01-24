#version 330 core

out vec4 outColor;

in vec4 vertexBasePos;
in vec4 vertexScreenPos;
in vec3 vertexColor;

void main()
{
    outColor = vec4(vertexColor, 1.0);
}