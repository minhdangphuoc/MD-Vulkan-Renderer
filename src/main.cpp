#if defined(_WIN64) || defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
#endif

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <stdexcept>
#include <vector>
#include <iostream>

#include "VulkanRenderer.hpp"

GLFWwindow* window;
VulkanRenderer vulkanRenderer;

void initWindow(std::string wName = "Vulkan Application", const int width = 800, const int height = 600)
{
	// Initialise GLFW
	glfwInit();

	// Set GLFW to NOT work with OpenGL
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	// glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	
	window = glfwCreateWindow(width, height, wName.c_str(), nullptr, nullptr);
	
	glfwSetFramebufferSizeCallback(window, vulkanRenderer.framebufferResizeCallback);
}



int main()
{
	// Create Window
	initWindow("Vulkan Application", 800, 600);

	// Create Vulkan Renderer instance
	if (vulkanRenderer.init(window) == EXIT_FAILURE)
	{
		return EXIT_FAILURE;
	}

	// Loop until closed
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		vulkanRenderer.drawFrame();
	}

	vulkanRenderer.deviceWaitIdle();

	vulkanRenderer.cleanup();

	// Destroy GLFW window and stop GLFW
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}