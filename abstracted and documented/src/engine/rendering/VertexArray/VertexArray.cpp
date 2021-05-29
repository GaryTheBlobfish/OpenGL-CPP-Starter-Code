#include "VertexArray.h"
#include "../Renderer.h"

VertexArray::VertexArray()
{
	/*
		Generate Vertex Arrays.
	*/
	GLCall(glGenVertexArrays(1, &vArrayID));
}

VertexArray::~VertexArray()
{
	/*
		Delete Vertex Arrays.
	*/
	GLCall(glDeleteVertexArrays(1, &vArrayID));
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
	/*
		First, this method binds the VBO and the VAO. It 
		then sets up the parsed VertexBufferLayout.
	*/

	BindVA();
	vb.bindVB();

	/* setup VertexBufferLayout */
	const auto& elements = layout.GetElements();
	unsigned int offset = 0;
	for (unsigned int i = 0; i < elements.size(); i++)
	{
		const auto& element = elements[i];
		GLCall(glEnableVertexAttribArray(i)); /* enables the generic vertex attribute array specified by i */
		GLCall(glVertexAttribPointer(i,
			element.count, element.type, element.normalised,
			layout.GetStride(), (const void *)offset) 
			/*
				The stride parameter in glVertexAttribPointer
				(which we are parsing as layout.GetStride())
				is what tells OpenGL the space between each
				new item in the array.
			
				So if we had a float[] and specified the stride
				as 2, every 2 bits of data in the float[] would 
				be treated as a singular variable on OpenGL's 
				side of things.
				
				One way to think of it is if we set the stride as
				3, we are saying that our float[] is an array of
				vec3 types and the first in a group of 3 in the 
				array is the X component, the second, the Y, the
				third the Z.
				
				See docs.gl/gl4/glVertexAttribPointer for a better,
				more professional explanation.
			*/
		);
		/* Increment offset each time */
		offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
	}
}

void VertexArray::BindVA() const
{
	/*
		Binds the VAO. This is NOT done in the constructor
		and so we need to bind the VAO at creation as well
		as in the game loop each frame.
	*/
	GLCall(glBindVertexArray(vArrayID));
}

void VertexArray::UnBindVA() const
{
	/*
		Unbinds the VAO. This needs to be called every
		frame.
	*/
	GLCall(glBindVertexArray(0));
}