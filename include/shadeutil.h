#pragma once

#ifdef __linux__
#define ASSERT(x) if (!(x)) raise(SIGTRAP);
#elif _WIN32
#define ASSERT(x) if (!(x)) __debugbreak; 
#endif

#include <string>

#define GLCALL(x) glClearError();\
    x;\
    ASSERT(glLogCall(#x, __FILE__, __LINE__));

//void glClearError();
//bool glLogCall(const char* function, const char* file, int line);

struct ShaderProgramSource {

    std::string VertexSource;
    std::string FragmentSource;

};
ShaderProgramSource parseShader(const std::string & filepath);
unsigned int compileShader(const std::string & source, unsigned int type);
unsigned int createShader(const std::string & vertexShader, const std::string & fragmentShader);