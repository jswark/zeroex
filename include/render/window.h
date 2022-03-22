#pragma once

#include <iostream>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "render/eventsHandler.h"
#include "render/graphicObject.h"

class GLFWWindow;

class Window : public EventsHandler, public GraphicObject
{
public:
	Window(const std::string& title, uint32_t width, uint32_t height);
	~Window();

	bool init() override;
	void render() override;
	void update() override;

	void clear(float x, float y, float z);
	bool closed();

	GLFWwindow* window;

private:
	uint32_t width, height;
	const std::string title;
};
