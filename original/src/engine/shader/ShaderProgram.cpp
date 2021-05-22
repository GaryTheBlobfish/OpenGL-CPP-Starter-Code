#include "ShaderProgram.h"

#include <GL/glew.h>

#include <iostream>
#include <sstream>

#include "../rendering/Renderer.h"
#include "../utils/Logging.h"

unsigned int shaderProgram::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    GLCall(glAttachShader(program, vs));
    GLCall(glAttachShader(program, fs));
    GLCall(glLinkProgram(program));
    GLCall(glValidateProgram(program));

    GLCall(glDeleteShader(vs));
    GLCall(glDeleteShader(fs));

    return program;
}

unsigned int shaderProgram::CompileShader(unsigned int type, const std::string& source)
{
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str(); //source needs to exist when this is called
    GLCall(glShaderSource(id, 1, &src, nullptr));
    GLCall(glCompileShader(id));

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result); //iv is types that it needs
    if (result == GL_FALSE)
    {
        std::cout << "GL " << (type == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT") << " SHADER COMPILE ERROR: \n";
        FileLogging::writeToTextFile("GL: SHADER COMPILATION ERROR");
        FileLogging::writeToTextFileNTime("            SHADER TYPE:");
        FileLogging::writeToTextFileNTime((type == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT"));
        int length;
        GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
        char* message = (char*)_malloca(length * sizeof(char));
        GLCall(glGetShaderInfoLog(id, length, &length, message));
        std::cout << message << "\n";
        GLCall(glDeleteShader(id));
        return 0;
    }
    return id;
}

shaderProgram::ShaderProgramSrc shaderProgram::parseShader(const std::string& filepath)
{
    std::ifstream stream(filepath);

    enum class ShaderType {
        NONE = -1,
        VERTEX = 0,
        FRAGMENT = 1,
        GEOMETRY = 2,
        TESSELATION = 3
    };

    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;

    while (getline(stream, line)) {
        // always make sure first line in shader file is the #shader type
        if (line.find("#shader") != std::string::npos) {
            std::cout << "SHADER PARSING:\t#shader found, verifying type\n";
            FileLogging::writeToTextFile("SHADER PARSING:    #shader found, verifying type\n");
            if (line.find("vertex") != std::string::npos) {
                std::cout << "SHADER PARSING:\tshader type: vertex shader\n";
                FileLogging::writeToTextFile("SHADER PARSING:    Shader type: Vertex shader\n");
                type = ShaderType::VERTEX;
            }
            else if (line.find("fragment") != std::string::npos) {
                std::cout << "SHADER PARSING:\tshader type: fragment shader\n";
                FileLogging::writeToTextFile("SHADER PARSING:    Shader type: Fragment shader\n");
                type = ShaderType::FRAGMENT;
            }
            else if (line.find("geometry") != std::string::npos) {
                std::cout << "SHADER PARSING:\tshader type: geometry shader\n";
                FileLogging::writeToTextFile("SHADER PARSING:    Shader type: Geometry shader\n");
                type = ShaderType::GEOMETRY;
            }
            else if (line.find("tesselation") != std::string::npos) {
                std::cout << "SHADER PARSING:\tshader type: tesselation shader\n";
                FileLogging::writeToTextFile("SHADER PARSING:    Shader type: Tesselation shader\n");
                type = ShaderType::TESSELATION;
            }
        }
        else {
            ss[(int)type] << line << "\n";
        }
    }
    return { ss[0].str(), ss[1].str() };
}

/****************UNIFORMS****************/

void shaderProgram::getUniformLocations(unsigned int shader)
{
    // OUTPUT COLOUR
    FileLogging::writeToTextFile("Game: Retreiving uniform locations\n");
    shaderProgram::u_Color_Location = glGetUniformLocation(shader, "u_Color");
    ASSERT(u_Color_Location != -1);
    FileLogging::writeToTextFile("Game: Successfully retreived uniform location for u_Color\n");
}

void shaderProgram::setUniforms(unsigned int shader)
{
    // set the values of the uniforms
    FileLogging::writeToTextFile("Game: Setting value of u_Color\n");
    GLCall(glUniform4f(shaderProgram::u_Color_Location, 0.2f, 0.3f, 0.8f, 1.0f));
}