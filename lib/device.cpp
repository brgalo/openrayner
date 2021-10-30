#include "vulkan/vulkan.hpp"
#include "device.hpp"

void Device::initVulkan() {
    createInstance();
    createDevice();
    getQueueInfo();
    setDeviceFunctionPointers();
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
    selectDevice();
    getQueueFamilyProperties();
    printQueueFamilyProperties();
    createDeviceQueueInfo();    
    createDeviceInfo();
    createLogicalDevice();
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

void Device::getInstanceLayers() {
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
    layers = new VkLayerProperties[layerCount];
    vkEnumerateInstanceLayerProperties(&layerCount, layers);
}

void Device::printInstanceLayers() {
    std::cout << "Instance Layers (total: " << layerCount << ")" << std::endl;
    for(size_t i = 0; i<layerCount; ++i) {
        std::cout << i << " " << layers[i].layerName << std::endl;
    }
}

void Device::getInstanceExtensions() {
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionsCount, NULL);
    extensions = new VkExtensionProperties[extensionsCount];
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionsCount, extensions);
}

void Device::printInstanceExtensions() {
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

void Device::selectDevice() {
    for(size_t i = 0; i<physDeviceCount; ++i) {
        VkPhysicalDeviceProperties properties;
        vkGetPhysicalDeviceProperties(physDevices[i], &properties);
        std::cout << "device no: " << i << std::endl;
        std::cout << "device name: " << properties.deviceName << std::endl;
        std::cout << "device type: " << properties.deviceType << std::endl;

        // check if dedicated gpu exists
        if(properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
            selectedPhysicalDevice = &physDevices[i];
        }
    }
    if(selectedPhysicalDevice == nullptr) {
        std::cerr << "no suitable device detected, exiting program" << std::endl;
        EXIT_FAILURE;
    }
}

void Device::createDeviceInfo() {
    deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceCreateInfo.pNext = NULL;
    deviceCreateInfo.flags = 0;
    deviceCreateInfo.queueCreateInfoCount = 1;
    deviceCreateInfo.pQueueCreateInfos = &deviceQueueCreateInfo;

    // enabled layers:
    // for rt
    enabledDeviceExtensions.push_back(VK_KHR_ACCELERATION_STRUCTURE_EXTENSION_NAME);
    enabledDeviceExtensions.push_back(VK_KHR_RAY_TRACING_PIPELINE_EXTENSION_NAME);
    // for VK_KHR_ACC
    enabledDeviceExtensions.push_back(VK_KHR_BUFFER_DEVICE_ADDRESS_EXTENSION_NAME);
    enabledDeviceExtensions.push_back(VK_KHR_DEFERRED_HOST_OPERATIONS_EXTENSION_NAME);
    enabledDeviceExtensions.push_back(VK_EXT_DESCRIPTOR_INDEXING_EXTENSION_NAME);
    // for raytracing pipeline
    enabledDeviceExtensions.push_back(VK_KHR_SPIRV_1_4_EXTENSION_NAME);
    // for SPIRV
    enabledDeviceExtensions.push_back(VK_KHR_SHADER_FLOAT_CONTROLS_EXTENSION_NAME);

    // features for RT
    VkPhysicalDeviceRayTracingPipelineFeaturesKHR rtPipelineFeatures = {};
    

    deviceCreateInfo.enabledLayerCount = 0;
    deviceCreateInfo.ppEnabledLayerNames = NULL;
    deviceCreateInfo.enabledExtensionCount = enabledDeviceExtensions.size();
    deviceCreateInfo.ppEnabledExtensionNames = enabledDeviceExtensions.data();
    deviceCreateInfo.pEnabledFeatures = nullptr;
}

void Device::createDeviceQueueInfo() {
    // TODO: How do i even assemble a queue from all the families?
    deviceQueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    deviceQueueCreateInfo.pNext = NULL;
    deviceQueueCreateInfo.flags = 0;
    deviceQueueCreateInfo.queueFamilyIndex = 0;
    deviceQueueCreateInfo.queueCount = 1; // TODO: How many do i actually need?
    deviceQueueCreateInfo.pQueuePriorities = NULL;

}

void Device::createLogicalDevice() {
    VkResult result = vkCreateDevice(*selectedPhysicalDevice, &deviceCreateInfo, nullptr, &logicalDevice);
    ASSERT_VULKAN(result);
}

void Device::getQueueInfo() {
    //vkGetDeviceQueue(device, );
}

void Device::getQueueFamilyProperties() {
    vkGetPhysicalDeviceQueueFamilyProperties(*selectedPhysicalDevice, &queueFamiliesCount, NULL);
    queueFamilyProperties = new VkQueueFamilyProperties[queueFamiliesCount];
    vkGetPhysicalDeviceQueueFamilyProperties(*selectedPhysicalDevice, &queueFamiliesCount, queueFamilyProperties);
}

void Device::printQueueFamilyProperties() {
    std::cout << "Queue Families: (total: " << queueFamiliesCount << ")" << std::endl;
    for(size_t i = 0; i<queueFamiliesCount; ++i) {
        std::cout << i << ": " << queueFamilyProperties[i].queueCount << " queues" << std::endl;
        std::cout << "VK_QUEUE_GRAPHICS_BIT       " << ((queueFamilyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) != 0) << std::endl;
        std::cout << "VK_QUEUE_COMPUTE_BIT        " << ((queueFamilyProperties[i].queueFlags & VK_QUEUE_COMPUTE_BIT) != 0) << std::endl;
        std::cout << "VK_QUEUE_TRANSFER_BIT       " << ((queueFamilyProperties[i].queueFlags & VK_QUEUE_TRANSFER_BIT) != 0) << std::endl;
        std::cout << "VK_QUEUE_SPARSE_BINDING_BIT " << ((queueFamilyProperties[i].queueFlags & VK_QUEUE_SPARSE_BINDING_BIT) != 0) << std::endl;
    }
}

void Device::setDeviceFunctionPointers() {
    std::cout << "Helo" << std::endl;
	vkCmdBuildAccelerationStructuresKHR = reinterpret_cast<PFN_vkCmdBuildAccelerationStructuresKHR>(vkGetDeviceProcAddr(logicalDevice, "vkCmdBuildAccelerationStructuresKHR"));
	vkBuildAccelerationStructuresKHR = reinterpret_cast<PFN_vkBuildAccelerationStructuresKHR>(vkGetDeviceProcAddr(logicalDevice, "vkBuildAccelerationStructuresKHR"));
    vkGetBufferDeviceAddressKHR = reinterpret_cast<PFN_vkGetBufferDeviceAddressKHR>(vkGetDeviceProcAddr(logicalDevice, "vkGetBufferDeviceAddressKHR"));
	vkCreateAccelerationStructureKHR = reinterpret_cast<PFN_vkCreateAccelerationStructureKHR>(vkGetDeviceProcAddr(logicalDevice, "vkCreateAccelerationStructureKHR"));
	vkDestroyAccelerationStructureKHR = reinterpret_cast<PFN_vkDestroyAccelerationStructureKHR>(vkGetDeviceProcAddr(logicalDevice, "vkDestroyAccelerationStructureKHR"));
	vkGetAccelerationStructureBuildSizesKHR = reinterpret_cast<PFN_vkGetAccelerationStructureBuildSizesKHR>(vkGetDeviceProcAddr(logicalDevice, "vkGetAccelerationStructureBuildSizesKHR"));
	vkGetAccelerationStructureDeviceAddressKHR = reinterpret_cast<PFN_vkGetAccelerationStructureDeviceAddressKHR>(vkGetDeviceProcAddr(logicalDevice, "vkGetAccelerationStructureDeviceAddressKHR"));
	vkCmdTraceRaysKHR = reinterpret_cast<PFN_vkCmdTraceRaysKHR>(vkGetDeviceProcAddr(logicalDevice, "vkCmdTraceRaysKHR"));
	vkGetRayTracingShaderGroupHandlesKHR = reinterpret_cast<PFN_vkGetRayTracingShaderGroupHandlesKHR>(vkGetDeviceProcAddr(logicalDevice, "vkGetRayTracingShaderGroupHandlesKHR"));
	vkCreateRayTracingPipelinesKHR = reinterpret_cast<PFN_vkCreateRayTracingPipelinesKHR>(vkGetDeviceProcAddr(logicalDevice, "vkCreateRayTracingPipelinesKHR"));
}