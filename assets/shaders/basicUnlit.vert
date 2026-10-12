#version 430 core

layout(location = 0) in vec4 pos;
//layout(location = 1) in vec4 color; // kept if needed later but for now: unused
layout(location = 2) in vec2 tex;
//layout(location = 3) in vec3 normal; //for universal use of the same VAO (lit/unlit scenarios)

out vec4 vertexColor;
out vec2 vertexTex;

uniform vec4 u_DynamicColor = vec4(1);

layout(std140, binding = 0) uniform ProjViewMatrices {
    mat4 u_View;
    mat4 u_Projection;
};
uniform mat4 u_Model;

void main()
{
    vertexColor = u_DynamicColor;
    vertexTex = tex;
    gl_Position = u_Projection * u_View * u_Model * pos;
}