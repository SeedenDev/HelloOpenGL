#pragma once

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "Scene/Quad.h"
#include "Shader.h"

const float g_ScreenQuadVertices[] = {
	// pos  // texture
	-1, -1, 0.0, 0.0, // 0 (bottom-left)
	 1, -1, 1.0, 0.0, // 1 (bottom-right)
	 1,  1, 1.0, 1.0, // 2 (top-right)
	-1,  1, 0.0, 1.0  // 3 (top-left)
};

class Framebuffer
{
private:
	unsigned int m_Fb, m_ColorTexture, m_Rbo;
	VertexArray m_Vao;
	VertexBuffer m_Vbo = VertexBuffer(g_ScreenQuadVertices, sizeof(g_ScreenQuadVertices));
	IndexBuffer m_Ibo = IndexBuffer(g_QuadIndices, sizeof(g_QuadIndices));
	int m_Width, m_Height;

public:
	Framebuffer(int width, int height);
	~Framebuffer();

	void Bind(); 
	void Unbind();
	void Draw(Shader& shader);//TODO: in real engine, get the proper shader from the ShaderStorage ig? Because this could lead to providing the wrong shader
	void Resize(int width, int height);

	const int GetWidth() { return m_Width; }
	const int GetHeight() { return m_Height; }
};