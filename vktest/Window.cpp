#include "Window.h"
#include <iostream>

Window::Window(const uint32_t WIDTH, const uint32_t HEIGHT, std::string&& title)
{
	if (glfwInit() == GLFW_FALSE) {
		 std::cerr<<"Failed to init glfw"<<std::endl;
	}
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	window = glfwCreateWindow(WIDTH, HEIGHT, title.c_str(),nullptr,nullptr);

}

void Window::run()
{
	instance.initVulkan(window);
	//instance.extensionSupport();
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
	}
	clenaup();
}

void Window::clenaup()
{
	instance.cleanup();
	glfwDestroyWindow(window);
	glfwTerminate();
}

Window::~Window()
{
}
