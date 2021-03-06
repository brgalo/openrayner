// hello there!
#include <string>
#include <memory>
#include <iostream>
#include <vector>
#include <array>


#define TINYOBJLOADER_IMPLEMENTATION
#include "3rdParty/tinyOBJ/tiny_obj_loader.h"
#include "vulkan/vulkan.h"
#include "glm/glm.hpp"
#include "openRayner.h"
#include "lib/device.hpp"

#define PRINT_INFO false

class PrimitiveData {
    public:
    int32_t meshID = -1;
    int32_t materialID = -1;
};

class Mesh {
    public:
    std::string name;
    uint32_t nPrimitives;
    uint32_t primitiveOffset;
};

struct Index {
    union {
        uint32_t raw[3];
        struct {
            uint32_t a,b,c;
        };
    };
};

class Scene {

    
    public:
    //bool loadFromGltf(std::string fileName, tinygltf::Model &model);
    bool loadModelFromFile(std::string fileName, std::string mtlFileName);
    bool pushBuffersToDevice();
    void loadScene();
    
    class GeometricData {


        public:
        std::vector<Mesh> meshes;
        std::vector<glm::vec3> vertices;
        std::vector<Index> indices;
        std::vector<glm::vec3> normals;
        std::vector<Index> normIdx;
        std::vector<PrimitiveData> primitives;
    } geometryData;
    std::vector<glm::vec3> vertices;

    private:
    bool parseFromImportedObj(tinyobj::attrib_t importedObj);

};






bool Scene::loadModelFromFile(std::string fileName, std::string mtlFileName) {
    
    // assumes .obj files
    tinyobj::ObjReader reader;
    tinyobj::ObjReaderConfig readerConfig;
    readerConfig.mtl_search_path = mtlFileName;

    if (!reader.ParseFromFile(fileName, readerConfig)) {
        if (!reader.Error().empty()) {
            std::cerr << "TinyObjReader: " << reader.Error();
        }
        exit(1);
    }

    if (!reader.Warning().empty()) {
        std::cout << "TinyObjReader: " << reader.Warning();
    }

    auto& attrib = reader.GetAttrib();
    auto& shapes = reader.GetShapes();
    auto& materials = reader.GetMaterials();

    // TODO: check if all faces are triangles!
    {
    //get vec3 typed acess to the float based vertex/normal buffers
    const glm::vec3* vertBegin = reinterpret_cast<const glm::vec3*>(attrib.vertices.data());
    const glm::vec3* normBegin = reinterpret_cast<const glm::vec3*>(attrib.normals.data());

    // deep copy the vertex and normal buffer from floats to glm::vec3
    geometryData.vertices.assign(vertBegin,vertBegin + attrib.vertices.size()/3);
    geometryData.normals.assign(normBegin,normBegin+attrib.normals.size()/3);
    }


    // create linear primitive buffer
    uint32_t offset = 0;
    uint32_t currMeshNo = 0;
    for (auto& shape : shapes) {
        std::vector<PrimitiveData> temp;
        Mesh currMesh;
        currMesh.name = shape.name;
        currMesh.nPrimitives = shape.mesh.indices.size()/3;
        currMesh.primitiveOffset = offset;
        // std::cout << "Shape name: " << currMesh.name << std::endl;

        offset += currMesh.nPrimitives;
        // std::cout << "offset: " << offset << std::endl;

        geometryData.meshes.push_back(currMesh);

        for(size_t prim = 0; prim < currMesh.nPrimitives; ++prim) {
            PrimitiveData data;
            Index idx,normIdx;
            // copy indices
            idx.a = shape.mesh.indices[3*prim + 0].vertex_index;
            idx.b = shape.mesh.indices[3*prim + 1].vertex_index;
            idx.c = shape.mesh.indices[3*prim + 2].vertex_index;
            geometryData.indices.push_back(idx);

            // copy normal indices
            normIdx.a = shape.mesh.indices[3*prim + 0].normal_index;
            normIdx.b = shape.mesh.indices[3*prim + 1].normal_index;
            normIdx.c = shape.mesh.indices[3*prim + 2].normal_index;
            geometryData.normIdx.push_back(normIdx);
            data.meshID = currMeshNo;
            data.materialID = shape.mesh.material_ids[prim];
        }
        ++currMeshNo;
    }
    return true;
}

bool Scene::pushBuffersToDevice() {

    return true;
}

void Scene::loadScene() {

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
    scene.loadModelFromFile(filename,mtlfilename);
    
    Device device;
    device.getInstanceLayers();
    device.printInstanceLayers();
    device.getInstanceExtensions();
    device.printInstanceExtensions();
    device.initVulkan();

    return 0;
}