#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include "Util.h"
#include <functional>
#include <GLFW/glfw3.h>

const int MAX_FRAMES_IN_FLIGHT = 2;

class Framebuffers
{
public:
	void createFramebuffers(const std::vector<VkImageView>& swapChainImageViews, VkDevice device, VkRenderPass renderPass, const VkExtent2D& swapChainExtent);
	void createCommandPool(VkDevice device,const QueueFamilyIndices& QueueFamilies);
	void createCommandBuffers(VkDevice device, VkRenderPass renderPass, const VkExtent2D& swapChainExtent, VkPipeline graphicsPipeline);
	void createSyncObjects(VkDevice device,const std::vector<VkImage>& swapChainImages);
	void draw(VkDevice device, VkSwapchainKHR swapChain, VkQueue graphicsQueue, VkQueue presentQueue, GLFWwindow* window, bool& framebufferResized,std::function<void(GLFWwindow*)>recreateSwapChain);
	void cleanup(VkDevice device);
	void cleanupSwapChain(VkDevice device);

private:
	std::vector<VkFramebuffer> swapChainFramebuffers;
	std::vector<VkCommandBuffer> commandBuffers;
	VkCommandPool commandPool;

	std::vector<VkSemaphore> imageAvailableSemaphores;
	std::vector<VkSemaphore> renderFinishedSemaphores;
	std::vector<VkFence> inFlightFences;
	std::vector<VkFence> imagesInFlight;

	size_t currentFrame = 0;

	friend class Instance;
};

