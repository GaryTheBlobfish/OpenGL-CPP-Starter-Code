#include "ShaderProgram.h"

#include <GL/glew.h>

#include <iostream>
#include <sstream>

#include "../rendering/Renderer.h"
#include "../utils/Logging.h"

shaderProgram::shaderProgram(std::string filename)
	: shdrFilePath("res/shaders/"+filename),
	  shdrID(0)
{
	shaderProgram::ShaderProgramSrc src = parseShader("res/shaders/" + filename);
	FileLogging::writeToTextFile("Game: Parsed shader res/shaders/" + filename + "successfully\n");

	shdrID = CreateShader(src.VertexSource, src.FragmentSource);
	FileLogging::writeToTextFile("GL: Shaders created successfully\n");
	
	GLCall(glUseProgram(shdrID)); /* specifies to GL to use the above shader */
	FileLogging::writeToTextFile("GL: Successfully binded shader(s)\n");
	FileLogging::writeToTextFile("GL: Successfully created shader program\n");
}

shaderProgram::~shaderProgram()
{
	/*
		Deletes the shader program from memory.
		This is to be called at the end of the program,
		at the bottom of the control flow (after the
		main game loop).
	*/
	GLCall(glDeleteProgram(shdrID));
	FileLogging::writeToTextFile("GL: Successfully deleted shader program\n");
}

void shaderProgram::BindShdr() const
{
	GLCall(glUseProgram(shdrID));
}

void shaderProgram::UnBindShdr() const
{
	GLCall(glUseProgram(0));
}


	/**************** UNIFORMS ****************/

unsigned int shaderProgram::getUniformLocation(const std::string& name)
{
	/*
		This method retrieves the location of the specified
		uniform variable, from the shader that is parsed into
		the method when it is called.

		We say there is a UNIFORM LOCATION WARNING if the 
		location is -1.



		u_locationCache is an std::unordered_map,
		basically a hashmap in Java. It has 2 types,
		std::string and int.

		In this form of an std::unordered_map, the
		std::string corresponds to the integer. So
		if we were to retrieve a std::string from
		u_locationCache it would return the
		corresponding integer.

		In our case of u_locationCache, the std::string
		is the name of the uniform variable and it's
		corresponding integer is it's location.

		Not entirely sure if the cashing system works,
		If not I will update the repository.
	*/
	if (u_locationCache.find(name) != u_locationCache.end())
		return u_locationCache[name];

	GLCall(int location = glGetUniformLocation(shdrID, name.c_str()));

	if (location == -1) {
		std::cout << "GL: UNIFORM LOCATION WARNING: The uniform variable " << name << "'s location is -1 and so doesn't exist\n";
		FileLogging::writeToTextFile("GL: UNIFORM LOCATION WARNING: The uniform variable " + name + "'s location is -1 and so doesn't exist\n");
	}
	u_locationCache[name] = location;
	std::cout << "GL: Successfully retrieved the location of the uniform variable " << name << ", (location: " << location << ") for the first time, it is now stored in cache\n";
	FileLogging::writeToTextFile("GL: Successfully retrieved the location of the uniform variable " + name + ", (location: " + std::to_string(location) + ") for the first time, it is now stored in cache\n");

	//return location;
	return u_locationCache[name];
}
void shaderProgram::setUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
	/*
		Sets the value of the uniform variables using
		glUniform4f().

		glUniform4f() takes in the location of the variable
		along with the 4 values you want to set into the
		vec4 uniform variable.
	*/
	GLCall(glUniform4f(getUniformLocation(name), v0, v1, v2, v3));
}





	/******************** SHADER COMPILATION, PARSING AND CREATING ********************/

unsigned int shaderProgram::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	/*
		This method creates the shader program and then compiles
		the vertex and fragment shaders. It then attaches the 2
		compiled shaders onto the shader program. The program is
		then linked and validated and the compiled shaders are
		deleted from memory.
	*/
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
	/*
		The shader is created using the variable, type, the 
		shader is then compiled (see docs.gl/gl4/glCompileShader).

		If the compilation of the shader does not succeed, we print
		out which shader is responsible for the error.

		If the compilation does indeed succeed, we return the ID of
		the shader.
	*/
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
		FileLogging::writeToTextFileNTime("			SHADER TYPE:");
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
	/*
		This is the most complicated method in the ShaderProgram
		class.

		First we create a file input stream which takes in the
		file located at 'filepath'.
		
		We then get each line in the file. If the line has
		#shader in it, then we will find if the line also has
		vertex or fragment or geometry or tesselation. If it
		does, we set the shader type to either vertex, fragment,
		goemetry or tesselation depending on what is found in the
		line. All the lines after the #shader line, until the next
		#shader line are appended onto the vertex, fragment,
		geometry or tesselation source string depending on what
		the last #shader line was. We then return the data in the
		file split into it's different shaders.
	*/
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
			FileLogging::writeToTextFile("SHADER PARSING:	#shader found, verifying type\n");
			if (line.find("vertex") != std::string::npos) {
				std::cout << "SHADER PARSING:\tshader type: vertex shader\n";
				FileLogging::writeToTextFile("SHADER PARSING:	Shader type: Vertex shader\n");
				type = ShaderType::VERTEX;
			}
			else if (line.find("fragment") != std::string::npos) {
				std::cout << "SHADER PARSING:\tshader type: fragment shader\n";
				FileLogging::writeToTextFile("SHADER PARSING:	Shader type: Fragment shader\n");
				type = ShaderType::FRAGMENT;
			}
			else if (line.find("geometry") != std::string::npos) {
				std::cout << "SHADER PARSING:\tshader type: geometry shader\n";
				FileLogging::writeToTextFile("SHADER PARSING:	Shader type: Geometry shader\n");
				type = ShaderType::GEOMETRY;
			}
			else if (line.find("tesselation") != std::string::npos) {
				std::cout << "SHADER PARSING:\tshader type: tesselation shader\n";
				FileLogging::writeToTextFile("SHADER PARSING:	Shader type: Tesselation shader\n");
				type = ShaderType::TESSELATION;
			}
		}
		else {
			ss[(int)type] << line << "\n";
		}
	}
	return { ss[0].str(), ss[1].str() };
}
