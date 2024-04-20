#include "Shader.h"

#include<fstream>
#include<sstream>
#include<iostream>

Shader::Shader(const std::string& filepath)
	: m_FilePath(filepath), m_RendererID(0) {
	
    ShaderProgramSource source = ParseShader(filepath);
    m_RendererID = CreateShader(source.VertexSource, source.FragmentSource);
}
Shader::~Shader() {
    GLCALL(glDeleteProgram(m_RendererID));
}


ShaderProgramSource Shader::ParseShader(const std::string& filepath) {
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
            }
            else if (line.find("fragment") != std::string::npos) {
                type = ShaderType::FRAGMENT;
            }
        }
        else {
            ss[(int)type] << line << "\n";
        }
    }

    return { ss[0].str(), ss[1].str() };
}
// TODO: Copy from shadutil.cpp

unsigned int Shader::CompileShader(const std::string& source, unsigned int type) {

    unsigned int sp = glCreateShader(type);
    const char* c_src = source.c_str();
    GLCALL(glShaderSource(sp, 1, &c_src, nullptr));
    GLCALL(glCompileShader(sp));

    int result;
    GLCALL(glGetShaderiv(sp, GL_COMPILE_STATUS, &result));
    if (result == GL_FALSE) {

        int length;
        glGetShaderiv(sp, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)_malloca(length * sizeof(char));
        glGetShaderInfoLog(sp, length, &length, message);

        std::cout << "SHADER ERROR: Failed to compile " << type << "!" << std::endl;
        std::cout << message << std::endl;

        glDeleteShader(sp);
    }

    return sp;
}

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {

    unsigned int program = glCreateProgram();
    unsigned int vsp = CompileShader(vertexShader, GL_VERTEX_SHADER);
    unsigned int fsp = CompileShader(fragmentShader, GL_FRAGMENT_SHADER);

    GLCALL(glAttachShader(program, vsp));
    GLCALL(glAttachShader(program, fsp));
    GLCALL(glLinkProgram(program));
    GLCALL(glValidateProgram(program));

    glDeleteShader(vsp);
    glDeleteShader(fsp);

    return program;
}

void Shader::Bind() const {
    GLCALL(glUseProgram(m_RendererID));
}
void Shader::Unbind() const {
    GLCALL(glUseProgram(0));
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3) {
    GLCALL(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}


unsigned int Shader::GetUniformLocation(const std::string& name) {

    if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end()) {
        return m_UniformLocationCache[name];
    }

    GLCALL(int location = glGetUniformLocation(m_RendererID, name.c_str()));
    if (location == -1) {
        std::cout << "[WARNING]: Uniform '" << name << "' doesn't exist!" << std::endl;
    }

    m_UniformLocationCache[name] = location;
    return location;
}
