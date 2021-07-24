#pragma once

class VertexBuffer
{
private:
	unsigned int m_RendererID;

public:
	VertexBuffer(const void* data, unsigned int size); // public constructor
	~VertexBuffer(); // destructor

	void Bind() const;  
	void Unbind() const;
};