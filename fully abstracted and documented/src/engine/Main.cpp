/**************** C++ ****************/
#include <iostream>

/**************** OpenGL ****************/
#include <GL/glew.h>
#include <GLFW/glfw3.h>

/**************** ENGINE ****************/
#include "utils/Logging.h"
#include "rendering/Renderer.h"
#include "shader/ShaderProgram.h"
#include "rendering/buffers/VertexBuffer.h"
#include "rendering/buffers/IndexBuffer.h"
#include "rendering/VertexArray/VertexArray.h"
#include "code_assets/Quad.h"

int main(void)
{
	char title[66] = "C++ OpenGL Game  -  GaryTheBlobfish  -  garytheblobfish.github.io";
	FileLogging::clearLogFile();

	GLFWwindow* window;

	if (!glfwInit()) {
		/*
			checks if there is an error with inititalising GLFW, if there
			is, terminate the program.
		*/
		FileLogging::writeToTextFile("Terminating program\n");
		return -1;
	}
	else
		FileLogging::writeToTextFile("GLFW: Initialised GLFW library\n");

	/**** SETS UP OPENGL CORE PROFILE ****/
	FileLogging::writeToTextFile("GLFW: Setting version to 3.3 core\n");
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // version 3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // version 3.3
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // set OpenGL mode to core
	// for compatibility mode, instead of GLFW_OPENGL_CORE_PROFILE, do GLFW_OPENGL_COMPAT_PROFILE

	window = glfwCreateWindow(1600, 900, title , NULL, NULL);
	/*
		if window creation fails, the method returns null, this
		is why we check the windows validity in the if statement
		below
	*/
	FileLogging::writeToTextFile("GLFW: GLFW window created\n");

	if (!window)
	{
		/*
			checks if the window is valid, if not then terminate GLFW
			and then the program itself.
		*/
		FileLogging::writeToTextFile("GLFW: The window has a value of null. Terminating GLFW\n");
		glfwTerminate();
		FileLogging::writeToTextFile("Terminating program\n");
		return -1;
	}
	
	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	FileLogging::writeToTextFile("GLFW: Setting glfwSpawInterval to 1 (vsync)\n");
	glfwSwapInterval(1);

	if (glewInit() != GLEW_OK) {
		/*
			checks if there is an error with inititalising GLEW, if there
			is, terminate the program.
		*/
		std::cout << "GLEW is not OK\n";
		FileLogging::writeToTextFile("GLEW: ERROR, GLEW IS NOT OK!\n");
	}

	std::cout << glGetString(GL_VERSION) << "\n";

	FileLogging::writeToTextFile("Game: Initialising vertex positions\n");
	{ 
		/*
			this scope is what allows the game to terminate when the window 
			is closed as the VAOs, VBOS etc are inside the scope and so are
			deleted when the control flow is outside of the scope.
		*/
		float positions[] = {
			/*
				The positions of each vertice that make up the shape. The 
				comments indicate which vertex corresponds to which number
				in the indices[].
			*/
			-1, -1, //0
			 1, -1, //1
			 1,  1, //2
			-1,  1  //3
		};

		FileLogging::writeToTextFile("Game: Initialising vertex indices\n");

		unsigned int indices[] = {
			/*
				Specifies to OpenGL which corners to render in what order,
				this allows us to have only 4 vertices for a square instead
				of 6 (3 per triangle) because 2 of the vertices are the same
				as 2 of the vertices on the other triangle and this is good
				for optimisation, but pretty much everyone does it (at least
				they should do, especially for big applications).
			*/

			0, 1, 2,
			2, 3, 0
		};

		unsigned int vao;
		GLCall(glGenVertexArrays(1, &vao));
		GLCall(glBindVertexArray(vao));
		FileLogging::writeToTextFile("GL: Successfully created VAO\n");

		/* Vertex Array */

		VertexArray va;
		/* Vertex Buffer (VBO) */
		VertexBuffer vb(positions, quad::tri_size * 2 * sizeof(float)); /* already bound */
		VertexBufferLayout vbl;
		vbl.Push<float>(2);
		va.AddBuffer(vb, vbl);

		/**** IBO (Index Buffer Object) ****/
		IndexBuffer ib(indices, quad::tri_indexb_size);

		/************************ SHADERS ************************/
		shaderProgram shader("Basic.shader");
		shader.BindShdr();

		/**************** uniforms ****************/
		
		shader.setUniform4f("u_Color", 0.1f, 0.5f, 0.9f, 1.0f);
		/*
			The values this is being set to does not matter
			so much as we later increment the uniforms 
			values
		*/

		/*
			Unbind shaders, buffers and VAOs.

			This done by binding 0 instead of the actual
			unsigned integer of the buffer/array etc.
		*/
		va.UnBindVA();
		FileLogging::writeToTextFile("GL: Successfully unbound VAO\n");
		shader.UnBindShdr();
		vb.unBindVB();
		FileLogging::writeToTextFile("GL: Successfully unbound VBO\n");
		ib.unBindIB();
		FileLogging::writeToTextFile("GL: Successfully unbound IBO\n");

		float r = 0.0f, b = 1.0f, g = 0.5f, a = 1.0f;

		float rIncrement = 0.05f, bIncrement = -0.05f, gIncrement = 0.03f, aIncrement = 0.04f;

		clock_t beginFrame, delta_ticks = 0, fps = 0;

		/* to be used in calculating average */
		double totalFPS = 0;
		double FPSCount = 0;

		FileLogging::writeToTextFile("Game: Entering main game loop\n");
		
		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			beginFrame = clock();

			GLCall(glClear(GL_COLOR_BUFFER_BIT)); /* clears screen */

			shader.BindShdr();
			shader.setUniform4f("u_Color", r, g, b, a);

			/* bind neccessary arrays and buffers */
			va.BindVA();
			ib.bindIB();

			GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

			/*
				increment the colours to be parsed into the u_Color 
				variable by the individual component's incrementer
			*/

			// RED
			if (r > 1.0f) { rIncrement = -0.05f; }		else if (r < 0.0f) { rIncrement = 0.05f; }
			r += rIncrement;
			
			// GREEN
			if (g > 1.0f) { gIncrement = -0.03f; }		else if (g < 0.0f) { gIncrement = 0.03f; }
			g += gIncrement;

			// BLUE
			if (b > 1.0f) { bIncrement = -0.05f; }		else if (b < 0.0f) { bIncrement = 0.05f; }
			b += bIncrement;

			// ALPHA (transparency)
			if (a > 1.0f) { aIncrement = -0.04f; }		else if (a < 0.0f) { aIncrement = 0.04f; }
			a += aIncrement;

			/* Swap front and back buffers */
			GLCall(glfwSwapBuffers(window));

			/* Poll for and process events */
			glfwPollEvents();


			/* GET FPS */
			delta_ticks = clock() - beginFrame; //the time, in ms, that took to render the scene
			fps = CLOCKS_PER_SEC / delta_ticks; 
			/*
				Formula for FPS = 1 / delta
				
				Where delta is the time taken to go through one iteration of 
				the game loop.
			*/
			std::cout << "FPS: " << fps << "\n";
			totalFPS += fps;
			FPSCount++;
		}
		std::cout << "Average FPS this play time: " << (totalFPS / FPSCount) << " \n";
		FileLogging::writeToTextFile("Game: Average FPS this play time: " + std::to_string(totalFPS / FPSCount) + "\n");
	}

	/* Terminate GLFW */
	glfwTerminate();
	FileLogging::writeToTextFile("GL: GLFW window terminated\n");

	FileLogging::writeToTextFile("Terminating program...\n");

	return 0;
}
