#include "device.hpp"

void Device::initVulkan() {
    createInstance();
    createDevice();
}

void Device::createInstance() {
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "OpenRayner";
    appInfo.applicationVersion = VK_MAKE_VERSION(0,0,0);
    appInfo.pEngineName = "openRay";
    appInfo.engineVersion = VK_MAKE_VERSION(0,0,0);
    appInfo.apiVersion = VK_API_VERSION_1_2;
    
    setInstanceCreateInfo();

    VkResult result = vkCreateInstance(&createInfo, NULL, &instance);
    ASSERT_VULKAN(result);
}

void Device::createDevice() {
    getDevices();
}

void Device::setInstanceCreateInfo() {
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pNext = NULL;
    createInfo.flags = 0;
    createInfo.pApplicationInfo = &appInfo;

    // TODO: configure Layers
    createInfo.enabledLayerCount = 0;
    createInfo.ppEnabledLayerNames = NULL;

    createInfo.enabledExtensionCount = 0;
    createInfo.ppEnabledExtensionNames = NULL;
}

void Device::getLayers() {
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
    layers = new VkLayerProperties[layerCount];
    vkEnumerateInstanceLayerProperties(&layerCount, layers);
}

void Device::printLayers() {
    std::cout << "Instance Layers (total: " << layerCount << ")" << std::endl;
    for(size_t i = 0; i<layerCount; ++i) {
        std::cout << i << " " << layers[i].layerName << std::endl;
    }
}

void Device::getExtensions() {
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionsCount, NULL);
    extensions = new VkExtensionProperties[extensionsCount];
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionsCount, extensions);
}

void Device::printExtensions() {
    std::cout << "Instance Extenstions (total: " << extensionsCount << ")" << std::endl;
    for(size_t i = 0; i< extensionsCount; ++i) {
        std::cout << i << " " << extensions[i].extensionName << std::endl;
    }
}

void Device::getDevices() {
    vkEnumeratePhysicalDevices(instance, &physDeviceCount, NULL);
    physDevices.resize(physDeviceCount);
    vkEnumeratePhysicalDevices(instance, &physDeviceCount, physDevices.data());
}