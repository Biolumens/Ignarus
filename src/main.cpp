#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "Shader.h"

#include <iostream>

void framebuffer_size_callback(GLFWwindow*, int, int);
void processInput(GLFWwindow*);

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

int main(int argc, char* argv[]) {

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "Ignarus", nullptr, nullptr);
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

	Renderer renderer;
	Shader shader("res/shaders/hello_triangle.glsl");
	shader.Bind();

	VertexArray *VAO = new VertexArray();
	VertexBuffer *VBO = new VertexBuffer(vertices, 3 * 4 * sizeof(float));
	VertexBufferLayout *layout = new VertexBufferLayout();
	layout->Push<float>(3);
	VAO->AddBuffer(*VBO, *layout);

	IndexBuffer IBO(indices, 6);

	VAO->Bind();
	IBO.Bind();
	
	float r = 0.0f;
	float increment = 0.5f;

	while (!glfwWindowShouldClose(window)) {

		processInput(window);

		renderer.Clear(0.2f, 0.3f, 0.3f, 1.0f);

		shader.SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);
		renderer.Draw(*VAO, IBO, shader);

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

	delete(VBO);
	delete(layout);
	delete(VAO);
	glfwTerminate();

	return 0;
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	GLCALL(glViewport(0, 0, height, width));
}

