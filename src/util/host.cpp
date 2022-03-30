
#include "vulkan/vulkan.hpp"
#include "host.hpp"
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <locale>
#include <string>
#include <vector>
#include <vulkan/vulkan_core.h>




void Application::initInstance() {
  // populate app&instancecreate info
  applicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  applicationInfo.pNext = NULL;
  applicationInfo.pApplicationName = "openRayner";
  applicationInfo.applicationVersion = VK_MAKE_VERSION(0, 0, 1);
  applicationInfo.pEngineName = "G&G";
  applicationInfo.engineVersion = VK_MAKE_VERSION(0, 0, 1);
  applicationInfo.apiVersion = VK_API_VERSION_1_3;

  // retrieve available layers -> TODO: toggle validation
  vkEnumerateInstanceLayerProperties(&instanceInfo.layerCount, nullptr);
  instanceInfo.availableLayers.resize(instanceInfo.layerCount);
  vkEnumerateInstanceLayerProperties(&instanceInfo.layerCount,
                                     instanceInfo.availableLayers.data());

  // verify existence of requested layers:
  {
    bool missingLayers = false;
    for (const char* req : instanceInfo.requestedLayers) {
      bool foundCurr = false;
      for (const auto ava : instanceInfo.availableLayers) {
      foundCurr |= strcmp(req,ava.layerName);
      }
      if (foundCurr == false) {
        std::cout << "Error: " << req << " not Found!\n";
        missingLayers = true;
      }
      else {
          std::cout << "Requested Layer Present: " << req << "\n";
      }
    }
    if (missingLayers == true)
      exit(EXIT_FAILURE);
  }

  instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  instanceCreateInfo.pNext = nullptr; // TODO: migght need an info struct for validation layers!
  instanceCreateInfo.flags = 0;
  instanceCreateInfo.pApplicationInfo = &applicationInfo;
  instanceCreateInfo.enabledLayerCount = 0;//instanceInfo.layerCount; // TODO: change if want windows
  instanceCreateInfo.ppEnabledLayerNames = NULL;//instanceInfo.requestedLayers.data();
  instanceCreateInfo.enabledExtensionCount = 0;
  instanceCreateInfo.ppEnabledExtensionNames = NULL;

    // create Instance
  if(vkCreateInstance(&instanceCreateInfo, NULL, &instance) == VK_SUCCESS) std::cout << "Instance created!\n";

}

