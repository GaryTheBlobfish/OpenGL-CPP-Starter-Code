#include "VertexBuffer.h"

#include "../Renderer.h"
#include "../../utils/Logging.h"
#include "../../code_assets/Quad.h"

VertexBuffer::VertexBuffer(const void* data, unsigned int size)
{
	/*
		Generates the VBO and then binds it to type
		GL_ARRAY_BUFFER and puts the data variable,
		data, into the buffer and supplies the size
		of the data variable.
	*/
	GLCall(glGenBuffers(1, &vBufferID));
	// now need to select buffer by binding
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, vBufferID));
	FileLogging::writeToTextFile("GL: Successfully binded vertex buffers\n");
	GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

VertexBuffer::~VertexBuffer() /* delete Vertex Buffer */
{
	/*
		Deletes the VBO.
	*/
	GLCall(glDeleteBuffers(1, &vBufferID));
}

void VertexBuffer::bindVB() const
{
	/*
		Binds the VBO. This is already done in the constructor,
		so when we first create the VBO we don't need to bind 
		it, but we still need to bind it in the game loop.
	*/
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, vBufferID));
}

void VertexBuffer::unBindVB() const
{
	/*
		Unbinds the VBO. This has to be called every frame for
		every VBO.
	*/
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}
