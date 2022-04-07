#include "vulkaninstance.hpp"
#include "host.hpp"
#include "iostream"
#include <vulkan/vulkan_core.h>


void VulkanInstance::initInstance() {
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

  instanceInfo.instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  instanceInfo.instanceCreateInfo.pNext = nullptr; // TODO: migght need an info struct for validation layers!
  instanceInfo.instanceCreateInfo.flags = 0;
  instanceInfo.instanceCreateInfo.pApplicationInfo = &applicationInfo;
  instanceInfo.instanceCreateInfo.enabledLayerCount = 0;//instanceInfo.layerCount; // TODO: change if want windows
  instanceInfo.instanceCreateInfo.ppEnabledLayerNames = NULL;//instanceInfo.requestedLayers.data();
  instanceInfo.instanceCreateInfo.enabledExtensionCount = 0;
  instanceInfo.instanceCreateInfo.ppEnabledExtensionNames = NULL;

    // create Instance
  if(vkCreateInstance(&instanceInfo.instanceCreateInfo, NULL, &instance) == VK_SUCCESS) std::cout << "Instance created!\n";
}

void VulkanInstance::setupDevice() {
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
            phyDevices.physDeviceHandle = phyDevices.listOfDevices[i];
            std::cout << "Set to device: " << i << ".\n";
        }
    }
    // check for RT extenstions
    std::cout << "\n check device features:\n";
    vkGetPhysicalDeviceFeatures(phyDevices.physDeviceHandle, &phyDevices.physDeviceFeatures);
    // TODO: Check if relevant?
    std::cout << "missing functionality, also not sure if relevant for Raytracing extensions\n";

    // check memory properties
    vkGetPhysicalDeviceMemoryProperties(phyDevices.physDeviceHandle, &phyDevices.memoryProperties);

    // check queue settings
    vkGetPhysicalDeviceQueueFamilyProperties(phyDevices.physDeviceHandle, &queues.queueFamilyCount, NULL);
    queues.props.resize(queues.queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(phyDevices.physDeviceHandle, &queues.queueFamilyCount, queues.props.data());
    for(size_t i = 0; i < queues.queueFamilyCount ; ++i) {
        std::cout << "Queue Fam # " << i << ":\n";
        std::cout << "GRAPHICS: " << ((queues.props[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) != 0) << "\n";
        std::cout << "COMPUTE: " << ((queues.props[i].queueFlags & VK_QUEUE_COMPUTE_BIT)   != 0) << "\n";
        std::cout << "TRANSFER: " << ((queues.props[i].queueFlags & VK_QUEUE_TRANSFER_BIT) != 0) << "\n";
        std::cout << "# of queue: " << queues.props[i].queueCount << "\n\n";
    }

    // TODO: select highest number of COMPUTE queues as compute Queue
    {
        size_t i = 0;
        for (const auto& queue : queues.props) {
            if((((queue.queueFlags & VK_QUEUE_GRAPHICS_BIT)!=0) && queues.graphicsIndex == -1) ){
                queues.graphicsIndex = i;
            }
            else if((((queue.queueFlags & VK_QUEUE_COMPUTE_BIT)!=0) && queues.computeIndex == -1)) {
                queues.computeIndex = i;
            }
            ++i;
        }
        if( (queues.computeIndex == queues.graphicsIndex) || ((queues.graphicsIndex == -1) || (queues.computeIndex == -1))) {
            std::cout << "Error assinging queue indices!\n";
            exit(EXIT_FAILURE);
        } else {
            std::cout << "Assigned Queues:\ngraphics queue idx: " << queues.graphicsIndex << "\n";
            std::cout << "compute queue idx: " << queues.computeIndex << "\n";
        }
    }
    {
        // graphics queue (for rt?)
        queues.createInfo[0].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queues.createInfo[0].pNext = NULL;
        queues.createInfo[0].flags = 0;
        queues.createInfo[0].queueFamilyIndex = queues.graphicsIndex;
        queues.createInfo[0].queueCount = 1;
        queues.createInfo[0].pQueuePriorities = &queues.priority;

        // compute queue create acc structure!
        queues.createInfo[1].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queues.createInfo[1].pNext = NULL;
        queues.createInfo[1].flags = 0;
        queues.createInfo[1].queueFamilyIndex = queues.computeIndex;
        queues.createInfo[1].queueCount = 1;
        queues.createInfo[1].pQueuePriorities = &queues.priority;
    }
    {   
        createInfo.bufferDevAdressFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_BUFFER_DEVICE_ADDRESS_FEATURES;
        createInfo.bufferDevAdressFeatures.pNext = NULL;
        createInfo.bufferDevAdressFeatures.bufferDeviceAddress = VK_TRUE;
        createInfo.bufferDevAdressFeatures.bufferDeviceAddressCaptureReplay = VK_FALSE;
        createInfo.bufferDevAdressFeatures.bufferDeviceAddressMultiDevice = VK_FALSE;

        createInfo.rtPipelineFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ACCELERATION_STRUCTURE_FEATURES_KHR;
        createInfo.rtPipelineFeatures.pNext = &createInfo.bufferDevAdressFeatures;
        createInfo.rtPipelineFeatures.rayTracingPipeline = VK_TRUE;
        createInfo.rtPipelineFeatures.rayTracingPipelineShaderGroupHandleCaptureReplay = VK_FALSE;
        createInfo.rtPipelineFeatures.rayTracingPipelineShaderGroupHandleCaptureReplayMixed = VK_FALSE;
        createInfo.rtPipelineFeatures.rayTracingPipelineTraceRaysIndirect = VK_FALSE;
        createInfo.rtPipelineFeatures.rayTraversalPrimitiveCulling = VK_FALSE; 

        createInfo.accStructFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ACCELERATION_STRUCTURE_FEATURES_KHR;
        createInfo.accStructFeatures.pNext = &createInfo.rtPipelineFeatures;
        createInfo.accStructFeatures.accelerationStructure = VK_TRUE;
        createInfo.accStructFeatures.accelerationStructureCaptureReplay = VK_FALSE;
        createInfo.accStructFeatures.accelerationStructureIndirectBuild = VK_FALSE;
        createInfo.accStructFeatures.accelerationStructureHostCommands = VK_FALSE;
        createInfo.accStructFeatures.descriptorBindingAccelerationStructureUpdateAfterBind = VK_FALSE; // TODO: Update here if the acc changes!

        createInfo.info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        createInfo.info.pNext = &createInfo.accStructFeatures;
        createInfo.info.flags = 0;
        createInfo.info.queueCreateInfoCount = queues.createInfo.size();
        createInfo.info.pQueueCreateInfos = queues.createInfo.data();
        createInfo.info.enabledLayerCount = 0;
        createInfo.info.ppEnabledLayerNames = nullptr;
        createInfo.info.enabledExtensionCount = enabledExtensions.size();
        createInfo.info.ppEnabledExtensionNames = enabledExtensions.data();
        createInfo.info.pEnabledFeatures = NULL;

        if(vkCreateDevice(phyDevices.physDeviceHandle, &createInfo.info, NULL, &logicalDevHandle) == VK_SUCCESS) {
            std::cout << "\nDevice succesfully created!\n\n";
        }
        else exit(EXIT_FAILURE);
    }
    {
        //create Queues
        vkGetDeviceQueue(logicalDevHandle, queues.graphicsIndex, 0, &queues.graphicsQ);
        vkGetDeviceQueue(logicalDevHandle, queues.computeIndex,  0, &queues.computeQ );
    }
}

void VulkanInstance::createBuffers(Application &app) {
    // command pool
    commandPool.info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    commandPool.info.pNext = NULL;
    commandPool.info.flags = 0;
    commandPool.info.queueFamilyIndex = queues.graphicsIndex;

    if(vkCreateCommandPool(logicalDevHandle, &commandPool.info, NULL, &commandPool.pool) == VK_SUCCESS) {
        std::cout << "Created Command Pool!\n";
    }

    // vertex Buffer
    app.buffers.vertices = VertexBuffer(app.geom.vertexBuf.size()*sizeof(glm::vec3));
    app.buffers.vertices.create(app.vkIns);
    app.buffers.vertices.copyH2D(app.vkIns,app.geom.vertexBuf.data());
    std::cout << "vertex buffer created!\n";
    // idx Buffer
    app.buffers.verIdx = IndexBuffer(app.geom.vertexBuf.size()*sizeof(glm::ivec3));
    app.buffers.verIdx.create(app.vkIns);
    app.buffers.verIdx.copyH2D(app.vkIns,app.geom.vertexBuf.data());
    std::cout << "index buffer created!\n";
}


VkPhysicalDeviceMemoryProperties VulkanInstance::getMemProps() const {
    return this->phyDevices.memoryProperties ;
}

VkCommandPool VulkanInstance::getCmdPool() const {
    return this->commandPool.pool;
}

VkQueue VulkanInstance::getGraphicsQ() const {
    return this->queues.graphicsQ;
}

VkQueue VulkanInstance::getComputeQ() const {
    return this->queues.computeQ;
}