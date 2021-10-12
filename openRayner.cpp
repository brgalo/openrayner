// hello there!
#include <string>
#include <memory>
#include <iostream>
#include <vector>
#include <array>

#include "./3rdParty/tinyGLTF/tiny_gltf.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include "./3rdParty/tinyOBJ/tiny_obj_loader.h"
#include "vulkan/vulkan.h"
//#include "glm/glm.hpp"
#include "openRayner.h"

#define PRINT_INFO false


class Scene {
    public:
    //bool loadFromGltf(std::string fileName, tinygltf::Model &model);
    bool loadFromObj(std::string fileName, std::string mtlFileName);
    
    void loadFromFile(std::string fileName);
    void loadScene();

    private:    
    struct Vertex {
        float pos;
    };
    struct Model {
        std::vector<Vertex> vertices;
        std::vector<std::array<uint32_t, 3>> indices;
        std::string name;
        uint32_t meshNumber;
    };
    std::vector<Model> models = {};
    bool parseFromImportedObj(tinyobj::attrib_t importedObj);
};

class geometricData {

};



void Scene::loadFromFile(std::string fileName) {
    
    // loads file from GLTF


}

void Scene::loadScene() {

}


bool Scene::loadFromObj(std::string fileName, std::string mtlFileName) {
    std::cout << "Loading " << fileName << std::endl;

    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    const char* filename = fileName.c_str();
    const char* mtlfile = mtlFileName.c_str();
    std::string warn,err;
    bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filename,
                                mtlfile, true);

    if (!warn.empty()) std::cout << "WARN: " << warn << std::endl;
    if (!err.empty()) std::cerr << "ERR: " << err << std::endl;

    if (!ret) {
        std::cout << "Failed to load/parse " << fileName << std::endl;
        return false;
    }

    std::cout << "Loading completed successful!" << std::endl;
    std::cout << "Parsing..." << std::endl;
    for (auto shape : shapes) {
        std::cout << shape.name << std::endl;
        std::cout << static_cast<unsigned long>(shape.mesh.indices.size());
    }

    return ret;
}

void printStats(VkPhysicalDevice &device) {
    VkPhysicalDeviceProperties properties;
    vkGetPhysicalDeviceProperties(device, &properties);

    std::cout << "Name:          " << properties.deviceName << std::endl;
    std::cout << "API Version:   " << VK_VERSION_MAJOR(properties.apiVersion) << ".";
    std::cout << VK_VERSION_MINOR(properties.apiVersion) << ".";
    std::cout << VK_VERSION_PATCH(properties.apiVersion) << std::endl;
    std::cout << "Device Type:   " << properties.deviceType << std::endl;
    std::cout << "DiscQueueProp: " << properties.limits.discreteQueuePriorities << std::endl;

    VkPhysicalDeviceMemoryProperties memProps;
    vkGetPhysicalDeviceMemoryProperties(device, &memProps);

    uint32_t amountOfQueueFamilies = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &amountOfQueueFamilies, NULL);
    std::cout << "n Queue Fams: " << amountOfQueueFamilies << std::endl;
    VkQueueFamilyProperties *familiyProperties = new VkQueueFamilyProperties[amountOfQueueFamilies];
    vkGetPhysicalDeviceQueueFamilyProperties(device, &amountOfQueueFamilies, familiyProperties);

    for(size_t i = 0; i < amountOfQueueFamilies; ++i) {
        std::cout << std::endl;
        std::cout << "Queue Family #" << i << std::endl;
        std::cout << "VK_QUEUE_GRAPHICS_BIT       " << ((familiyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) != 0) << std::endl;
        std::cout << "VK_QUEUE_COMPUTE_BIT        " << ((familiyProperties[i].queueFlags & VK_QUEUE_COMPUTE_BIT) != 0) << std::endl;
        std::cout << "VK_QUEUE_TRANSFER_BIT       " << ((familiyProperties[i].queueFlags & VK_QUEUE_TRANSFER_BIT) != 0) << std::endl;
        std::cout << "VK_QUEUE_SPARSE_BINDING_BIT " << ((familiyProperties[i].queueFlags & VK_QUEUE_SPARSE_BINDING_BIT) != 0) << std::endl;
        std::cout << "Queue Count: " << familiyProperties[i].queueCount << std::endl;
        std::cout << "Timestamp Valid Bits: " << familiyProperties[i].queueCount << std::endl;
        std::cout << "Min Image Timestamp Granularity: " << familiyProperties[i].minImageTransferGranularity.width << ", "
                                                         << familiyProperties[i].minImageTransferGranularity.height << ", "
                                                         << familiyProperties[i].minImageTransferGranularity.depth << std::endl;
    }

}

