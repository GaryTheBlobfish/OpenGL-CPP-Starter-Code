#shader vertex
// always make sure first line in shader file is the #shader type

/****************VERTEX****************/
#version 330 core

layout(location = 0) in vec4 position;

void main()
{
	gl_Position = position;
};

#shader fragment
/****************FRAGMENT****************/
#version 330 core

layout(location = 0) out vec4 color;

uniform vec4 u_Color;

void main()
{
	color = u_Color;
};
