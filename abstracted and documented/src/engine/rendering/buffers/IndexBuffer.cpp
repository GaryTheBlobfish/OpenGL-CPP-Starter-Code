#include "IndexBuffer.h"

#include "../Renderer.h"
#include "../../utils/Logging.h"
#include "../../code_assets/Quad.h"

IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count)
	/*
		Initialiser list.
	*/
	: m_Count(count)
{
	/*
		Generates the IBO and then binds it to type
		GL_ELEMENT_ARRAY_BUFFER (even though it seems
		to work with GL_ARRAY_BUFFER as well) and puts
		the data variable, data, into the buffer and 
		supplies the size of the data variable.
	*/
	GLCall(glGenBuffers(1, &iBufferID));
	// now need to select buffer by binding
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iBufferID));
	FileLogging::writeToTextFile("GL: Successfully binded index buffers\n");
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));
}

IndexBuffer::~IndexBuffer() /* delete Index Buffer */
{
	/*
		Deletes the IBO.
	*/
	GLCall(glDeleteBuffers(1, &iBufferID));
}

void IndexBuffer::bindIB() const
{
	/*
		Binds the IBO. This is already done in the constructor,
		so when we first create the IBO we don't need to bind
		it, but we still need to bind it in the game loop.
	*/
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iBufferID));
}

void IndexBuffer::unBindIB() const
{
	/*
		Unbinds the IBO. This has to be called every frame for
		every IBO.
	*/
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}
