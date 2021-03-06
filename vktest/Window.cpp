#include "Window.h"
#include <iostream>

Window::Window(const uint32_t WIDTH, const uint32_t HEIGHT, std::string&& title)
{
	if (glfwInit() == GLFW_FALSE) {
		 std::cerr<<"Failed to init glfw"<<std::endl;
	}
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	window = glfwCreateWindow(WIDTH, HEIGHT, title.c_str(),nullptr,nullptr);
	glfwSetWindowUserPointer(window, this);
	glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
}

void Window::run()
{
	instance.initVulkan(window);
	//instance.extensionSupport();
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		instance.drawFrame();
	}
	clenaup();
}

void Window::clenaup()
{
	instance.cleanup();
	glfwDestroyWindow(window);
	glfwTerminate();
}

void Window::framebufferResizeCallback(GLFWwindow* window, int width, int height)
{
	auto app = reinterpret_cast<Instance*>(glfwGetWindowUserPointer(window));
	app->framebufferResized = true;
}

Window::~Window()
{
}
