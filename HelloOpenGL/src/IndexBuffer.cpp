#include "IndexBuffer.h"

IndexBuffer::IndexBuffer(const unsigned int* indices, unsigned int count)
	: m_Count(count)
{
	unsigned int ibo;
	glGenBuffers(1, &ibo);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, count, indices, GL_STATIC_DRAW);

	m_HandlerID = ibo;
}

IndexBuffer::~IndexBuffer()
{
	glDeleteBuffers(1, &m_HandlerID);
}

void IndexBuffer::Bind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_HandlerID);
}
void IndexBuffer::Unbind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

unsigned int IndexBuffer::GetCount() const
{
	return m_Count;
}