#pragma once

class VertexBuffer
{
private:
	unsigned int vBufferID;
public:
	VertexBuffer(const void* data, unsigned int size);
	~VertexBuffer();

	void bindVB() const;
	void unBindVB() const;
};