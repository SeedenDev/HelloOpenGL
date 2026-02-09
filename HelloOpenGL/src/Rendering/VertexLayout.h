#pragma once

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

	void AddAttribute(GLenum type, GLint count, GLboolean normalized)
	{
		m_Elements.push_back({ type, count, normalized });
		m_Stride += count * GLUtil::GetSizeOfGLType(type);
	}

public:

	template<typename T>
	void AddAttr(GLint count)
	{
		static_assert(false);
	}

	template<>
	void AddAttr<float>(GLint count)
	{
		AddAttribute(GL_FLOAT, count, GL_FALSE);
	}

	template<>
	void AddAttr<double>(GLint count)
	{
		AddAttribute(GL_DOUBLE, count, GL_FALSE);
	}

	inline const std::vector<LayoutAttribute>& GetElements() const { return m_Elements; }
	inline const unsigned int GetStride() const { return m_Stride; }
};