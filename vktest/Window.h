#pragma once
#include <cstdint>
#include <string>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "Instance.h"

class Window
{
public:
	virtual ~Window();
	Window(const uint32_t WIDTH = 600, const uint32_t HEIGHT = 600, std::string&& title = "test");
	void run();

	GLFWwindow* getWindow()const {
		return window;
	}
private:
	void clenaup();
	static void framebufferResizeCallback(GLFWwindow* window, int width, int height);

private:
	GLFWwindow* window;
	Instance instance;
};

