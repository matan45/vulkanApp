#pragma once
#include <glm/glm.hpp>
#include <array>
#include <vector>
#include <vulkan/vulkan.h>

struct Vertex {
	glm::vec2 pos;
	glm::vec3 color;
};

class VertexInput
{
public:

	static VkVertexInputBindingDescription getBindingDescription();
	static std::array<VkVertexInputAttributeDescription, 2> getAttributeDescriptions();

	void createVertexBuffer(VkDevice& device, VkPhysicalDevice& physicalDevice, VkCommandPool commandPool, VkQueue& graphicsQueue);
	void createIndexBuffer(VkDevice& device, VkPhysicalDevice& physicalDevice, VkCommandPool commandPool, VkQueue& graphicsQueue);
	void cleanup(VkDevice device);

	VkBuffer vertexBuffer;
	VkBuffer indexBuffer;

	const std::vector<Vertex> vertices = {
	{{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
	{{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
	{{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
	{{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}}
	};

	const std::vector<uint16_t> indices = {
	0,1,2,2,3,0
	};

	void createBuffer(VkDeviceSize size, VkDevice device, VkPhysicalDevice physicalDevice, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
private:
	uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties, VkPhysicalDevice physicalDevice);
	void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size, VkDevice device, VkCommandPool commandPool, VkQueue graphicsQueue);
private:
	VkDeviceMemory indexBufferMemory;

	VkDeviceMemory vertexBufferMemory;
};

