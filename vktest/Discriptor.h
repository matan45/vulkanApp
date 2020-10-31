#pragma once
#include <glm/glm.hpp>
#include <vulkan/vulkan.h>
#include <stdexcept>
#include <vector>
class VertexInput;


struct UniformBufferObject {
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 proj;
};

class Discriptor
{
public:
	void createDescriptorSetLayout(VkDevice& device);
	void cleanup(VkDevice device);
	void createUniformBuffers(std::vector<VkImage>& swapChainImages,VertexInput& vertexInput, VkDevice device, VkPhysicalDevice physicalDevice);
	void cleanupuniformBuffers(VkDevice device, std::vector<VkImage>& swapChainImages);
	void updateUniformBuffer(uint32_t currentImage, VkExtent2D &swapChainExtent, VkDevice& device);
	void createDescriptorPool(VkDevice device, std::vector<VkImage>& swapChainImages);
	void createDescriptorSets(VkDevice device, std::vector<VkImage>& swapChainImages);

	VkDescriptorSetLayout descriptorSetLayout;
	std::vector<VkDescriptorSet> descriptorSets;

private:

	VkDescriptorPool descriptorPool;
	

	std::vector<VkBuffer> uniformBuffers;
	std::vector<VkDeviceMemory> uniformBuffersMemory;

};

