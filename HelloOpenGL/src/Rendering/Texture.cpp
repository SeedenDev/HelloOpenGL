#include "Texture.h"

#include <stb_image/stb_image.h>
#include <GL/glew.h>
#include <iostream>

static int i = 0; //TODO: remove or index/identifier in the asset storage (for debug)

Texture::Texture(const std::string& texturePath)
	: m_HandlerID(0), m_TexturePath(texturePath), m_DataBuffer(nullptr), m_Width(0), m_Height(0), m_bytesPerChannel(0)
{
	stbi_set_flip_vertically_on_load(1);
	m_DataBuffer = stbi_load(texturePath.c_str(), &m_Width, &m_Height, &m_bytesPerChannel, 4);
	SetupGLData();

	index = i++;
	std::cout << "texture(file) constructor" << std::to_string(index) << "|" << std::to_string(m_HandlerID) << std::endl;
}

Texture::Texture(const unsigned char* data, int length)
	: m_HandlerID(0), m_TexturePath(""), m_DataBuffer(nullptr), m_Width(0), m_Height(0), m_bytesPerChannel(0)
{
	stbi_set_flip_vertically_on_load(1);
	m_DataBuffer = stbi_load_from_memory(data, length, &m_Width, &m_Height, &m_bytesPerChannel, 4);
	SetupGLData();

	index = i++;
	std::cout << "texture(mem) constructor" << std::to_string(index) << "|" << std::to_string(m_HandlerID) << std::endl;
}

Texture::~Texture()
{
	glDeleteTextures(1, &m_HandlerID);
	std::cout << "texture destructor" << std::to_string(index) << "|" << std::to_string(m_HandlerID) << std::endl;
}

void Texture::Bind(unsigned int slot)
{
	m_LastSlot = slot;
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, m_HandlerID);
}

void Texture::Unbind() const
{
	glActiveTexture(GL_TEXTURE0 + m_LastSlot);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::SetupGLData()
{
	glGenTextures(1, &m_HandlerID);
	glActiveTexture(GL_TEXTURE0);
	GLint lastHandle;
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &lastHandle);
	glBindTexture(GL_TEXTURE_2D, m_HandlerID);

	//TODO: a way to choose it for every texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST); // GL_NEAREST ; GL_NEAREST_MIPMAP_NEAREST ; GL_NEAREST_MIPMAP_LINEAR 
	// ; GL_LINEAR ; GL_LINEAR_MIPMAP_LINEAR ; GL_LINEAR_MIPMAP_NEAREST
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); // only GL_NEAREST or GL_LINEAR
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // GL_REPEAT ; GL_MIRRORED_REPEAT ; GL_CLAMP_TO_EDGE
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_DataBuffer);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, lastHandle);

	if (m_DataBuffer)
		stbi_image_free(m_DataBuffer);
}