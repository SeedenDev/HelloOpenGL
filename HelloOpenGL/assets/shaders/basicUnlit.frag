#version 330 core

out vec4 outColor;

in vec4 vertexColor;
in vec2 vertexTex;

uniform sampler2D u_Texture;

void main()
{
    outColor = vertexColor * texture(u_Texture, vertexTex);
}