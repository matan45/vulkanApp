#include "Instance.h"
#include <iostream>
#include <set>

void Instance::initVulkan(GLFWwindow* window)
{
	inwindow = window;
	try {
		createInstance();
		windowSurface.createSurface(instance, window);
		pickPhysicalDevice();
		createLogicalDevice();
		createSwapChain(window);
		windowSurface.createImageView(device, swapChainImageFormat, swapChainImages);
		shaderModules.createRenderPass(device, swapChainImageFormat);
		discriptor.createDescriptorSetLayout(device);
		shaderModules.createGraphicsPipline(device, swapChainExtent, discriptor);
		framebuffers.createFramebuffers(windowSurface.getSwapChainImageViews(), device, shaderModules.getRenderPass(), swapChainExtent);
		framebuffers.createCommandPool(device, findQueueFamilies(physicalDevice));
		image.createTextureImage(device,vertexInput,physicalDevice,framebuffers.commandPool,graphicsQueue);
		vertexInput.createVertexBuffer(device, physicalDevice, framebuffers.commandPool, graphicsQueue);
		vertexInput.createIndexBuffer(device, physicalDevice, framebuffers.commandPool, graphicsQueue);
		discriptor.createUniformBuffers(swapChainImages, vertexInput, device, physicalDevice);
		discriptor.createDescriptorPool(device, swapChainImages);
		discriptor.createDescriptorSets(device, swapChainImages);
		framebuffers.createCommandBuffers(device, vertexInput, shaderModules.getRenderPass(), swapChainExtent, shaderModules.getGraphicsPipeline(),discriptor,shaderModules);
		framebuffers.createSyncObjects(device, swapChainImages);
	}
	catch (const std::runtime_error& e) {
		std::cerr << e.what() << std::endl;
	}

}

void Instance::cleanup()
{
	vkDeviceWaitIdle(device);
	cleanupSwapchain();

	image.cleanup(device);

	discriptor.cleanup(device);

	framebuffers.cleanup(device);

	windowSurface.cleanup(instance);
	vkDestroyDevice(device, nullptr);
	vkDestroyInstance(instance, nullptr);
}

void Instance::extensionSupport()
{
	uint32_t extensionCount = 0;
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
	std::vector<VkExtensionProperties>extensions(extensionCount);
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

	for (const auto& extension : extensions) {
		std::cout << '\t' << extension.extensionName << std::endl;
	}
}

void Instance::drawFrame()
{
	auto func = std::bind(&Instance::recreateSwapChain, this, inwindow);
	try {
		framebuffers.draw(device, swapChain, graphicsQueue, presentQueue, inwindow, discriptor, framebufferResized, func, swapChainExtent);
	}
	catch (const std::runtime_error& e) {
		std::cerr << e.what() << std::endl;
	}

}



void Instance::createInstance()
{
	if (enableValidationLayers && !checkValidationLayerSupport()) {
		throw std::runtime_error("Validation layers requested, but not available!");
	}

	VkApplicationInfo appInfo = {};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "test vk";
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.pEngineName = "no name";
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.apiVersion = VK_API_VERSION_1_2;

	VkInstanceCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;

	uint32_t glfwExtensionCout = 0;
	const char** glfwExtensions;

	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCout);

	if (glfwExtensionCout == 0) {
		throw std::runtime_error("NO GLFW Extensions");
	}

	createInfo.enabledExtensionCount = glfwExtensionCout;
	createInfo.ppEnabledExtensionNames = glfwExtensions;

	if (enableValidationLayers) {
		createInfo.enabledLayerCount = static_cast<uint32_t>(this->validationLayers.size());
		createInfo.ppEnabledLayerNames = this->validationLayers.data();
	}
	else {
		createInfo.enabledLayerCount = 0;
		createInfo.ppEnabledLayerNames = nullptr;
	}


	if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
		throw std::runtime_error("failed to create instance");
	}
}

bool Instance::checkValidationLayerSupport()
{
	uint32_t layerCount;
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

	std::vector<VkLayerProperties> availableLayers(layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

	for (const char* layerName : this->validationLayers) {
		bool layerFound = false;

		for (const auto& layerProperties : availableLayers) {
			if (strcmp(layerName, layerProperties.layerName) == 0) {
				layerFound = true;
				break;
			}
		}

		if (!layerFound) {
			return false;
		}
	}

	return true;
}

void Instance::pickPhysicalDevice()
{
	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
	if (deviceCount == 0) {
		throw std::runtime_error("failed to find GPUs with vulkan support");
	}
	std::vector<VkPhysicalDevice> devices(deviceCount);
	vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

	for (const auto& device : devices) {
		if (isDeviceSuitable(device)) {
			physicalDevice = device;
			break;
		}
	}

	if (physicalDevice == VK_NULL_HANDLE) {
		throw std::runtime_error("failed to find a suitable GPU!");
	}
}

void Instance::createSwapChain(GLFWwindow* window)
{
	SwapChainSupportDetails swapChainSupport = windowSurface.querySwapChainSupport(physicalDevice);

	VkSurfaceFormatKHR surfaceFormat = windowSurface.chooseSwapSurfaceFormat(swapChainSupport.formats);
	VkPresentModeKHR presentMode = windowSurface.chooseSwapPresentMode(swapChainSupport.presentModes);
	VkExtent2D extent = windowSurface.chooseSwapExtent(swapChainSupport.capabilities, window);

	uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
	if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) {
		imageCount = swapChainSupport.capabilities.maxImageCount;
	}

	VkSwapchainCreateInfoKHR createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	createInfo.surface = windowSurface.getSurface();

	createInfo.minImageCount = imageCount;
	createInfo.imageFormat = surfaceFormat.format;
	createInfo.imageColorSpace = surfaceFormat.colorSpace;
	createInfo.imageExtent = extent;
	createInfo.imageArrayLayers = 1;
	createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

	QueueFamilyIndices indices = findQueueFamilies(physicalDevice);
	uint32_t queueFamilyIndices[] = { indices.graphicsFamily.value(), indices.presentFamily.value() };

	if (indices.graphicsFamily != indices.presentFamily) {
		createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		createInfo.queueFamilyIndexCount = 2;
		createInfo.pQueueFamilyIndices = queueFamilyIndices;
	}
	else {
		createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
	}

	createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
	createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	createInfo.presentMode = presentMode;
	createInfo.clipped = VK_TRUE;

	createInfo.oldSwapchain = VK_NULL_HANDLE;

	if (vkCreateSwapchainKHR(device, &createInfo, nullptr, &swapChain) != VK_SUCCESS) {
		throw std::runtime_error("failed to create swap chain!");
	}

	vkGetSwapchainImagesKHR(device, swapChain, &imageCount, nullptr);
	swapChainImages.resize(imageCount);
	vkGetSwapchainImagesKHR(device, swapChain, &imageCount, swapChainImages.data());

	swapChainImageFormat = surfaceFormat.format;
	swapChainExtent = extent;
}

