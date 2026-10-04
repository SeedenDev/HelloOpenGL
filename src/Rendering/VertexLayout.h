#pragma once

#include <glad/glad.h>
#include <vector>

#include "GLUtil.h"

struct LayoutAttribute
{
	GLenum type;
	GLint count;
	GLboolean normalized;
};

class VertexLayout
{
private:
	std::vector<LayoutAttribute> m_Elements;
	unsigned int m_Stride = 0;

	void AddAttribute(const GLenum type, const GLint count, const GLboolean normalized)
	{
		m_Elements.push_back({ type, count, normalized });
		m_Stride += count * GLUtil::GetSizeOfGLType(type);
	}

public:

	template<typename T>
	void AddAttr(GLint count);

	const std::vector<LayoutAttribute>& GetElements() const { return m_Elements; }
	unsigned int GetStride() const { return m_Stride; }
};