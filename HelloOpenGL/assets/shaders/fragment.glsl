#version 330 core
   
out vec4 outColor;

in vec3 vertexColor;
in vec3 vertexPos;
in vec2 vertexTex;

uniform float u_Time;
uniform sampler2D u_Texture0;
uniform sampler2D u_Texture1;

void main()
{
    //vec4 texColor = mix(texture(u_Texture0, vertexTex), texture(u_Texture1, vertexTex), 0.2);
    vec4 texColor = texture(u_Texture1, vertexTex);
    vec3 color = vertexColor.rgb;
    color.r *= sin(u_Time)*cos(u_Time);
    outColor = texColor;
}