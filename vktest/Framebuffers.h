#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include "Util.h"

class Framebuffers
{
public:
	void createFramebuffers(const std::vector<VkImageView>& swapChainImageViews, VkDevice device, VkRenderPass renderPass, const VkExtent2D& swapChainExtent);
	void createCommandPool(VkDevice device,const QueueFamilyIndices& QueueFamilies);
	void createCommandBuffers(VkDevice device, VkRenderPass renderPass, const VkExtent2D& swapChainExtent, VkPipeline graphicsPipeline);
	void cleanup(VkDevice device);
private:
	std::vector<VkFramebuffer> swapChainFramebuffers;
	std::vector<VkCommandBuffer> commandBuffers;
	VkCommandPool commandPool;
};