void Application::setupDevice() {
    // get list of phyical devices
    vkEnumeratePhysicalDevices(instance, &phyDevices.deviceCount, NULL);
    phyDevices.listOfDevices.resize(phyDevices.deviceCount);
    phyDevices.deviceProperties.resize(phyDevices.deviceCount);
    vkEnumeratePhysicalDevices(instance, &phyDevices.deviceCount, phyDevices.listOfDevices.data());
    std::cout << "Found " << phyDevices.deviceCount << " devices.\n";
    for(size_t i = 0; i < phyDevices.deviceCount; ++i) {
        vkGetPhysicalDeviceProperties(phyDevices.listOfDevices[i], &phyDevices.deviceProperties[i]);
        std::cout << "Dev " << i << ":\n";
        std::cout << "Name: " << phyDevices.deviceProperties[i].deviceName << "\n";
        std::cout << "Type: " << phyDevices.deviceProperties[i].deviceType << "\n";
        if(phyDevices.deviceProperties[i].deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
            // set device to type 2 -> dicrete GPU
            device.physDeviceHandle = phyDevices.listOfDevices[i];
            std::cout << "Set to device: " << i << ".\n";
        }
    }
    // check for RT extenstions
    std::cout << "\n check device features:\n";
    vkGetPhysicalDeviceFeatures(device.physDeviceHandle, &device.physDeviceFeatures);
    // TODO: Check if relevant?
    std::cout << "missing functionality, also not sure if relevant for Raytracing extensions\n";

    // check memory properties
    vkGetPhysicalDeviceMemoryProperties(device.physDeviceHandle, &phyDevices.memoryProperties);

    // check queue settings
    vkGetPhysicalDeviceQueueFamilyProperties(device.physDeviceHandle, &device.queues.queueFamilyCount, NULL);
    device.queues.props.resize(device.queues.queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device.physDeviceHandle, &device.queues.queueFamilyCount, device.queues.props.data());
    for(size_t i = 0; i < device.queues.queueFamilyCount ; ++i) {
        std::cout << "Queue Fam # " << i << ":\n";
        std::cout << "GRAPHICS: " << ((device.queues.props[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) != 0) << "\n";
        std::cout << "COMPUTE: " << ((device.queues.props[i].queueFlags & VK_QUEUE_COMPUTE_BIT)   != 0) << "\n";
        std::cout << "TRANSFER: " << ((device.queues.props[i].queueFlags & VK_QUEUE_TRANSFER_BIT) != 0) << "\n";
        std::cout << "# of queue: " << device.queues.props[i].queueCount << "\n\n";
    }

    // TODO: select highest number of COMPUTE queues as compute Queue
    {
        size_t i = 0;
        for (const auto& queue : device.queues.props) {
            if((((queue.queueFlags & VK_QUEUE_GRAPHICS_BIT)!=0) && device.queues.graphicsIndex == -1) ){
                device.queues.graphicsIndex = i;
            }
            else if((((queue.queueFlags & VK_QUEUE_COMPUTE_BIT)!=0) && device.queues.computeIndex == -1)) {
                device.queues.computeIndex = i;
            }
            ++i;
        }
        if( (device.queues.computeIndex == device.queues.graphicsIndex) || ((device.queues.graphicsIndex == -1) || (device.queues.computeIndex == -1))) {
            std::cout << "Error assinging queue indices!\n";
            exit(EXIT_FAILURE);
        } else {
            std::cout << "Assigned Queues:\ngraphics queue idx: " << device.queues.graphicsIndex << "\n";
            std::cout << "compute queue idx: " << device.queues.computeIndex << "\n";
        }
    }
    {
        // graphics queue (for rt?)
        device.queues.createInfo[0].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        device.queues.createInfo[0].pNext = NULL;
        device.queues.createInfo[0].flags = 0;
        device.queues.createInfo[0].queueFamilyIndex = device.queues.graphicsIndex;
        device.queues.createInfo[0].queueCount = 1;
        device.queues.createInfo[0].pQueuePriorities = &device.queues.priority;

        // compute queue create acc structure!
        device.queues.createInfo[1].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        device.queues.createInfo[1].pNext = NULL;
        device.queues.createInfo[1].flags = 0;
        device.queues.createInfo[1].queueFamilyIndex = device.queues.computeIndex;
        device.queues.createInfo[1].queueCount = 1;
        device.queues.createInfo[1].pQueuePriorities = &device.queues.priority;
    }
    {   
        device.createInfo.bufferDevAdressFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_BUFFER_DEVICE_ADDRESS_FEATURES;
        device.createInfo.bufferDevAdressFeatures.pNext = NULL;
        device.createInfo.bufferDevAdressFeatures.bufferDeviceAddress = VK_TRUE;
        device.createInfo.bufferDevAdressFeatures.bufferDeviceAddressCaptureReplay = VK_FALSE;
        device.createInfo.bufferDevAdressFeatures.bufferDeviceAddressMultiDevice = VK_FALSE;

        device.createInfo.rtPipelineFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ACCELERATION_STRUCTURE_FEATURES_KHR;
        device.createInfo.rtPipelineFeatures.pNext = &device.createInfo.bufferDevAdressFeatures;
        device.createInfo.rtPipelineFeatures.rayTracingPipeline = VK_TRUE;
        device.createInfo.rtPipelineFeatures.rayTracingPipelineShaderGroupHandleCaptureReplay = VK_FALSE;
        device.createInfo.rtPipelineFeatures.rayTracingPipelineShaderGroupHandleCaptureReplayMixed = VK_FALSE;
        device.createInfo.rtPipelineFeatures.rayTracingPipelineTraceRaysIndirect = VK_FALSE;
        device.createInfo.rtPipelineFeatures.rayTraversalPrimitiveCulling = VK_FALSE; 

        device.createInfo.accStructFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ACCELERATION_STRUCTURE_FEATURES_KHR;
        device.createInfo.accStructFeatures.pNext = &device.createInfo.rtPipelineFeatures;
        device.createInfo.accStructFeatures.accelerationStructure = VK_TRUE;
        device.createInfo.accStructFeatures.accelerationStructureCaptureReplay = VK_FALSE;
        device.createInfo.accStructFeatures.accelerationStructureIndirectBuild = VK_FALSE;
        device.createInfo.accStructFeatures.accelerationStructureHostCommands = VK_FALSE;
        device.createInfo.accStructFeatures.descriptorBindingAccelerationStructureUpdateAfterBind = VK_FALSE; // TODO: Update here if the acc changes!

        device.createInfo.info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        device.createInfo.info.pNext = &device.createInfo.accStructFeatures;
        device.createInfo.info.flags = 0;
        device.createInfo.info.queueCreateInfoCount = device.queues.createInfo.size();
        device.createInfo.info.pQueueCreateInfos = device.queues.createInfo.data();
        device.createInfo.info.enabledLayerCount = 0;
        device.createInfo.info.ppEnabledLayerNames = nullptr;
        device.createInfo.info.enabledExtensionCount = device.enabledExtensions.size();
        device.createInfo.info.ppEnabledExtensionNames = device.enabledExtensions.data();
        device.createInfo.info.pEnabledFeatures = NULL;

        if(vkCreateDevice(device.physDeviceHandle, &device.createInfo.info, NULL, &device.logicalDevHandle) == VK_SUCCESS) {
            std::cout << "\nDevice succesfully created!\n\n";
        }
        else exit(EXIT_FAILURE);
    }
    {
        //create Queues
        vkGetDeviceQueue(device.logicalDevHandle, device.queues.graphicsIndex, 0, &device.queues.graphicsQ);
        vkGetDeviceQueue(device.logicalDevHandle, device.queues.computeIndex,  0, &device.queues.computeQ );
    }
}

void Application::createBuffers() {
    // command pool
    buffers.commandPool.info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    buffers.commandPool.info.pNext = NULL;
    buffers.commandPool.info.flags = 0;
    buffers.commandPool.info.queueFamilyIndex = device.queues.graphicsIndex;

    if(vkCreateCommandPool(device.logicalDevHandle, &buffers.commandPool.info, NULL, &buffers.commandPool.pool) == VK_SUCCESS) {
        std::cout << "Created Command Pool!\n";
    }


}

void Application::loadModel() {
    readerConfig.mtl_search_path = mtlName;
    if (!reader.ParseFromFile(modelName, readerConfig)) {
        if (!reader.Error().empty()) {
            std::cerr << "TinyObjReader: " << reader.Error();
        }
    }
}