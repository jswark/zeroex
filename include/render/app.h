#pragma once

#include "render/Render.h"
#include "render/window.h"
#include "camera/camera.h"

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "render/callBack.h"

class App
{
public:
	void run()
	{
		init();

		while (!window->closed())
		{
			gl::GLfloat currentFrame = glfwGetTime();
			deltaTime = currentFrame - lastFrame;
			lastFrame = currentFrame;

			// Clear the colorbuffer
			window->clear(0.2f, 0.3f, 0.3f);

			do_movement();
			glm::mat4 view = camera->getViewMatrix();

			// Draw rectangle
			r->view = view;
			r->render();

			window->render();
			window->update();
		}
	};

private:
	void init()
	{
		window = std::unique_ptr<Window>(new Window(name, WIDTH, HEIGHT));
		window->init();
		glfwSetKeyCallback(window->window, keyCallback);

		r = std::unique_ptr<Render>(new Render(vShaderPath, fShaderPath));
		r->init();

		camera = std::unique_ptr<Camera>(new Camera());
	};

	void do_movement()
	{
		// Camera controls
		gl::GLfloat cameraSpeed = 5.0f * deltaTime;
		if (keys[GLFW_KEY_W])
			camera->position += cameraSpeed * camera->front;
		if (keys[GLFW_KEY_S])
			camera->position -= cameraSpeed * camera->front;
		if (keys[GLFW_KEY_A])
			camera->position -= glm::normalize(glm::cross(camera->front, camera->up)) * cameraSpeed;
		if (keys[GLFW_KEY_D])
			camera->position += glm::normalize(glm::cross(camera->front, camera->up)) * cameraSpeed;
	}


	// Deltatime
	gl::GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
	gl::GLfloat lastFrame = 0.0f;  	// Time of last frame

	// Window dimensions
	const gl::GLuint WIDTH = 800, HEIGHT = 600;
	const std::string name = "Render";

	std::string vShaderPath = "shaders/rectangle.vs";
	std::string fShaderPath = "shaders/rectangle.frag";

	std::unique_ptr<Render> r;
	std::unique_ptr<Window> window;

	std::unique_ptr<Camera> camera;
};
