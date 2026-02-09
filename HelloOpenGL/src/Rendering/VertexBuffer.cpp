#include "VertexBuffer.h"

VertexBuffer::VertexBuffer(const float* vertices, unsigned int size)
{
	unsigned int vbo;
	glGenBuffers(1, &vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	//TODO: perhaps make one version for each "flag" (STATIC/DYNAMIC/STREAM & DRAW/READ/COPY)
	glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);

	m_HandlerID = vbo;
}

VertexBuffer::~VertexBuffer()
{
	glDeleteBuffers(1, &m_HandlerID);
}

void VertexBuffer::Bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, m_HandlerID);
}

void VertexBuffer::Unbind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}