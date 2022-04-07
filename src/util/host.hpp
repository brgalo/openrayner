#pragma once


#include "vulkan/vulkan.hpp"
#include <cstdint>
#include <vector>
#include <vulkan/vulkan_core.h>
#include "../../3rdParty/glm/glm/glm.hpp"
#include "geometry.hpp"
#include "vulkaninstance.hpp"
#include "buffers.hpp"


class Application {
    public:
    void loadModel();
    VulkanInstance vkIns = {};
    Buffers buffers;
    Geometry geom;
    private:
    std::string absDir = "/HOME/s409158/projects/openRayner/";
    std::string modelName = absDir+"testGeometry/plane.obj";
    std::string mtlName = absDir+"testGeometry/";


    void getNormals();
    
};