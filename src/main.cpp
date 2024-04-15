#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "shadeutil.h"
#include <ios>
#include <iostream>
#include <iomanip>
#include <signal.h>

void framebuffer_size_callback(GLFWwindow *, int, int);
void processInput(GLFWwindow *);


float vertices[12] = {
    -0.5f, -0.5f, 0.0f, // 0
    0.5f,  -0.5f, 0.0f, // 1
    0.5f,  0.5f,  0.0f, // 2
    -0.5f, 0.5f,  0.0f  // 3
};

unsigned int indices[6] = {
    0, 1, 2,  // Right Triangle
    2, 3, 0   // Left Triangle
};

int main(int argc, char *argv[]) {

  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow *window = glfwCreateWindow(800, 600, "Ignarus", nullptr, nullptr);
  if (window == NULL) {
    std::cout << "Failed to create GLFW window!" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);
  glfwSwapInterval(3);  // I run a 144hz display, sue me
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  ShaderProgramSource source = parseShader("res/shaders/hello_triangle.glsl");
  unsigned int shaderProgram = createShader(source.VertexSource, source.FragmentSource);
  

  unsigned int VBO, VAO;
  GLCALL(glGenVertexArrays(1, &VAO));
  GLCALL(glGenBuffers(1, &VBO));
  GLCALL(glBindVertexArray(VAO));
  GLCALL(glBindBuffer(GL_ARRAY_BUFFER, VBO));
  GLCALL(glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * 4, vertices, GL_STATIC_DRAW));

  GLCALL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void *)0));
  GLCALL(glEnableVertexAttribArray(0));

  unsigned int IBO;
  GLCALL(glGenBuffers(1, &IBO));
  GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO));
  GLCALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * 3 * 2, indices, GL_STATIC_DRAW));

  GLCALL(glBindVertexArray(0));



  GLCALL(int location = glGetUniformLocation(shaderProgram, "u_Color"));
  ASSERT(location != -1);
  //GLCALL(glUniform4f(location, 0.2f, 0.3f, 0.8f, 1.0f));

  GLCALL(glUseProgram(shaderProgram));

  float r = 0.0f;
  float increment = 0.5f;

  while (!glfwWindowShouldClose(window)) {

    processInput(window);

    GLCALL(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));
    GLCALL(glClear(GL_COLOR_BUFFER_BIT));

    GLCALL(glBindVertexArray(VAO));
    
  
    GLCALL(glUniform4f(location, r, 0.3f, 0.8f, 1.0f));
    GLCALL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
    
   if (r > 1.0f) {
      increment = -0.05f;
    }
    else if (r < 0.0f) {
      increment = 0.05f;
    }
    r += increment;
    
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  GLCALL(glDeleteProgram(shaderProgram));
  glfwTerminate();

  return 0;
}

void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  GLCALL(glViewport(0, 0, height, width));
}

