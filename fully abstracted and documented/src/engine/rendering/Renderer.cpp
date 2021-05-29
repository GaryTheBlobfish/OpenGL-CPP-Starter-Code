#include "Renderer.h"

#include <iostream>

#include "../utils/Logging.h"
#include <string>

void GLClearError() {
	/*
		It is recommended to not call this at the end of 
		every GL function call for release builds, only
		for debug builds.
	*/
	while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line) {
	/*
		Writes to the console, that there is an OpenGL error
		and the error code it corresponds to. This method will
		also write the file that the error was found in and the
		method, and the line where the error-causing method is.
	*/
	while (GLenum error = glGetError()) {
		if (error != 1280 && error != 1281 && error != 1282) { /* Because we know the causes of these error codes */
			std::cout << "[GL ERROR] : { " << error << " } " << "in " << file << " at " << function << " on line " << line << "\n";
			FileLogging::writeToTextFile("GL: A [GL ERROR] : { " + std::to_string(error) + " } has occured in " + std::string(file) + "\n\t\t\t\t\t\t\t\t\t in method " + function + +"\n\t\t\t\t\t\t\t\t\t at line " + std::to_string(line) + "\n");
			return false;
		}
		else {
			if (error == 1280) {
				std::cout << "[GL ERROR] : { " << error << " : Invalid Enum } " << "in " << file << " at " << function << " on line " << line << "\n";
				FileLogging::writeToTextFile("GL: A [GL ERROR] : { " + std::to_string(error) + " : Invalid Enum } has occured in " + std::string(file) + "\n\t\t\t\t\t\t\t\t\t in method " + function + +"\n\t\t\t\t\t\t\t\t\t at line " + std::to_string(line) + "\n");
				return false;
			}
			else if (error == 1281) {
				std::cout << "[GL ERROR] : { " << error << " : Invalid Value } " << "in " << file << " at " << function << " on line " << line << "\n";
				FileLogging::writeToTextFile("GL: A [GL ERROR] : { " + std::to_string(error) + " : Invalid Value } has occured in " + std::string(file) + "\n\t\t\t\t\t\t\t\t\t in method " + function + +"\n\t\t\t\t\t\t\t\t\t at line " + std::to_string(line) + "\n");
				return false;
			}
			else if (error == 1282) {
				std::cout << "[GL ERROR] : { " << error << " : Invalid Operation } " << "in " << file << " at " << function << " on line " << line << "\n";
				FileLogging::writeToTextFile("GL: A [GL ERROR] : { " + std::to_string(error) + " : Invalid Operation } has occured in " + std::string(file) + "\n\t\t\t\t\t\t\t\t\t in method " + function + +"\n\t\t\t\t\t\t\t\t\t at line " + std::to_string(line) + "\n");
				return false;
			}
		}
	}
	return true;
}