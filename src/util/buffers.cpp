#include "host.hpp"
#include <cstdint>
#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan_core.h>
#include "buffers.hpp"
#include "vulkaninstance.hpp"
#include "iostream"

void singleBuffer::create(const VulkanInstance &inst) {
    createInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    //createInfo.flags = 
    createInfo.size = size;
    createInfo.usage = usgFlags;
    createInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    
    if (vkCreateBuffer(inst.logicalDevHandle, &createInfo, NULL, &bufferHost) == VK_SUCCESS) {
        std::cout << "created buffer with size " << size << "\n";
    }

    // fetch avialable memory
    vkGetBufferMemoryRequirements(inst.logicalDevHandle, bufferHost, &memReq);

    VkMemoryAllocateInfo memAlloqInfo = {};
    memAlloqInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    memAlloqInfo.allocationSize = size;

    // find suitable memory
    uint32_t memTypIdx = -1;
    for (int x = 0; x < inst.getMemProps().memoryTypeCount; ++x) {
        if ((memReq.memoryTypeBits & (1 << x)) && 
            (inst.getMemProps().memoryTypes[x].propertyFlags & memFlags) == memFlags) {
                memTypIdx = x;
                break;
        }
    }
    memAlloqInfo.memoryTypeIndex = memTypIdx;

    if(vkAllocateMemory(inst.logicalDevHandle, &memAlloqInfo, NULL, &bufferMemoryDevice) == VK_SUCCESS) {
        std::cout << "Allocated Buffer memory\n";
    }

    vkBindBufferMemory(inst.logicalDevHandle, bufferHost, bufferMemoryDevice, 0);
};

void singleBuffer::copyBuffer(const VulkanInstance &inst, VkBuffer from, VkBuffer to) {
    VkCommandBufferAllocateInfo bufAlocInfo = {};
    bufAlocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    bufAlocInfo.pNext = 0;
    bufAlocInfo.commandPool = inst.getCmdPool();
    bufAlocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    bufAlocInfo.commandBufferCount = 1;
    
    VkCommandBuffer cmdBuf;
    vkAllocateCommandBuffers(inst.logicalDevHandle, &bufAlocInfo, &cmdBuf);

    VkCommandBufferBeginInfo cmdBufBeginInfo = {};
    cmdBufBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    cmdBufBeginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    vkBeginCommandBuffer(cmdBuf, &cmdBufBeginInfo);
    VkBufferCopy bufferCopy = {};
    bufferCopy.size = size;
    vkCmdCopyBuffer(cmdBuf, from, to, 1, &bufferCopy);

    VkSubmitInfo subInfo = {};
    subInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    subInfo.commandBufferCount = 1;
    subInfo.pCommandBuffers = &cmdBuf;

    vkQueueSubmit(inst.getGraphicsQ(),1, &subInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(inst.getGraphicsQ());

    vkFreeCommandBuffers(inst.logicalDevHandle, inst.getCmdPool(), 1, &cmdBuf);
}

void singleBuffer::copyH2D(const VulkanInstance &inst,const void* const src) {
    // create Staging Buffer
    StagingBuffer stagingBuf = StagingBuffer(*this, src);
    stagingBuf.create(inst);

    // map and copy data
    vkMapMemory(inst.logicalDevHandle,stagingBuf.bufferMemoryDevice , 0, size, 0,&stagingBuf.data);
    memcpy(stagingBuf.data,src,size);
    vkUnmapMemory(inst.logicalDevHandle,stagingBuf.bufferMemoryDevice);
    
    // create final buffer
    this->create(inst);
    this->copyBuffer(inst, stagingBuf.bufferHost, this->bufferHost);

    vkDestroyBuffer(inst.logicalDevHandle,stagingBuf.bufferHost,NULL);
    vkFreeMemory(inst.logicalDevHandle,stagingBuf.bufferMemoryDevice,NULL);
}

uint64_t singleBuffer::getDeviceAdress(const VulkanInstance &inst) {
    VkBufferDeviceAddressInfo adressInfo = {};
    adressInfo.sType = VK_STRUCTURE_TYPE_BUFFER_DEVICE_ADDRESS_INFO;
    adressInfo.buffer = bufferHost;
    return vkGetBufferDeviceAddress(inst.logicalDevHandle, &adressInfo);
}