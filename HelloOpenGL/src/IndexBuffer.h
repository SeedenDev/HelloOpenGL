#pragma once

#include <GL/glew.h>

class IndexBuffer
{
private:
	unsigned int m_HandlerID;
	unsigned int m_Count;

public:
	IndexBuffer(const unsigned int* indices, unsigned int count);
	~IndexBuffer();

	void Bind() const;
	void Unbind() const;

	unsigned int GetCount() const;
};