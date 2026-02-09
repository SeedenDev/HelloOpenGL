#pragma once

#include <GL/glew.h>
#include "VertexBuffer.h"
#include "VertexLayout.h"

class VertexArray
{
private:
	unsigned int m_HandlerID;

public:
	VertexArray();
	~VertexArray();

	void ApplyLayout(VertexBuffer& vbo, VertexLayout& vLayout);
	void Bind() const;
	void Unbind() const;
};