#pragma once

#include <GL/glew.h>

namespace GLUtil
{
	static unsigned int GetSizeOfGLType(GLenum type)
	{
		switch (type)
		{
		case GL_FLOAT: return 4;
		case GL_UNSIGNED_INT: return 4;
		}
		return 0;
	}
}