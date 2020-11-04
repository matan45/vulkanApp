#pragma once
#include <vulkan/vulkan.h>
class VertexInput;

class Image
{
public:
	void createTextureImage(VkDevice device,VertexInput& input, VkPhysicalDevice physicalDevice, VkCommandPool commandPool, VkQueue graphicsQueue);
	void cleanup(VkDevice device);
	void createTextureImageView(VkDevice device);
	void createTextureSampler(VkDevice device);

	VkImageView textureImageView;
	VkSampler textureSampler;
private:
	void createImage(VkDevice device, uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory, VertexInput& input, VkPhysicalDevice physicalDevice);
	VkCommandBuffer beginSingleTimeCommands(VkDevice device, VkCommandPool commandPool);
	void endSingleTimeCommands(VkCommandBuffer commandBuffer, VkCommandPool commandPool, VkQueue graphicsQueue, VkDevice device);
	void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size, VkCommandPool commandPool, VkQueue graphicsQueue, VkDevice device);
	void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, VkCommandPool commandPool, VkQueue graphicsQueue, VkDevice device);
	void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height, VkCommandPool commandPool, VkQueue graphicsQueue, VkDevice device);
	VkImageView createImageView(VkDevice device,VkImage image, VkFormat format);
private:
	VkImage textureImage;
	VkDeviceMemory textureImageMemory;
	
};