void deviceInfo() {
    // name and version of application
    VkApplicationInfo appInfo;
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pNext = NULL;
    appInfo.pApplicationName = "openRayner";
    appInfo.applicationVersion = VK_MAKE_VERSION(0, 0, 0);
    appInfo.pEngineName = "Qdot";
    appInfo.engineVersion = VK_MAKE_VERSION(0,0,0);
    appInfo.apiVersion = VK_API_VERSION_1_2;

    uint32_t amountOfLayers = 0;
    vkEnumerateInstanceLayerProperties(&amountOfLayers, NULL);
    VkLayerProperties *layers = new VkLayerProperties[amountOfLayers];
    vkEnumerateInstanceLayerProperties(&amountOfLayers, layers);

    std::cout << "n InstanceLayers: " << amountOfLayers << std::endl;
    for(size_t i = 0; i < amountOfLayers; ++i) {
        std::cout << std::endl;
        std::cout << "Name:       " << layers[i].layerName << std::endl;
        std::cout << "Spec Ver:   " << layers[i].specVersion << std::endl;
        std::cout << "Impl Ver:   " << layers[i].implementationVersion << std::endl;
        std::cout << "Desciption: " << layers[i].description << std::endl;
    }

    const std::vector<const char*> validationLayers = {
        "VK_LAYER_KHRONOS_validation"
    };

    uint32_t amountOfExtensions = 0;
    vkEnumerateInstanceExtensionProperties(NULL, &amountOfExtensions, NULL);
    VkExtensionProperties *extensions = new VkExtensionProperties[amountOfExtensions];
    vkEnumerateInstanceExtensionProperties(NULL, &amountOfExtensions, extensions);
    std::cout << std::endl;
    std::cout << "n Extensions: " << amountOfExtensions << std::endl;
    for(size_t i = 0; i < amountOfExtensions; ++i) {
        std::cout << std::endl;
        std::cout << "Name : " << extensions[i].extensionName << std::endl;
        std::cout << "Spec Ver: " << extensions[i].specVersion << std:: endl;
    }

    // information of the instance to be launched
    VkInstanceCreateInfo instanceInfo;
    instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instanceInfo.pNext = NULL;
    instanceInfo.flags = 0;
    instanceInfo.pApplicationInfo = &appInfo;
    instanceInfo.enabledLayerCount = validationLayers.size();
    instanceInfo.ppEnabledLayerNames = validationLayers.data();
    instanceInfo.enabledExtensionCount = 0;
    instanceInfo.ppEnabledExtensionNames = NULL;


    // the main instance
    VkInstance instance;

    VkResult result = vkCreateInstance(&instanceInfo, NULL, &instance);
    ASSERT_VULKAN(result);

    // get physical reference
    uint32_t nPhysicalDevices = 0; // gets updated by
    result = vkEnumeratePhysicalDevices(instance, &nPhysicalDevices, NULL);
    ASSERT_VULKAN(result);
    VkPhysicalDevice *physicalDevices = new VkPhysicalDevice[nPhysicalDevices];
    result = vkEnumeratePhysicalDevices(instance, &nPhysicalDevices, physicalDevices);
    ASSERT_VULKAN(result);

    printStats(physicalDevices[0]);
    //printStats(physicalDevices[1]);

    float queuePrios[] = {1.f};

    VkDeviceQueueCreateInfo queueInfo;
    queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueInfo.pNext = NULL;
    queueInfo.flags = 0;
    queueInfo.queueFamilyIndex = 0;
    queueInfo.queueCount = 1;
    queueInfo.pQueuePriorities = queuePrios;


    VkPhysicalDeviceFeatures usedFeatures = {};

    VkDeviceCreateInfo deviceCreateInfo;
    deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceCreateInfo.pNext = NULL;
    deviceCreateInfo.flags = 0;
    deviceCreateInfo.queueCreateInfoCount = 1;
    deviceCreateInfo.pQueueCreateInfos = &queueInfo;
    deviceCreateInfo.enabledLayerCount = 0;
    deviceCreateInfo.ppEnabledLayerNames = NULL;
    deviceCreateInfo.enabledExtensionCount = 0;
    deviceCreateInfo.ppEnabledExtensionNames = NULL;
    deviceCreateInfo.pEnabledFeatures = &usedFeatures;
    
    VkDevice device;
    //TODO Pick correct device
    result = vkCreateDevice(physicalDevices[0],&deviceCreateInfo, NULL, &device);
    ASSERT_VULKAN(result);

    // device layers
    uint32_t nDeviceExtensions = 0;
    vkEnumerateDeviceExtensionProperties(physicalDevices[0], NULL, &nDeviceExtensions, NULL);
    VkExtensionProperties *devExtension = new VkExtensionProperties[nDeviceExtensions];
    vkEnumerateDeviceExtensionProperties(physicalDevices[0], NULL, &nDeviceExtensions, devExtension);

    std::cout << std::endl;
    std::cout << "n Device Extensions: " << nDeviceExtensions << std::endl;
    for(size_t i = 0; i < nDeviceExtensions; ++i) {
        std::cout << std::endl;
        std::cout << "Name : " << devExtension[i].extensionName << std::endl;
        std::cout << "Spec Ver: " << devExtension[i].specVersion << std:: endl;
    }
}

int main() {
    Scene scene;
    std::string filename = "../testGeometry/test.obj";
    std::string mtlfilename = "../testGeometry/";
    scene.loadFromObj(filename,mtlfilename);

    return 0;
}