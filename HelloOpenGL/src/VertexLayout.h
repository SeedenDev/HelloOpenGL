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
	unsigned int m_Stride;

public:
	//TODO: maybe make it template<>
	void addAttribute(GLenum type, GLint count, GLboolean normalized)
	{
		m_Elements.push_back({ type, count, normalized });
		m_Stride += count * GLUtil::GetSizeOfGLType(type);
	}

	inline const std::vector<LayoutAttribute>& GetElements() const { return m_Elements; }
	inline const unsigned int getStride() const { return m_Stride; }
};