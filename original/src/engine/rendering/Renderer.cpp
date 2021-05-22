#include "Renderer.h"

#include <iostream>

#include "../utils/Logging.h"
#include <string>

void GLClearError() {
    /*
        I recommend to not call this at the end of every GL function call
        for release builds, only for debug builds
    */
    while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line) {

    while (GLenum error = glGetError()) {
        std::cout << "[GL ERROR] : { " << error << " } " << "in " << file << " at " << function << " on line " << line << "\n";
        FileLogging::writeToTextFile("GL: A [GL ERROR] : { " + std::to_string(error) +" } has occured in " + std::string(file) + "\n\t\t\t\t\t\t\t\t\t in method " + function +  + "\n\t\t\t\t\t\t\t\t\t at line " + std::to_string(line) + "\n");
        return false;
    }
    return true;
}