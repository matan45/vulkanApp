#pragma once

#include <vulkan/vulkan.h>
#include <stdexcept>
#include <GLFW/glfw3.h>
#include <vector>

struct SwapChainSupportDetails
{
	VkSurfaceCapabilitiesKHR capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;
};

class WindowSurface
{
public:
	void createSurface(VkInstance instance, GLFWwindow* window);
	void cleanup(VkInstance instance);
	bool checkDeviceExtensionSupport(VkPhysicalDevice device);

	SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);

	VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
	VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
	VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, GLFWwindow* window);
	void createImageView(VkDevice device, VkFormat swapChainImageFormat, const std::vector<VkImage>& swapChainImages);

	void cleanupSwapChain(VkDevice device);

	VkSurfaceKHR getSurface()const {
		return surface;
	}

	const std::vector<VkImageView>& getSwapChainImageViews()const {
		return swapChainImageViews;
	}

private:
	VkSurfaceKHR surface;
	const std::vector<const char*> deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

	std::vector<VkImageView>swapChainImageViews;
};

