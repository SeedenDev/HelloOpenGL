#pragma once

#include <GL/glew.h>

class VertexBuffer {
public:
	VertexBuffer(const float vertices[]);
	void Bind() const;
	void Unbind() const;
private:
	unsigned int m_HandlerID;
};