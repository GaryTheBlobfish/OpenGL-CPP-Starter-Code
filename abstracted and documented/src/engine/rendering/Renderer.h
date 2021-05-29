#pragma once

#include <GL/glew.h>

/************************ERROR HANDLING************************/

/*
	A backslash allows you to define a macro over multiple
	lines.
*/

/*
	If the x which is parsed into ASSERT() returns false,
	a debug break will occur which stops the process of the
	program and shows us where the error is. Only works in
	Debug mode (F5 not Ctrl + F5).
*/
#define ASSERT(x) \
if (!(x)) \
__debugbreak();


/*
	If there is an error, call GLLogCall() (see Renderer.cpp)
	Surround every OpenGL function with GLCall().
*/
#define GLCall(x) GLClearError();\
	x;\
	ASSERT(GLLogCall(#x, __FILE__, __LINE__))

void GLClearError();

bool GLLogCall(const char* function, const char* file, int line);