#pragma once

#include "vulkan/vulkan.hpp"

class application {
    public:
    void init();
    private:
    VkInstance instance = {};
    VkApplicationInfo applicationInfo = {};
    VkInstanceCreateInfo instanceCreateInfo = {};
};