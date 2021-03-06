#pragma once

// ADD FOR FIXING WINDOW CPP max and min BUG WITH VULKAN 
#define NOMINMAX   
///////////////////////////////////////////////////// 

#if defined(_WIN64) || defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
#endif

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <chrono>

#include <stdexcept>
#include <vector>
#include <cstring>

#include <cstdint> 
#include <cstddef>
#include <limits> 
#include <algorithm>
#include <iostream>
#include <set>
#include <cstdlib>
#include <unordered_map>


#ifndef Utilities_h
#define Utilities_h

#include "Utilities.h"

#endif

const int MAX_FRAMES_IN_FLIGHT = 2;

class VulkanRenderer
{
public:
	VulkanRenderer();

	int init(GLFWwindow* newWindow);
	void cleanup();
	void drawFrame();
	// -- Waiting
	void deviceWaitIdle();
	// -- Setter Functions
	void setFramebufferResized(bool value) {this -> framebufferResized = value;}

	
	static void framebufferResizeCallback(GLFWwindow* window, int width, int height) {
		auto app = reinterpret_cast<VulkanRenderer*>(glfwGetWindowUserPointer(window));
		app->setFramebufferResized(true);
	}

	~VulkanRenderer();

private:
	GLFWwindow* window;
	
	// New validation layer	
	const std::vector<const char*> validationLayers = {
		"VK_LAYER_KHRONOS_validation"
	};

	const std::vector<const char*> deviceExtensions = {
    	VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};

	// std::vector<VkDynamicState> dynamicStates = {
	// 	VK_DYNAMIC_STATE_VIEWPORT,
	// 	VK_DYNAMIC_STATE_LINE_WIDTH
	// };

	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;


	#ifdef NDEBUG
		const bool enableValidationLayers = false;
	#else
		const bool enableValidationLayers = true;
	#endif

	// Vulkan Components
	VkInstance instance;
	struct {
		VkPhysicalDevice physicalDevice;
		VkDevice logicalDevice;
	} mainDevice;
	VkQueue graphicsQueue;
	VkQueue presentQueue;
	VkSurfaceKHR surface;

	VkSwapchainKHR swapChain;
    std::vector<VkImage> swapChainImages;
    VkFormat swapChainImageFormat;
    VkExtent2D swapChainExtent;
	std::vector<VkImageView> swapChainImageViews;

	// Pipeline
	VkShaderModule createShaderModule(const std::vector<char>& code);
	VkPipelineLayout pipelineLayout;
	VkPipeline graphicsPipeline;
	VkRenderPass renderPass;

	// Drawing
	std::vector<VkFramebuffer> swapChainFramebuffers;
	VkCommandPool commandPool;

	// Before: Frames in flight
	// VkCommandBuffer commandBuffer;
	// VkSemaphore imageAvailableSemaphore;
	// VkSemaphore renderFinishedSemaphore;
	// VkFence inFlightFence;

	// Frames in flight -> array of commandBuffers
	std::vector<VkCommandBuffer> commandBuffers;
	std::vector<VkSemaphore> imageAvailableSemaphores;
	std::vector<VkSemaphore> renderFinishedSemaphores;
	std::vector<VkFence> inFlightFences;
	bool framebufferResized = false;
	uint32_t currentFrame = 0;

	// Vertex Buffer
	VkBuffer vertexBuffer;
	VkDeviceMemory vertexBufferMemory;
	VkBuffer indexBuffer;
	VkDeviceMemory indexBufferMemory;

	// Uniform Buffer 
	VkDescriptorSetLayout descriptorSetLayout;

	std::vector<VkBuffer> uniformBuffers;
	std::vector<VkDeviceMemory> uniformBuffersMemory;

	VkDescriptorPool descriptorPool;
	std::vector<VkDescriptorSet> descriptorSets;

	// Texture Mapping
	VkImage textureImage;
	VkDeviceMemory textureImageMemory;
	VkImageView textureImageView;
	VkSampler textureSampler;
	
	// Depth image and view
	VkImage depthImage;
	VkDeviceMemory depthImageMemory;
	VkImageView depthImageView;


	// Vulkan Functions
	// - Create Functions
	void createInstance();
	void createLogicalDevice();
	void createSurface();	
	void createSwapChain();
	void createImageViews();
	void createRenderPass();
	void createGraphicsPipeline();
	void createFramebuffers();
	void createCommandPool();
	void createCommandBuffer();
	void createSyncObjects();
	void createVertexBuffer();
	void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
	void createIndexBuffer();
	void createDescriptorSetLayout();
	void createUniformBuffers();
	void createDescriptorPool();
	void createDescriptorSets();
	void createTextureImage();
	VkImageView createImageView (VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);
	void createImage (
		uint32_t width, 
		uint32_t height, 
		VkFormat format, 
		VkImageTiling tiling, 
		VkImageUsageFlags usage, 
		VkMemoryPropertyFlags properties, 
		VkImage& image, 
		VkDeviceMemory& imageMemory
	);
	void createTextureImageView();
	void createTextureSampler();
	void createDepthResources();


	// - Recreate Function
	void recreateSwapChain();

	// - Get Functions
	void getPhysicalDevice();
	QueueFamilyIndices getQueueFamilies(VkPhysicalDevice device);

	// - Support Functions
	// -- Checker Functions
	bool checkInstanceExtensionSupport(std::vector<const char*>* checkExtensions);
	bool checkDeviceSuitable(VkPhysicalDevice device);
	bool checkValidationLayerSupport();
	bool checkDeviceExtensionSupport(VkPhysicalDevice device);
	bool hasStencilComponent(VkFormat format);

	// -- Finder Functions
	SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device); // Get SC support query
	uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
	VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
	VkFormat findDepthFormat();
	// -- Choosing Functions
	VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
	VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
	VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

	// -- Copying Function
	void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
	void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);

	// -- Recording Funtion
	void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);
	
	// -- Cleanup SwapChain
	void cleanupSwapChain();

	// -- Update Funtions
	void updateUniformBuffer(uint32_t currentImage);

	// -- Transition Image
	void transitionImageLayout (
		VkImage image, 
		VkFormat format, 
		VkImageLayout oldLayout, 
		VkImageLayout newLayout
	);
	
	// -- Buffer Command
	// --- Begin
	VkCommandBuffer beginSingleTimeCommands();
	// --- End
	void endSingleTimeCommands(VkCommandBuffer commandBuffer);

	void loadModel();
};

