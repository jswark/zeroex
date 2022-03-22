#include "render/Render.h"

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

bool Render::init() {
	// Build and compile our shader program
	Shader<VERTEX> rectangleV(vShaderPath);
	Shader<FRAGMENT> rectangleF(fShaderPath);
	shaderProg = std::unique_ptr<ShaderProgram>(new ShaderProgram(std::move(rectangleV), std::move(rectangleF)));

	std::vector<glm::vec3> vertices = {
		// first triangle
		{0.5f,  0.5f, 0.0f},  // top right
		{0.5f, -0.5f, 0.0f},  // bottom right
		{-0.5f,  0.5f, 0.0f},  // top left 
		// second triangle
		{0.5f, -0.5f, 0.0f},  // bottom right
		{-0.5f, -0.5f, 0.0f},  // bottom left
		{-0.5f,  0.5f, 0.0f}   // top left
	};

	gl::glEnable(gl::GL_DEPTH_TEST);
	// bind Vertex Array Object
	gl::glGenVertexArrays(1, &containerVAO);
	gl::glGenBuffers(1, &VBO);

	gl::glBindVertexArray(containerVAO);

	gl::glBindBuffer(gl::GL_ARRAY_BUFFER, VBO);
	gl::glBufferData(gl::GL_ARRAY_BUFFER, sizeof(float) * vertices.size() * 3, vertices.data(), gl::GL_STATIC_DRAW);

	gl::glVertexAttribPointer(0, 3, gl::GL_FLOAT, gl::GL_FALSE, 3 * sizeof(float), (void*)0);
	gl::glEnableVertexAttribArray(0);

	return true;
}

void Render::render()
{
	gl::glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
gl:glClear(gl::GL_COLOR_BUFFER_BIT | gl::GL_DEPTH_BUFFER_BIT);

	shaderProg->Use();

	// Projection 
	glm::mat4 projection;
	projection = glm::perspective(45.0f, (gl::GLfloat)800 / (gl::GLfloat)600, 0.1f, 100.0f);

	// Get the uniform locations
	gl::GLint modelLoc = gl::glGetUniformLocation(shaderProg->getProgram(), "model");
	gl::GLint viewLoc = gl::glGetUniformLocation(shaderProg->getProgram(), "view");
	gl::GLint projLoc = gl::glGetUniformLocation(shaderProg->getProgram(), "projection");

	// Pass the matrices to the shader
	gl::glUniformMatrix4fv(viewLoc, 1, gl::GL_FALSE, glm::value_ptr(view));
	gl::glUniformMatrix4fv(projLoc, 1, gl::GL_FALSE, glm::value_ptr(projection));

	glm::vec3 pos = glm::vec3(0.f, 0.f, 0.f);
	gl::glBindVertexArray(containerVAO);

	// Calculate the model matrix for each object and pass it to shader before drawing
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, pos);

	gl::glUniformMatrix4fv(modelLoc, 1, gl::GL_FALSE, glm::value_ptr(model));
	gl::glDrawArrays(gl::GL_TRIANGLES, 0, 6);

	gl::glBindVertexArray(0);
}

void Render::cleanup() {
	// Terminate GLFW, clearing any resources allocated by GLFW.
	gl::glDeleteVertexArrays(1, &containerVAO);
	gl::glDeleteBuffers(1, &VBO);
}

void key_Callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}
