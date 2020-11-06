#pragma once
#include <vulkan/vulkan.h>
#include <vector>
class Image;
class VertexInput;

class DepthBuffering
{
public:
	void createDepthResources(VkPhysicalDevice physicalDevice, VkDevice device,Image& image, VkExtent2D swapChainExtent, VertexInput& input);
	VkFormat findDepthFormat(VkPhysicalDevice physicalDevice);
	VkImageView depthImageView;
	void cleanup(VkDevice device);
private:
	VkFormat findSupportedFormat(VkPhysicalDevice physicalDevice,const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
	VkImageView createImageView(VkDevice device,VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);
	bool hasStencilComponent(VkFormat format) {
		return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;
	}
private:
	VkImage depthImage;
	VkDeviceMemory depthImageMemory;
	
};

