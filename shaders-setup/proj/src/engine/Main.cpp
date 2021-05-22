/****************C++****************/
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <ctime>
#include <chrono>
#include <Windows.h>

/****************OpenGL****************/
#include <GL/glew.h>
#include <GLFW/glfw3.h>

/****************SELF****************/
#include "utils/Logging.h"
#include "rendering/Renderer.h"
#include "shader/ShaderProgram.h"

int main(void)
{
    std::string startTime = FileLogging::getTime();
    FileLogging::clearLogFile();
    /********CREATE WINDOW********/

    GLFWwindow* window;
    /* Initialize the library */
    
    if (!glfwInit())
        return -1;
    else
        FileLogging::writeToTextFile("GLFW: Initialised GLFW library\n");
    /**** SETS UP OPENGL CORE PROFILE ****/
    FileLogging::writeToTextFile("GLFW: Setting version to 3.3 core\n");
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // version 3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // version 3.3
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // set OpenGL mode to core
    // for compatibility mode, instead of GLFW_OPENGL_CORE_PROFILE, do GLFW_OPENGL_COMPAT_PROFILE

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(1280, 720, "C++ OpenGL Game", NULL, NULL);
    FileLogging::writeToTextFile("GLFW: GLFW window created: \n");

    if (!window)
    {
        FileLogging::writeToTextFile("GLFW: There was an error with the window. Terminating GLFW\n");
        glfwTerminate();
        return -1;
    }
    
    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    //std::string vyon;
    //std::cout << "would you like vsync?\n";
    //std::cin >> vyon;

    //if (vyon == "yes")
    //{
    FileLogging::writeToTextFile("GLFW: Setting glfwSpawInterval to 1 (vsync)\n");
        glfwSwapInterval(1); //vsync
    //}
    if (glewInit() != GLEW_OK) {
        std::cout << "GLEW is not OK\n";
        FileLogging::writeToTextFile("GLEW: ERROR, GLEW IS NOT OK!\n");
    }

    std::cout << glGetString(GL_VERSION) << "\n";
    
    const unsigned int tri_size = 4;
    const unsigned int tri_indexb_size = 6;

    //float positions[] = {
    //    -0.5f, -0.5f, //0
    //     0.5f, -0.5f, //1
    //     0.5f,  0.5f, //2
    //    -0.5f,  0.5f  //3
    //};

    FileLogging::writeToTextFile("Game: Initialising vertex positions\n");

    float positions[] = {
        -1, -1, //0
         1, -1, //1
         1,  1, //2
        -1,  1  //3
    };
    
    FileLogging::writeToTextFile("Game: Initialising vertex indices\n");

    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
    };

    unsigned int vao;
    GLCall(glGenVertexArrays(1, &vao));
    GLCall(glBindVertexArray(vao));
    FileLogging::writeToTextFile("GL: Successfully created VAO\n");

    /****VBO****/
    unsigned int buffer;
    GLCall(glGenBuffers(1, &buffer));
    //now need to select buffer by binding
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, buffer));
    FileLogging::writeToTextFile("GL: Successfully binded vertex buffers for first time\n");
    GLCall(glBufferData(GL_ARRAY_BUFFER, tri_size * 2 * sizeof(float), positions, GL_STATIC_DRAW));

    GLCall(glEnableVertexAttribArray(0));
    GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0));

    unsigned int ibo; //Index Buffer object
    GLCall(glGenBuffers(1, &ibo));
    //now need to select buffer by binding
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
    FileLogging::writeToTextFile("GL: Successfully binded index buffers for first time\n");
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, tri_indexb_size * sizeof(unsigned int), indices, GL_STATIC_DRAW));

    FileLogging::writeToTextFile("Game: Parsing shader    File: res/shaders/Basic.shader\n");
    shaderProgram::ShaderProgramSrc src = shaderProgram::parseShader("res/shaders/Basic.shader");
    /*std::cout << "VERTEX\n" << src.VertexSource << "\n";
    std::cout << "FRAGMENT\n" << src.FragmentSource << "\n";*/

    unsigned int shader = shaderProgram::CreateShader(src.VertexSource, src.FragmentSource);
    FileLogging::writeToTextFile("GL: Shaders created successfully\n");
    GLCall(glUseProgram(shader));
    FileLogging::writeToTextFile("GL: Successfully binded shaders\n");


    /****************uniforms****************/

    int location = glGetUniformLocation(shader, "u_Color");
    FileLogging::writeToTextFile("GL: Accessed uniform variable u_Color\n");
    ASSERT(location != 1);
    GLCall(glUniform4f(location, 0.2f, 0.3f, 0.0f, 1.0f));

    /* unbind shaders, buffers and VAOs */
    GLCall(glBindVertexArray(0));
    FileLogging::writeToTextFile("GL: Successfully unbound VAO\n");
    GLCall(glUseProgram(0));
    FileLogging::writeToTextFile("GL: Successfully unbound shader\n");
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
    FileLogging::writeToTextFile("GL: Successfully unbound array buffer\n");
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
    FileLogging::writeToTextFile("GL: Successfully unbound element array buffer\n");

    float r = 0.0f;
    float b = 1.0f;
    float g = 0.5f;
    float a = 1.0f;
    
    float rIncrement = 0.05f;
    float bIncrement = -0.05f;
    float gIncrement = 0.03f;
    float aIncrement = 0.04f;

    clock_t beginFrame;
    clock_t delta_ticks = 0.0;
    clock_t fps = 0;
    double totalFPS = 0;
    double FPSCount = 0;

    FileLogging::writeToTextFile("Game: Entering main game loop\n");

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        beginFrame = clock();

        /* Render here */
        GLCall(glClear(GL_COLOR_BUFFER_BIT));

        GLCall(glUseProgram(shader));
        GLCall(glUniform4f(location, r, b, g, a));

        GLCall(glBindVertexArray(vao));
        GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));

        GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

        if (r > 1.0f) {
            rIncrement = -0.05f;
        }
        else if (r < 0.0f) {
            rIncrement = 0.05f;
        }

        if (b > 1.0f) {
            bIncrement = -0.05f;
        }
        else if (b < 0.0f) {
            bIncrement = 0.05f;
        }

        if (g > 1.0f) {
            gIncrement = -0.03f;
        }
        else if (g < 0.0f) {
            gIncrement = 0.03f;
        }
        if (a > 1.0f) {
            aIncrement = -0.04f;
        }
        else if (a < 0.0f) {
            aIncrement = 0.04f;
        }


        r += rIncrement;
        b += bIncrement;
        g += gIncrement;
        a += aIncrement;

        /* Swap front and back buffers */
        GLCall(glfwSwapBuffers(window));

        /* Poll for and process events */
        glfwPollEvents();


        /*GET FPS*/
        delta_ticks = clock() - beginFrame; //the time, in ms, that took to render the scene
        fps = CLOCKS_PER_SEC / delta_ticks;
        if (fps >= 1000)
        {
            totalFPS += fps;
            FPSCount++;
        }
        else {
            std::cout << "FPS: " << fps << "\n";
            totalFPS += fps;
            FPSCount++;
        }
    }

    //delete the shader program
    GLCall(glDeleteProgram(shader));
    FileLogging::writeToTextFile("GL: Shader program deleted\n");

    glfwTerminate();
    FileLogging::writeToTextFile("GL: GLFW window terminated\n");
    std::string endTime = FileLogging::getTime();

    std::cout << "Average FPS this play time: " << (totalFPS / FPSCount) << " \n";
    FileLogging::writeToTextFile("Game: Average FPS this play time: " + std::to_string(totalFPS / FPSCount) + "\n");
    FileLogging::writeToTextFile("Terminating program\n");

    //FileLogging::writePlayTime(startTime, endTime);

    return 0;
}

// 5:31