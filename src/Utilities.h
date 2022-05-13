#pragma once
#include <fstream>

// Indices (locations) of QUeue Families (if they exist at all)
class QueueFamilyIndices 
{
	public:
		int graphicsFamily = -1;
		int presentFamily = -1;

		bool isValid()
		{
			return graphicsFamily >= 0 && presentFamily >= 0;
		}

};

class SwapChainSupportDetails {
	public:
		VkSurfaceCapabilitiesKHR capabilities;
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR> presentModes;
};

static std::vector<char> readFile(const std::string& filename) {
    std::ifstream file(filename, std::ios::ate | std::ios::binary);

    if (!file.is_open()) {
        throw std::runtime_error("failed to open file!");
    }

	size_t fileSize = (size_t) file.tellg();
	std::vector<char> buffer(fileSize);

	file.seekg(0);
	file.read(buffer.data(), fileSize);

	file.close();

	return buffer;
}