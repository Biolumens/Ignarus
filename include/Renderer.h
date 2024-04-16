#pragma once

#include "GLAD/glad.h"

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