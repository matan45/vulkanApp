#pragma once
#include <vulkan/vulkan.h>
#include <string>
#include <fstream>
#include <vector>
#include "VertexInput.h"
class Discriptor;

class ShaderModules
{
public:
	void createRenderPass(VkDevice device, VkFormat swapChainImageFormat);
	void createGraphicsPipline(VkDevice device,const VkExtent2D& swapChainExtent, Discriptor& discriptor);
	void cleanup(VkDevice device);

	VkRenderPass getRenderPass()const {
		return renderPass;
	}

	VkPipeline getGraphicsPipeline()const {
		return graphicsPipeline;
	}

	VkPipelineLayout pipelineLayout;
private:
	VkShaderModule createShaderModule(const std::vector<char>& code, VkDevice device);
	static std::vector<char> readFile(const std::string& filename);

private:
	
	VkRenderPass renderPass;
	VkPipeline graphicsPipeline;

};

