#version 430 core

layout(location = 0) in vec3 pos;

out vec3 vertexPos;

uniform mat4 u_View;
uniform mat4 u_Projection;

void main()
{
	vec3 mirroredPos = vec3(pos.x, pos.y, -pos.z); // later: better to mirror the Z in the vertices data
	vertexPos = mirroredPos;
	gl_Position = (u_Projection * u_View * vec4(pos, 1)).xyww;
}