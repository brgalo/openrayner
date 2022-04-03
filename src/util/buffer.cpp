#include "host.hpp"
#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan_core.h>
#include "buffer.hpp"
#include "vulkaninstance.hpp"
#include "iostream"

void Buffer::create(const VulkanInstance &inst,
                            VkDeviceSize size,
                            VkBufferUsageFlags usageFlags,s
                            VkMemoryPropertyFlags propFlags,
                            VkBuffer *buffer,
                            VkDeviceMemory *bufferMem) {
    createInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    //createInfo.flags = 
    createInfo.size = size;
    createInfo.usage = usageFlags;
    createInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    
    if (vkCreateBuffer(inst.logicalDevHandle, &createInfo, NULL, buffer) == VK_SUCCESS) {
        std::cout << "created buffer with size " << size << "\n";
    }

    vkGetBufferMemoryRequirements(inst.logicalDevHandle, *buffer, &memReq);
};
