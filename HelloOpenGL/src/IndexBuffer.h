#pragma once

#include <GL/glew.h>

class IndexBuffer {
public:
	IndexBuffer(const unsigned int indices[]);
	void Bind() const;
	void Unbind() const;
private:
	unsigned int m_HandlerID;
};