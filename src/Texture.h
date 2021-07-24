#pragma once

#include "Renderer.h"

class Texture
{
private:
	unsigned int m_RendererID;
	std::string m_FilePath; // keep track of actual filepath
	unsigned char* m_LocalBuffer; // local storage for texture
	int m_Width, m_Height, m_BPP; // BPP = Bits per pixel

public:
	Texture(const std::string& path);
	~Texture();

	void Bind(unsigned int slot = 0) const; // optional parameter for specify the slot to bind the texture to 
	void Unbind() const;

	inline int GetWidth() const { return m_Width; }
	inline int GetHeight() const { return m_Height; }
};