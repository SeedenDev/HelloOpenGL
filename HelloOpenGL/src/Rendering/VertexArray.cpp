#include "VertexArray.h"

#include <string>

VertexArray::VertexArray()
{
	unsigned int vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	m_HandlerID = vao;
}

VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &m_HandlerID);
}

void VertexArray::ApplyLayout(VertexBuffer& vbo, VertexLayout& vLayout)
{
	Bind();
	vbo.Bind();
	const auto& elements = vLayout.GetElements();
	unsigned int offset = 0;
	for (unsigned int i = 0; i < elements.size(); i++)
	{
		LayoutAttribute attr = vLayout.GetElements()[i];
		glVertexAttribPointer(i, attr.count, attr.type, attr.normalized, vLayout.GetStride(), (void*) offset);
		glEnableVertexAttribArray(i);
		offset += attr.count * GLUtil::GetSizeOfGLType(attr.type);
	}
	Unbind();
	vbo.Unbind();
}

void VertexArray::Bind() const
{
	glBindVertexArray(m_HandlerID);
}

void VertexArray::Unbind() const
{
	glBindVertexArray(0);
}