#pragma once

class VertexBuffer
{
private:
	unsigned int m_HandlerID;

public:
	VertexBuffer(const float* vertices, unsigned int size);
	~VertexBuffer();

	void Bind() const;
	void Unbind() const;
};