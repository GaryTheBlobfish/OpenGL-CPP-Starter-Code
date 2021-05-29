#pragma once
#include <string>
#include <unordered_map>

class shaderProgram {
private:
	std::string shdrFilePath;
	unsigned int shdrID;
	std::unordered_map<std::string, int> u_locationCache;
public:

	shaderProgram(std::string filename);
	~shaderProgram();

	void BindShdr() const;
	void UnBindShdr() const;

	struct ShaderProgramSrc {
		std::string VertexSource;
		std::string FragmentSource;
	};


	/************ UNFORMS ************/
	void setUniform4f(const std::string& name, float v0, float v1, float v2, float v3);

private:
	unsigned int getUniformLocation(const std::string& name);
	
	/**************** SHADERS ****************/
	static unsigned int CompileShader(unsigned int type, const std::string& source);
	static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
	static ShaderProgramSrc parseShader(const std::string& filepath);
};