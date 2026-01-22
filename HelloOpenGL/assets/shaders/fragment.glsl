#version 330 core
   
out vec4 outColor;

in vec4 vertexBasePos;
in vec4 vertexRealPos;
in vec3 vertexColor;
in vec2 vertexTex;

uniform float u_Time;
uniform sampler2D u_Texture0;
uniform sampler2D u_Texture1;

void main()
{
    vec4 lowerTex = texture(u_Texture0, vertexTex);
    vec4 upperTex = texture(u_Texture1, vertexTex);
    vec4 texColor = mix(lowerTex, upperTex, upperTex.a*0.5);
    vec4 color = texColor.rgba * vec4(vertexColor, 1.0) * vec4(vertexRealPos.xy, 1.0, 1.0);
    //color.r *= sin(u_Time)*cos(u_Time);
    outColor = color;
}