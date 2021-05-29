#pragma once
#include "../buffers/VertexBuffer.h"
#include "../buffers/VertexBufferLayout.h"

class VertexArray
{
	/*
		What a Vertex Array needs to do is combine a Vertex Buffer 
		with a layout, which in our case is VertexBufferLayout.

		This is because a Vertex Buffer is just a bunch of data, it
		does not know that say, one part is for positions and one for
		indices.
	*/
private:
	unsigned int vArrayID;
public:
	VertexArray();
	~VertexArray();

	void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);

	void BindVA() const;
	void UnBindVA() const;
};