#pragma once
#include "vulkan/vulkan.h"
#include "vulkaninstance.hpp"
#include <cstdint>
#include <vector>
#include <vulkan/vulkan_core.h>
#include "../../3rdParty/glm/glm/glm.hpp"

struct singleBuffer {
    singleBuffer() {};
    singleBuffer(VkDeviceSize SIZE,VkBufferUsageFlags usageFlags,VkMemoryPropertyFlags memFlags) : size(SIZE), usgFlags(usageFlags), memFlags(memFlags) {};
    VkBufferCreateInfo createInfo = {};
    VkMemoryRequirements memReq = {};
    VkDeviceSize size;
    VkBufferUsageFlags usgFlags;
    VkMemoryPropertyFlags memFlags;
    VkBuffer bufferHost;
    VkDeviceMemory bufferMemoryDevice; // on device, not visible to CPU!
    void copyH2D(const VulkanInstance & inst,const void* src);
    void copyD2H(const VulkanInstance & inst);
    void create(const VulkanInstance &inst);
    uint64_t getDeviceAdress(const VulkanInstance &inst);

    private:
    void copyBuffer(const VulkanInstance &inst, VkBuffer from, VkBuffer to);

};

struct VertexBuffer : singleBuffer {
    VertexBuffer(VkDeviceSize SIZE) : singleBuffer(SIZE, VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR |
                                                         VK_BUFFER_USAGE_TRANSFER_DST_BIT |
                                                         VK_BUFFER_USAGE_STORAGE_BUFFER_BIT |
                                                         VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT,
                                                         VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT) {}
};

struct IndexBuffer : singleBuffer {
    IndexBuffer(VkDeviceSize SIZE) : singleBuffer(SIZE,VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR |
                                                       VK_BUFFER_USAGE_TRANSFER_DST_BIT |
                                                       VK_BUFFER_USAGE_STORAGE_BUFFER_BIT |
                                                       VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT,
                                                       VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT) {}
};

struct StagingBuffer : singleBuffer {
    StagingBuffer(singleBuffer &buf, const void* const dataOnHost) : 
        singleBuffer(buf.size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                               VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                               VK_MEMORY_PROPERTY_HOST_COHERENT_BIT), dataOnHost(dataOnHost) {}
    const void * const dataOnHost;
    void * data;
};


struct AccStrBuffer : singleBuffer {
    AccStrBuffer(VkDeviceSize SIZE) :
        singleBuffer(SIZE,VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR |
                          VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT,
			              VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT) {}
};

struct TrafoBuffer : singleBuffer {
    TrafoBuffer(VkDeviceSize SIZE) :
        singleBuffer(SIZE,VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT |
                          VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR,
			              VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT)
    {}
};

struct Buffers{
        VertexBuffer vertices = 0;
        IndexBuffer verIdx = 0;
        AccStrBuffer accelStrct = 0;
        TrafoBuffer trafo = 0;
        singleBuffer normals;
        singleBuffer HitBuffer;
};