#pragma once
#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include <stdexcept>
#include <vector>
#include <optional>

struct QueueFamilyIndices
{
	std::optional<uint32_t> graphicsFamily;
	bool isComplete() {
		return graphicsFamily.has_value();
	}
};

class Instance
{
public:

	void initVulkan();
	void cleanup();
	void extensionSupport();

private:
	void createInstance();
	bool checkValidationLayerSupport();
	void pickPhysicalDevice();
	bool isDeviceSuitable(VkPhysicalDevice device);
	QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
	void createLogicalDevice();
private:
	VkInstance instance;
	VkDevice device;
	VkQueue graphicsQueue;
	VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;

	const std::vector<const char*> validationLayers = {
	"VK_LAYER_KHRONOS_validation"
	};

#ifdef NDEBUG
	const bool enableValidationLayers = false;
#else
	const bool enableValidationLayers = true;
#endif
};

