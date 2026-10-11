#pragma once

#include <string>

class Texture {
private:
	unsigned int m_HandlerID = 0;
	std::string m_TexturePath;
	unsigned char* m_DataBuffer = nullptr;
	int m_Width = 0, m_Height = 0, m_bytesPerChannel = 0;
	unsigned int m_LastSlot = 0;
	int index;

public:
	explicit Texture(const std::string& texturePath);
	Texture(const unsigned char* data, int length);
	~Texture();

	void Bind(unsigned int slot);
	void Unbind() const;

	int GetWidth() const { return m_Width; }
	int GetHeight() const { return m_Height; }
	unsigned int GetSlot() const { return m_LastSlot; }

private:
	void SetupGLData();
};