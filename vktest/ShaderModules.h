#pragma once
#include <vulkan/vulkan.h>
#include <string>
#include <fstream>
#include <vector>

class ShaderModules
{
public:
	void createGraphicsPipline(VkDevice device,const VkExtent2D& swapChainExtent);
	void cleanup(VkDevice device);
private:
	VkShaderModule createShaderModule(const std::vector<char>& code, VkDevice device);
	static std::vector<char> readFile(const std::string& filename);

private:
	VkPipelineLayout pipelineLayout;
};

