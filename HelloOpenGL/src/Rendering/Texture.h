#pragma once

#include <string>

class Texture {
private:
	unsigned int m_HandlerID;
	std::string m_TexturePath;
	unsigned char* m_DataBuffer= nullptr;
	int m_Width, m_Height, m_bytesPerChannel;
	unsigned int m_LastSlot = 0;
	int index;

public:
	Texture(const std::string& texturePath);
	Texture(const unsigned char* data, int length);
	~Texture();
	Texture(const Texture& other);
	Texture(Texture&& other) noexcept;
	Texture& operator=(Texture&& other) noexcept;

	void Bind(unsigned int slot);
	void Unbind() const;

	inline int GetWidth() const { return m_Width; }
	inline int GetHeight() const { return m_Height; }

private:
	void SetupGLData();
};