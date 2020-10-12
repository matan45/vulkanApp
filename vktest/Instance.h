#pragma once
#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include <stdexcept>
#include <vector>

#include "WindowSurface.h"
#include "ShaderModules.h"
#include "Framebuffers.h"
#include "Util.h"


class Instance
{
public:

	void initVulkan(GLFWwindow* window);
	void cleanup();
	void extensionSupport();
	void drawFrame();
private:
	void createInstance();
	bool checkValidationLayerSupport();
	void pickPhysicalDevice();
	void createSwapChain();
	bool isDeviceSuitable(const VkPhysicalDevice device);
	QueueFamilyIndices findQueueFamilies(const VkPhysicalDevice device);
	void createLogicalDevice();
private:
	VkInstance instance;
	VkDevice device;
	VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;

	VkQueue presentQueue;
	VkQueue graphicsQueue;
	
	VkSwapchainKHR swapChain;
	std::vector<VkImage> swapChainImages;
	VkFormat swapChainImageFormat;
	VkExtent2D swapChainExtent;

	WindowSurface windowSurface;
	ShaderModules shaderModules;
	Framebuffers framebuffers;

	const std::vector<const char*> validationLayers = {
	"VK_LAYER_KHRONOS_validation"
	};
	const std::vector<const char*> Extensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
#ifdef NDEBUG
	const bool enableValidationLayers = false;
#else
	const bool enableValidationLayers = true;
#endif


};

