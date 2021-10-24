#include "vulkan/vulkan.hpp"
#include "vkutils.hpp"


class Device {
    public:
    void initVulkan();
    void getLayers();
    void printLayers();
    void getExtensions();
    void printExtensions();
    void getDevices();
    void printDevices();
    private:
    void createInstance();
    void setInstanceCreateInfo();
    void createDevice();
    uint32_t layerCount = 0;
    uint32_t extensionsCount = 0;
    uint32_t physDeviceCount = 0;
    VkLayerProperties *layers = NULL;
    VkExtensionProperties *extensions = NULL;
    std::vector<VkPhysicalDevice> physDevices = {};
    VkInstance instance;
    VkApplicationInfo appInfo;
    VkInstanceCreateInfo createInfo;
    std::vector<const char*> enabledDeviceExtensions;
};