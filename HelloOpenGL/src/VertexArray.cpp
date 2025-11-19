#include "VertexArray.h"

VertexArray::VertexArray()
{
	unsigned int vao;
	glGenVertexArrays(1, &vao);
		
	m_HandlerID = vao;
}
void VertexArray::Bind() const
{
	glBindVertexArray(m_HandlerID);
}
void VertexArray::Unbind() const
{
	glBindVertexArray(0);
}