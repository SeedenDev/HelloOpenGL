#version 330 core
//TODO: remove when there'll be infinite texture layers support (maybe not infinite but like an array of 10 samplers?)
out vec4 outColor;

in vec4 vertexColor;
in vec2 vertexTex;

uniform sampler2D u_TextureLower;
uniform sampler2D u_TextureUpper;

void main()
{
    vec4 lowerTex = texture(u_TextureLower, vertexTex);
    vec4 upperTex = texture(u_TextureUpper, vertexTex);
    vec4 texColor = mix(lowerTex, upperTex, 0.3);
    vec4 color = texColor * vertexColor;
    outColor = color;
}