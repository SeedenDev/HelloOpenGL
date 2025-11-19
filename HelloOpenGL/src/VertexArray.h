#pragma once

#include <GL/glew.h>

class VertexArray {
public:
	VertexArray();
	void Bind() const;
	void Unbind() const;
private:
	unsigned int m_HandlerID;
};