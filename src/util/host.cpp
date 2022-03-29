
#include "host.hpp"
#include <cstdint>
#include <vector>
#include <iostream>
#include <vulkan/vulkan_core.h>





void application::init() {
    //populate app&instancecreate info
    applicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    applicationInfo.pNext = NULL;
    applicationInfo.pApplicationName = "openRayner";
    applicationInfo.applicationVersion = VK_MAKE_VERSION(0, 0, 1);
    applicationInfo.pEngineName = "G&G";
    applicationInfo.engineVersion = VK_MAKE_VERSION(0, 0, 1);
    applicationInfo.apiVersion = VK_API_VERSION_1_2; //TODO: see 1.3

    instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instanceCreateInfo.pNext = nullptr;
    instanceCreateInfo.flags = 0;
    instanceCreateInfo.pApplicationInfo = &applicationInfo;
    instanceCreateInfo.enabledLayerCount = 0; //TODO: change if want windows
    instanceCreateInfo.ppEnabledLayerNames = NULL;
    instanceCreateInfo.enabledExtensionCount = 0;
    instanceCreateInfo.ppEnabledExtensionNames = NULL;

    vkCreateInstance(&instanceCreateInfo, NULL, &instance);

    // get available extensions
    uint32_t layerCount = 0;
    VkLayerProperties *layerProperties = 0;
    //vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
    std::vector<VkLayerProperties> availableLayers(layerCount);
    //vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());
    for (const auto &layer : availableLayers) {
        std::cout << layer.layerName << "\n";
    }

/*

    /*/
}