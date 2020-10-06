#include "Window.h"

Window::Window(const uint32_t WIDTH, const uint32_t HEIGHT, std::string&& title)
{
	if (glfwInit() == GLFW_FALSE) {
		throw std::runtime_error("Failed to init glfw");
	}
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	window = glfwCreateWindow(WIDTH, HEIGHT, title.c_str(),nullptr,nullptr);
}

void Window::run()
{
	instance.initVulkan();
	//instance.extensionSupport();
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
	}
	clenaup();
}

void Window::clenaup()
{
	glfwDestroyWindow(window);
	glfwTerminate();

	instance.cleanup();
}
