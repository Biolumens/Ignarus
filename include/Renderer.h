#pragma once

#include "GLAD/glad.h"

#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Shader.h"

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

class Renderer{
public:
    void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader);
    void Clear();
    void Clear(float v0, float v1, float v2, float v3); // TODO: Make implementation
};