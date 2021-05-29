#pragma once
#include <vector>
#include "GL/glew.h"
#include "../Renderer.h"

struct VertexBufferElement
{
	unsigned int type;
	unsigned int count;
	unsigned char normalised;

	static unsigned int GetSizeOfType(unsigned int type)
	{
		switch (type)
		{
		case GL_FLOAT:			return 4;
		case GL_UNSIGNED_INT:	return 4;
		case GL_UNSIGNED_BYTE:	return 1;
		}

		ASSERT(false);
		return 0;
	}
};

class VertexBufferLayout
{
	/*
		The layout for the Vertex Buffer to be used with
		in Vertex Array. See VertexArray.h for the reason
		why this is used
	*/
private:
	std::vector<VertexBufferElement> m_Elements;
	unsigned int m_Stride;
public:
	VertexBufferLayout()
	: m_Stride(0) {}

	template<typename T>
	void Push(unsigned int count)
	{
		static_assert(false);
	}
	
	/*
		The following is the different types of data that 
		can be parsed into the Vertex Buffer. If we need to 
		put new types of data into the VB, we can just write
		another one of these Push<>() functions and changing 
		the neccessary details. These details include the
		GL_FLOAT, GL_UNSIGNED_INT etc variables from OpenGL,
		the variable type in the <> after Push and the variable
		type in the VertexBufferElement::GetSizeOfType() methods
		at the end of each Push<>() method.
	*/

	template<>
	void Push<float>(unsigned int count)
	{
		/*
			In a very simple way, we are basically saying, here's
			count floats here, now give it to the Vertex Array to
			combine with the Vertex Buffer.
		*/

		m_Elements.push_back({ GL_FLOAT, count, GL_FALSE }); /* Add the data { GL_FLOAT, count, GL_FALSE } to the elements list */
		m_Stride += count * VertexBufferElement::GetSizeOfType(GL_FLOAT);
	}

	template<>
	void Push<unsigned int>(unsigned int count)
	{
		m_Elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
		m_Stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT);
	}

	template<>
	void Push<unsigned char>(unsigned int count)
	{
		m_Elements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
		m_Stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE);
	}

	inline const std::vector<VertexBufferElement> GetElements() const& { return m_Elements; };
	inline unsigned int GetStride() const { return m_Stride; };

};