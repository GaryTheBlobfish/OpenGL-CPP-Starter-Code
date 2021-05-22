#pragma once
#include <string>

#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))


class shaderProgram {
public:
    int u_Color_Location;
    struct ShaderProgramSrc {
        std::string VertexSource;
        std::string FragmentSource;
    };
	static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
    static ShaderProgramSrc parseShader(const std::string& filepath);
    void getUniformLocations(unsigned int shader);
    void setUniforms(unsigned int shader);

private:
    static unsigned int CompileShader(unsigned int type, const std::string& source);
};