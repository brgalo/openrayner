#ifndef HOST_HPP
#define HOST_HPP

#define TINYOBJLOADER_IMPLEMENTATION
#include "../../3rdParty/tinyOBJ/tiny_obj_loader.h"

#include "vulkan/vulkan.hpp"
#include <cstdint>
#include <vector>
#include <vulkan/vulkan_core.h>




class Application {
    public:
    void initInstance();
    void setupDevice();
    void createBuffers();
    void loadModel();
    private:
    std::string modelName = "../testGeometry/plane.obj";
    std::string mtlName = "../testGeometry/";
    tinyobj::ObjReader reader;
    tinyobj::ObjReaderConfig readerConfig;
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
        VkPhysicalDevice physDeviceHandle = VK_NULL_HANDLE;
        VkPhysicalDeviceFeatures physDeviceFeatures;
        struct {
            uint32_t queueFamilyCount = 0;
            std::vector<VkQueueFamilyProperties> props;
            const float priority = 1.0f;
            const uint32_t nQueueInfos = 2; // TODO: Magic number :(
            std::array<VkDeviceQueueCreateInfo,2> createInfo;
            uint32_t graphicsIndex = -1;
            uint32_t computeIndex = -1;
            VkQueue computeQ = 0;
            VkQueue graphicsQ = 0;
        } queues;
        VkPhysicalDeviceMemoryProperties memoryProperties;
        const std::vector<const char*> enabledExtensions = {
            "VK_KHR_ray_tracing_pipeline",
            "VK_KHR_acceleration_structure",
            "VK_KHR_spirv_1_4",
            "VK_KHR_shader_float_controls", // TODO: required?
            "VK_KHR_buffer_device_address",
            "VK_KHR_deferred_host_operations",
            "VK_KHR_pipeline_library"
        };
        struct {
            VkPhysicalDeviceBufferDeviceAddressFeatures bufferDevAdressFeatures = {};
            VkPhysicalDeviceRayTracingPipelineFeaturesKHR rtPipelineFeatures = {};
            VkPhysicalDeviceAccelerationStructureFeaturesKHR accStructFeatures = {};
            VkDeviceCreateInfo info = {};
        } createInfo;
        VkDevice logicalDevHandle = VK_NULL_HANDLE;
    } device;
    struct {
        uint32_t deviceCount = 0;
        std::vector<VkPhysicalDevice> listOfDevices;
        std::vector<VkPhysicalDeviceProperties> deviceProperties;
        VkPhysicalDeviceMemoryProperties memoryProperties;

    } phyDevices;
    struct {
        struct {
            VkCommandPoolCreateInfo info;
            VkCommandPool pool;
        } commandPool;
    } buffers;


};
#endif