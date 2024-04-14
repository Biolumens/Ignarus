#include "glad/glad.h"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

struct ShaderProgramSource {

    std::string VertexSource;
    std::string FragmentSource;

};

static ShaderProgramSource parseShader(const std::string& filepath) {
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

static unsigned int compileShader(const std::string& source, unsigned int type) {

    unsigned int sp = glCreateShader(type);
    const char* c_src = source.c_str();
    glShaderSource(sp, 1, &c_src, nullptr);
    glCompileShader(sp);

    int result;
    glGetShaderiv(sp, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {

        int length;
        glGetShaderiv(sp, GL_INFO_LOG_LENGTH, &length);
        char *message = (char *) alloca(length * sizeof(char)); 
        glGetShaderInfoLog(sp, length, &length, message);
        
        std::cout << "SHADER ERROR: Failed to compile " << type << "!" << std::endl;
        std::cout << message << std::endl;

        glDeleteShader(sp);
    }

    return sp;
}

static unsigned int createShader(const std::string& vertexShader, const std::string& fragmentShader) {

    unsigned int program = glCreateProgram();
    unsigned int vsp = compileShader(vertexShader, GL_VERTEX_SHADER);
    unsigned int fsp = compileShader(fragmentShader, GL_FRAGMENT_SHADER);

    glAttachShader(program, vsp);
    glAttachShader(program, fsp);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vsp);
    glDeleteShader(fsp);

    return program;
}

