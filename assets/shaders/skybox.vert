#version 430 core

layout(location = 0) in vec3 pos;

out vec3 vertexPos;

layout(std140, binding = 0) uniform ProjViewMatrices {
	mat4 u_View;
	mat4 u_Projection;
};

void main()
{
	vec3 mirroredPos = vec3(pos.x, pos.y, pos.z);
	vertexPos = mirroredPos;
	//Memo: we need to remove the translation part of the matrix, therefore the cast to mat3
	gl_Position = (u_Projection * mat4(mat3(u_View)) * vec4(pos, 1)).xyww;
}