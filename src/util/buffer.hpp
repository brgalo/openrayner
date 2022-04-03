#pragma once
#include "vulkan/vulkan.h"
#include "vulkaninstance.hpp"
#include <vulkan/vulkan_core.h>

struct Buffer {
            Buffer() {};
            Buffer(VkDeviceSize SIZE,VkBufferUsageFlags usageFlags,VkMemoryPropertyFlags memFlags) : size(SIZE), usgFlags(usageFlags), memFlags(memFlags) {};
            VkBufferCreateInfo createInfo = {};
            VkMemoryRequirements memReq = {};
            VkDeviceSize size;
            VkBufferUsageFlags usgFlags;
            VkMemoryPropertyFlags memFlags;
            VkBuffer *buf;
            VkDeviceMemory buffMem;
            void create(const VulkanInstance &inst,
                                VkDeviceSize size,
                                VkBufferUsageFlags usageFlags,
                                VkMemoryPropertyFlags propFlags,
                                VkBuffer *buffer,
                                VkDeviceMemory *bufferMem);
};