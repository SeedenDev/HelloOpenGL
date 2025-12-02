#pragma once

#include <string>

class Texture {
private:
	unsigned int m_HandlerID;
	std::string m_TexturePath;
	unsigned char* m_DataBuffer;
	int m_Width, m_Height, m_bytesPerChannel;

public:
	Texture(const std::string& texturePath);
	~Texture();

	void Bind(unsigned int slot = 0) const;
	void Unbind() const;

	inline int GetWidth() const { return m_Width; }
	inline int GetHeight() const { return m_Height; }
};