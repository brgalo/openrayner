#pragma once

#include "vulkan/vulkan.hpp"
#include <cstdint>
#include <vulkan/vulkan_core.h>

class Application {
    public:
    void initInstance();
    void setupDevices();
    private:
    VkInstance instance = {};
    struct {
        uint32_t layerCount = 0;
        VkLayerProperties *layerProperties = NULL;
        std::vector<VkLayerProperties> availableLayers;
        const std::vector<const char*> requestedLayers = {"VK_LAYER_KHRONOS_VALIDATION"};
    } instanceInfo;
    VkApplicationInfo applicationInfo = {};
    VkInstanceCreateInfo instanceCreateInfo = {};
    struct {
        VkPhysicalDevice *device = VK_NULL_HANDLE;
        uint32_t deviceCount = 0;
        std::vector<VkPhysicalDevice> listOfDevices;
    } phyDevices;
    struct {
        uint32_t graphicsQueueIndex = -1;
        uint32_t computeQueueIndex = -1;
        uint32_t queueFamilyCount = 0;
    } queues;
};