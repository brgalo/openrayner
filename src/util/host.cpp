
#include "host.hpp"
#include <cstdint>
#include <cstdlib>
#include <iostream>
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
  applicationInfo.apiVersion = VK_API_VERSION_1_2; // TODO: see 1.3

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
  instanceCreateInfo.enabledLayerCount = instanceInfo.layerCount; // TODO: change if want windows
  instanceCreateInfo.ppEnabledLayerNames = instanceInfo.requestedLayers.data();
  instanceCreateInfo.enabledExtensionCount = 0;
  instanceCreateInfo.ppEnabledExtensionNames = NULL;

    // create Instance
  if(vkCreateInstance(&instanceCreateInfo, NULL, &instance) == VK_SUCCESS) std::cout << "Instance created!\n";

}

void Application::setupDevices() {
    vkEnumeratePhysicalDevices(instance, &phyDevices.deviceCount, NULL);
    phyDevices.listOfDevices.resize(phyDevices.deviceCount);
    vkEnumeratePhysicalDevices(instance, &phyDevices.deviceCount, phyDevices.listOfDevices.data());
}