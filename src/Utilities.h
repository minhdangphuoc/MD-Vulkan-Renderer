#pragma once
#include <fstream>
#include <glm/glm.hpp>
#include <array>
#include <iostream>
#include <sstream>
#include <cmath>
#include <vector>
#include <string>
#include <math.h>
#include <utility>  
#include <iomanip>
#include <chrono>
#include <thread>

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

class PushConstantData
{
    public:
    glm::mat2 transform{1.0f};
    glm::vec2 offset;
    alignas(16) glm::vec3 color;
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



class Vertex {
	public:
	glm::vec2 pos;
    alignas(16) glm::vec3 color;

    static VkVertexInputBindingDescription getBindingDescription() {
        VkVertexInputBindingDescription bindingDescription{};
		bindingDescription.binding = 0;
        bindingDescription.stride = sizeof(Vertex);
        bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

        return bindingDescription;
    }

	static std::array<VkVertexInputAttributeDescription, 2> getAttributeDescriptions() {
        std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions{};

        attributeDescriptions[0].binding = 0;
        attributeDescriptions[0].location = 0;
        attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
        attributeDescriptions[0].offset = offsetof(Vertex, pos);

        attributeDescriptions[1].binding = 0;
        attributeDescriptions[1].location = 1;
        attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[1].offset = offsetof(Vertex, color);

        return attributeDescriptions;
    }
};

class Transform2dComponent 
{
	public:
	glm::vec2 translation{};
    glm::vec2 scale{1.0f, 1.0f};
    float rotation;
    int angle = 0;

	glm::mat2 mat2() 
    {
        const float sin = glm::sin(rotation);
        const float cos = glm::cos(rotation);
        
        glm::mat2 rotMatrix{{cos, sin}, {-sin, cos}};

        glm::mat2 scaleMat {{scale.x, .0f}, {.0f, scale.y}};
        return scaleMat * rotMatrix;
    }
};

class GameObject 
{
    public: 
    using id_t = unsigned int;

    static GameObject createGameObject()
    {
        static id_t currentId = 0;
        return GameObject{currentId++};
    }


    GameObject(const GameObject &) = delete;
    GameObject &operator=(const GameObject &) = delete;
    GameObject(GameObject &&) = default;
    GameObject &operator=(GameObject &&) = default;

    // getter
    id_t getId() 
    {
        return id;
    }

    glm::vec3 color{};
	Transform2dComponent transform2d;

    private:
    id_t id;

    GameObject(id_t objId): id(objId)
    {

    }

};
