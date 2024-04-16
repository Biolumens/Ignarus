#include "Renderer.h"

#include <iostream>
#include <iomanip>
#include <sstream>

void glClearError() {
    while (glGetError() != GL_NO_ERROR)
        ;
}

bool glLogCall(const char* function, const char* file, int line) {

    while (GLenum error = glGetError()) {

        std::stringstream hexstream;
        hexstream << "0x" << std::uppercase << std::setfill('0') << std::setw(4) << std::hex << error;
        std::cout << "[OpenGL Error] (" << hexstream.str() << "): " << function << " "
            << file << ":" << line << std::endl;
        return false;
    }

    return true;
}