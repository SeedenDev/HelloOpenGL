#version 430 core

layout(location = 0) in vec4 pos;
//layout(location = 1) in vec4 color; // for universal use of the same VAO (lit/unlit scenarios) but useless for this lighting shader (=> use of material properties)
layout(location = 2) in vec2 tex;
layout(location = 3) in vec3 normal; 

out vec4 vertexBasePos;
out vec3 vertexWorldPos;
out vec4 vertexScreenPos;
out vec2 vertexTex;
out vec3 vertexBaseNormal;
out vec3 vertexComputedNormal;

layout(std140, binding = 0) uniform ProjViewMatrices {
                        // base alignement:     // aligned offset
    mat4 u_View;        // 64 (4N per column)   // 0
    mat4 u_Projection;  // Same                 // 64
};
uniform mat4 u_Model;

void main()
{
    vertexBasePos = pos;
    vertexWorldPos = vec3(u_Model * pos);
    vertexScreenPos = u_Projection * u_View * u_Model * pos;
    vertexBaseNormal = normal;
    vertexComputedNormal = mat3(transpose(inverse(u_Model))) * normal;
    vertexTex = tex;
    gl_Position = vertexScreenPos;
}