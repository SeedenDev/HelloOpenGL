#include "VertexLayout.h"

template<typename T>
void VertexLayout::AddAttr(GLint count)
{
	static_assert(false);
}

template<>
void VertexLayout::AddAttr<float>(GLint count)
{
	AddAttribute(GL_FLOAT, count, GL_FALSE);
}

template<>
void VertexLayout::AddAttr<double>(GLint count)
{
	AddAttribute(GL_DOUBLE, count, GL_FALSE);
}