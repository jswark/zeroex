#include "render/window.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <glbinding/gl/gl.h>
#include <glbinding/binding.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    gl::glViewport(0, 0, width, height);
}

Window::~Window()
{
    if (window != nullptr) {
        glfwTerminate();
        glfwDestroyWindow(window);
    }
};

bool Window::closed()
{
    return glfwWindowShouldClose(window);
}

bool Window::init()
{
    if (!glfwInit())
    {
        std::cout << "ERROR::GLFW::INIT : GLFW failed to initialize" << std::endl;
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, 0);

    window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glfwMakeContextCurrent(window);

    glbinding::Binding::initialize();

    if (glfwInit() != true)
    {
        glfwTerminate();
        std::cout << "ERROR::GLEW::INIT : GLEW failed to initialize" << std::endl;
        return false;
    }
}

Window::Window(const std::string& title, uint32_t width, uint32_t height)
    : title(title),
    width(width),
    height(height) {}

void Window::clear(float x, float y, float z)
{
    gl::glClearColor(x, y, z, 1.0f);
    gl::glEnable(gl::GL_DEPTH_TEST);
    gl::glClear(gl::GL_COLOR_BUFFER_BIT | gl::GL_DEPTH_BUFFER_BIT);
}

void Window::render()
{
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    gl::glViewport(0, 0, width, height);
    glfwSwapBuffers(window);
}

void Window::update()
{
    glfwPollEvents();
}
