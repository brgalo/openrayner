#include "vulkan/vulkan.hpp"
#include "vkutils.hpp"


class Device {
    public:
    void initVulkan();
    void getInstanceLayers();
    void printInstanceLayers();
    void getInstanceExtensions();
    void printInstanceExtensions();
    void getQueueFamilyProperties();
    void printQueueFamilyProperties();
    void getDevices();
    void printDevices();
    private:
    void createInstance();
    void setInstanceCreateInfo();
    void createDevice();
    void selectDevice();
    void createDeviceInfo();
    void createDeviceQueueInfo();
    void getDeviceQueueInfo();
    void createLogicalDevice();
    void getQueueInfo();
    void setDeviceFunctionPointers();

    uint32_t layerCount = 0;
    uint32_t extensionsCount = 0;
    uint32_t physDeviceCount = 0;
    uint32_t queueFamiliesCount = 0;
    VkLayerProperties *layers = nullptr;
    VkExtensionProperties *extensions = nullptr;
    std::vector<VkPhysicalDevice> physDevices = {};
    VkPhysicalDevice *selectedPhysicalDevice = nullptr;
    VkDevice logicalDevice;
    VkQueue *queue;
    VkQueueFamilyProperties *queueFamilyProperties = nullptr;
    std::vector<const char*> enabledDeviceExtensions = {};
    VkInstance instance;
    VkApplicationInfo appInfo;
    VkInstanceCreateInfo createInfo;
    VkDeviceCreateInfo deviceCreateInfo;
    VkDeviceQueueCreateInfo deviceQueueCreateInfo;

    // Function pointers for ray tracing related stuff
	PFN_vkGetBufferDeviceAddressKHR vkGetBufferDeviceAddressKHR;
	PFN_vkCreateAccelerationStructureKHR vkCreateAccelerationStructureKHR;
	PFN_vkDestroyAccelerationStructureKHR vkDestroyAccelerationStructureKHR;
	PFN_vkGetAccelerationStructureBuildSizesKHR vkGetAccelerationStructureBuildSizesKHR;
	PFN_vkGetAccelerationStructureDeviceAddressKHR vkGetAccelerationStructureDeviceAddressKHR;
	PFN_vkBuildAccelerationStructuresKHR vkBuildAccelerationStructuresKHR;
	PFN_vkCmdBuildAccelerationStructuresKHR vkCmdBuildAccelerationStructuresKHR;
	PFN_vkCmdTraceRaysKHR vkCmdTraceRaysKHR;
	PFN_vkGetRayTracingShaderGroupHandlesKHR vkGetRayTracingShaderGroupHandlesKHR;
	PFN_vkCreateRayTracingPipelinesKHR vkCreateRayTracingPipelinesKHR;
};