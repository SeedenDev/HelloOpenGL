#version 330 core
   
out vec4 outColor;

in vec3 vertexColor;
in vec3 vertexPos;

uniform float u_Time;

void main()
{
    vec3 color = vertexColor.rgb;
    color.r *= sin(u_Time)*cos(u_Time);
    outColor = vec4(color, 1.0);
}