#pragma once
#include <cstdint>
#include <string>
#include <stdexcept>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "Instance.h"

class Window
{
public:
	Window(const uint32_t WIDTH = 600, const uint32_t HEIGHT = 600, std::string&& title = "test");
	void run();
private:
	void clenaup();

private:
	GLFWwindow* window;
	Instance instance;
};

