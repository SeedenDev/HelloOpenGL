#pragma once

#include "VertexBuffer.h"
#include "VertexLayout.h"

class VertexArray
{
private:
	unsigned int m_HandlerID;

public:
	VertexArray();
	~VertexArray();

	void ApplyLayout(const VertexBuffer& vbo, const VertexLayout& vLayout) const;
	void Bind() const;
	void Unbind() const;
};