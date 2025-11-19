#pragma once

#include <GL/glew.h>

class IndexBuffer
{
private:
	unsigned int m_HandlerID;

public:
	IndexBuffer(const unsigned int* indices, unsigned int count);
	~IndexBuffer();

	void Bind() const;
	void Unbind() const;
};