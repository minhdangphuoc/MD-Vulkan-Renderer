#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
#include <stdexcept>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <thread>
#include <vector>
#include <chrono>

#include "simpleble/SimpleBLE.h"

#include "VulkanRenderer.hpp"

std::vector<SimpleBLE::Peripheral> peripherals;
std::vector<std::pair<SimpleBLE::BluetoothUUID, SimpleBLE::BluetoothUUID>> uuids;


GLFWwindow* window;
VulkanRenderer vulkanRenderer;

int seletedPeripheral = -1, seletedUUID = -1;

std::string hexToString(SimpleBLE::ByteArray bytes) {
        std::string a, b;
        std::stringstream ss;
        
        for (auto byte : bytes) {
            ss << std::hex << std::setfill('0') << std::setw(2) << (uint32_t)((uint8_t)byte) << " ";
            ss >> a;
            b = b + a;
        }
        return b;
}

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

void initBle()
{
	auto adapter_list = SimpleBLE::Adapter::get_adapters();

    if (adapter_list.size() == 0) {
        std::cout << "No adapter was found." << std::endl;
    }

    std::cout << "Available adapters: \n";
    int i = 0;
    for (auto& adapter : adapter_list) {
        std::cout << "[" << i++ << "] " << adapter.identifier() << " [" << adapter.address() << "]" << std::endl;
    }

    int adapter_selection = -1;
    while(adapter_selection < 0 || adapter_selection > adapter_list.size() - 1) {
        std::cout << "Please select an adapter: ";
        std::cin >> adapter_selection;
    }

    SimpleBLE::Adapter& adapter = adapter_list[adapter_selection];

    adapter.set_callback_on_scan_start([]() { std::cout << "Scan started." << std::endl; });

    adapter.set_callback_on_scan_stop([]() { std::cout << "Scan stopped." << std::endl; });

    adapter.set_callback_on_scan_found([&](SimpleBLE::Peripheral peripheral) {
        std::cout << "Found device: " << peripheral.identifier() << " [" << peripheral.address() << "]" << std::endl;
        peripherals.push_back(peripheral);
    });

    // Scan for 5 seconds and return.
    adapter.scan_for(5000);

    std::cout << "The following devices were found:" << std::endl;
    for (size_t i = 0; i < peripherals.size(); i++) {
        std::cout << "[" << i << "] " << peripherals[i].identifier() << " [" << peripherals[i].address() << "]"
                  << std::endl;
    }

    int selection = -1;
    std::cout << "Please select a device to connect to: ";
    std::cin >> selection;

    if (selection >= 0 && selection < peripherals.size()) {
		seletedPeripheral = selection;
        auto peripheral = peripherals[selection];
        std::cout << "Connecting to " << peripheral.identifier() << " [" << peripheral.address() << "]" << std::endl;
        peripheral.connect();

        std::cout << "Successfully connected, printing services and characteristics.." << std::endl;

        // Store all service and characteristic uuids in a vector.
        for (auto service : peripheral.services()) {
            for (auto characteristic : service.characteristics) {
                uuids.push_back(std::make_pair(service.uuid, characteristic));
            }
        }

        std::cout << "The following services and characteristics were found:" << std::endl;
        for (size_t i = 0; i < uuids.size(); i++) {
            std::cout << "[" << i << "] " << uuids[i].first << " " << uuids[i].second << std::endl;
        }

        std::cout << "Please select a characteristic to read: ";
        std::cin >> selection;
        seletedUUID = selection;
	}
}

void startGetMessage()
{
    peripherals[seletedPeripheral].notify(
        uuids[seletedUUID].first, uuids[seletedUUID].second, [&](SimpleBLE::ByteArray bytes) {
            std::string commingStr = "";
            // std::cout << "Converted: ";
            for (int i = 0; i < hexToString(bytes).length(); i += 2) {
                std::string byte = hexToString(bytes).substr(i, 2);
                char chr = (char)(int)strtol(byte.c_str(), nullptr, 16);
                commingStr.push_back(chr);
                // std::cout << chr;
            }
            // std::cout << "\r\n" << commingStr << std::endl;
            vulkanRenderer.setAngle(
                std::stoi(commingStr.substr(0, commingStr.size() - commingStr.find('|')).c_str(), nullptr));
        });
            Sleep(10);
}

int main()
{
	initBle();
	// Create Window
	initWindow("Vulkan Application", 800, 600);

	// Create Vulkan Renderer instance
	if (vulkanRenderer.init(window) == EXIT_FAILURE)
	{
		return EXIT_FAILURE;
	}

	std::string message;

	// Loop until closed
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
        startGetMessage();
		vulkanRenderer.drawFrame();
	}

	vulkanRenderer.deviceWaitIdle();

	vulkanRenderer.cleanup();

    peripherals[seletedPeripheral].unsubscribe(uuids[seletedUUID].first, uuids[seletedUUID].second);
	peripherals[seletedPeripheral].disconnect();

	// Destroy GLFW window and stop GLFW
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}