bool Instance::isDeviceSuitable(const VkPhysicalDevice device)
{
	QueueFamilyIndices indices = findQueueFamilies(device);

	bool extensionsSupported = windowSurface.checkDeviceExtensionSupport(device);
	bool swapChainAdequate = false;
	if (extensionsSupported) {
		SwapChainSupportDetails swapChainSupport = windowSurface.querySwapChainSupport(device);
		swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
	}

	return indices.isComplete() && extensionsSupported && swapChainAdequate;
}

QueueFamilyIndices Instance::findQueueFamilies(const VkPhysicalDevice device)
{
	QueueFamilyIndices indices;

	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

	int i = 0;
	for (const auto& queueFamily : queueFamilies) {
		if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
			indices.graphicsFamily = i;
		}
		VkBool32 presentSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(device, i, windowSurface.getSurface(), &presentSupport);
		if (queueFamily.queueCount > 0 && presentSupport) {
			indices.presentFamily = i;
		}
		if (indices.isComplete()) {
			break;
		}

		i++;
	}

	return indices;
}

void Instance::createLogicalDevice()
{
	QueueFamilyIndices indices = findQueueFamilies(physicalDevice);

	std::vector<VkDeviceQueueCreateInfo>queueCreateInfos;
	std::set<uint32_t>uniuqeQueueFamilies = { indices.graphicsFamily.value(),indices.presentFamily.value() };

	float queuePriority = 1.0f;
	for (uint32_t queueFamily : uniuqeQueueFamilies) {
		VkDeviceQueueCreateInfo queueCreateInfo = {};
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.queueFamilyIndex = queueFamily;
		queueCreateInfo.queueCount = 1;
		queueCreateInfo.pQueuePriorities = &queuePriority;
		queueCreateInfos.push_back(queueCreateInfo);
	}
	VkPhysicalDeviceFeatures deviceFeatures = {};

	VkDeviceCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
	createInfo.pQueueCreateInfos = queueCreateInfos.data();
	createInfo.pEnabledFeatures = &deviceFeatures;
	createInfo.enabledExtensionCount = static_cast<uint32_t>(Extensions.size());
	createInfo.ppEnabledExtensionNames = Extensions.data();

	if (enableValidationLayers) {
		createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
		createInfo.ppEnabledLayerNames = validationLayers.data();
	}
	else
	{
		createInfo.enabledLayerCount = 0;
	}

	if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &device) != VK_SUCCESS) {
		throw std::runtime_error("failed to create logical device");
	}

	vkGetDeviceQueue(device, indices.graphicsFamily.value(), 0, &graphicsQueue);
	vkGetDeviceQueue(device, indices.presentFamily.value(), 0, &presentQueue);
}

void Instance::recreateSwapChain(GLFWwindow* window)
{
	int width = 0, height = 0;
	glfwGetFramebufferSize(window, &width, &height);
	while (width == 0 || height == 0)
	{
		glfwGetFramebufferSize(window, &width, &height);
		glfwWaitEvents();
	}

	vkDeviceWaitIdle(device);

	cleanupSwapchain();

	createSwapChain(window);
	windowSurface.createImageView(device, swapChainImageFormat, swapChainImages);
	shaderModules.createRenderPass(device, swapChainImageFormat);
	shaderModules.createGraphicsPipline(device, swapChainExtent, discriptor);
	framebuffers.createFramebuffers(windowSurface.getSwapChainImageViews(), device, shaderModules.getRenderPass(), swapChainExtent);
	//framebuffers.createCommandPool(device, findQueueFamilies(physicalDevice));
	discriptor.createUniformBuffers(swapChainImages, vertexInput, device, physicalDevice);
	discriptor.createDescriptorPool(device, swapChainImages);
	discriptor.createDescriptorSets(device, swapChainImages);
	framebuffers.createCommandBuffers(device, vertexInput, shaderModules.getRenderPass(), swapChainExtent, shaderModules.getGraphicsPipeline(),discriptor, shaderModules);

}

void Instance::cleanupSwapchain()
{
	framebuffers.cleanupSwapChain(device);
	vertexInput.cleanup(device);
	shaderModules.cleanup(device);
	windowSurface.cleanupSwapChain(device);
	vkDestroySwapchainKHR(device, swapChain, nullptr);
	discriptor.cleanupuniformBuffers(device, swapChainImages);
}



