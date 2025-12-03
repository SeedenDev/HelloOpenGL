#include "Texture.h"

#include <stb_image/stb_image.h>
#include <GL/glew.h>

Texture::Texture(const std::string& texturePath)
	: m_HandlerID(0), m_TexturePath(texturePath), m_DataBuffer(nullptr), m_Width(0), m_Height(0), m_bytesPerChannel(0)
{
	stbi_set_flip_vertically_on_load(1);
	m_DataBuffer = stbi_load(texturePath.c_str(), &m_Width, &m_Height, &m_bytesPerChannel, 4);

	glGenTextures(1, &m_HandlerID);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_HandlerID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // GL_NEAREST ; GL_NEAREST_MIPMAP_NEAREST ; GL_NEAREST_MIPMAP_LINEAR
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR); // GL_LINEAR ; GL_LINEAR_MIPMAP_LINEAR ; GL_LINEAR_MIPMAP_NEAREST
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // GL_REPEAT ; GL_MIRRORED_REPEAT ; GL_CLAMP_TO_EDGE
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_DataBuffer);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);

	if (m_DataBuffer)
		stbi_image_free(m_DataBuffer);
}

Texture::~Texture()
{
	glDeleteTextures(1, &m_HandlerID);
}

void Texture::Bind(unsigned int slot) const
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, m_HandlerID);
}

void Texture::Unbind() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
}