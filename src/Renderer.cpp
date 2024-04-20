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

void Renderer::Clear() {
    GLCALL(glClear(GL_COLOR_BUFFER_BIT));
}
void Renderer::Clear(float v0, float v1, float v2, float v3)
{
    GLCALL(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));
    GLCALL(glClear(GL_COLOR_BUFFER_BIT));
}
void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) {
    
    shader.Bind();
    va.Bind();
    ib.Bind();
    GLCALL(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));

}
