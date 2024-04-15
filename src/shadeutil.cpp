#include "glad/glad.h"
#include"shadeutil.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

#ifdef __linux__
#define ASSERT(x) if (!(x)) raise(SIGTRAP);
#elif _WIN32
#define ASSERT(x) if (!(x)) __debugbreak; 
#endif

#define GLCALL(x) glClearError();\
    x;\
    ASSERT(glLogCall(#x, __FILE__, __LINE__));

void glClearError();
bool glLogCall(const char* function, const char* file, int line);



ShaderProgramSource parseShader(const std::string& filepath) {
    std::ifstream stream(filepath);

    enum class ShaderType {
        NONE = -1,
        VERTEX = 0,
        FRAGMENT = 1
    };

    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;

   while (getline(stream, line)) {
        if (line.find("#shader") != std::string::npos) {
            if (line.find("vertex") != std::string::npos) {
                type = ShaderType::VERTEX;
            } else if (line.find("fragment") != std::string::npos) {
                type = ShaderType::FRAGMENT;
            }
        } else {
            ss[(int)type] << line << "\n";
        }
   }

    return { ss[0].str(), ss[1].str() };
}

unsigned int compileShader(const std::string& source, unsigned int type) {

    unsigned int sp = glCreateShader(type);
    const char* c_src = source.c_str();
    GLCALL(glShaderSource(sp, 1, &c_src, nullptr));
    GLCALL(glCompileShader(sp));

    int result;
    GLCALL(glGetShaderiv(sp, GL_COMPILE_STATUS, &result));
    if (result == GL_FALSE) {

        int length;
        glGetShaderiv(sp, GL_INFO_LOG_LENGTH, &length);
        char *message = (char *) _malloca(length * sizeof(char)); 
        glGetShaderInfoLog(sp, length, &length, message);
        
        std::cout << "SHADER ERROR: Failed to compile " << type << "!" << std::endl;
        std::cout << message << std::endl;

        glDeleteShader(sp);
    }

    return sp;
}

unsigned int createShader(const std::string& vertexShader, const std::string& fragmentShader) {

    unsigned int program = glCreateProgram();
    unsigned int vsp = compileShader(vertexShader, GL_VERTEX_SHADER);
    unsigned int fsp = compileShader(fragmentShader, GL_FRAGMENT_SHADER);

    GLCALL(glAttachShader(program, vsp));
    GLCALL(glAttachShader(program, fsp));
    GLCALL(glLinkProgram(program));
    GLCALL(glValidateProgram(program));

    glDeleteShader(vsp);
    glDeleteShader(fsp);

    return program;
}

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