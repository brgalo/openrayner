#pragma once
class Application;
#include "vulkan/vulkan.hpp"
#include <vulkan/vulkan_core.h>



class VulkanInstance {
  void initInstance();
  void setupDevice();

public:
  VulkanInstance() {
    this->initInstance();
    this->setupDevice();
  }
  VkPhysicalDeviceMemoryProperties getMemProps() const;
  int test;
  void createBuffers(Application &app);
  VkDevice logicalDevHandle = VK_NULL_HANDLE;
  VkCommandPool getCmdPool() const;
  VkQueue getGraphicsQ() const;
  VkQueue getComputeQ() const;
private:
  // current vulkan Instance
  VkInstance instance = {};
  VkApplicationInfo applicationInfo = {};

  // holds the info about the queues
  struct {
    uint32_t queueFamilyCount = 0;
    std::vector<VkQueueFamilyProperties> props;
    const float priority = 1.0f;
    const uint32_t nQueueInfos = 2; // TODO: Magic number :(
    std::array<VkDeviceQueueCreateInfo, 2> createInfo;
    uint32_t graphicsIndex = -1;
    uint32_t computeIndex = -1;
    VkQueue computeQ = 0;
    VkQueue graphicsQ = 0;
  } queues;

  // TODO: magic words :(
  const std::vector<const char *> enabledExtensions = {
      "VK_KHR_ray_tracing_pipeline",
      "VK_KHR_acceleration_structure",
      "VK_KHR_spirv_1_4",
      "VK_KHR_shader_float_controls", // TODO: required?
      "VK_KHR_buffer_device_address",
      "VK_KHR_deferred_host_operations",
      "VK_KHR_pipeline_library"};

  // create infos
  struct {
    VkPhysicalDeviceBufferDeviceAddressFeatures bufferDevAdressFeatures = {};
    VkPhysicalDeviceRayTracingPipelineFeaturesKHR rtPipelineFeatures = {};
    VkPhysicalDeviceAccelerationStructureFeaturesKHR accStructFeatures = {};
    VkDeviceCreateInfo info = {};
  } createInfo;

  struct {
    VkPhysicalDevice physDeviceHandle = VK_NULL_HANDLE;
    VkPhysicalDeviceFeatures physDeviceFeatures;
    uint32_t deviceCount = 0;
    std::vector<VkPhysicalDevice> listOfDevices;
    std::vector<VkPhysicalDeviceProperties> deviceProperties;
    VkPhysicalDeviceMemoryProperties memoryProperties;
  } phyDevices;
  struct {
    VkInstanceCreateInfo instanceCreateInfo = {};
    uint32_t layerCount = 0;
    VkLayerProperties *layerProperties = NULL;
    std::vector<VkLayerProperties> availableLayers;
    const std::vector<const char *> requestedLayers = {
        "VK_LAYER_KHRONOS_VALIDATION"};
  } instanceInfo;

  struct {
    VkCommandPoolCreateInfo info;
    VkCommandPool pool;
  } commandPool;
};