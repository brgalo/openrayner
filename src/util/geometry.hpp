#pragma once

#include "memory"
#include <climits>
#include <cstdint>
#include <memory>
#include <vector>
#include <vulkan/vulkan_core.h>
#include "../../3rdParty/glm/glm/glm.hpp"


#include "../../3rdParty/tinyOBJ/tiny_obj_loader.h"

class Mesh {
    public:
    Mesh(const tinyobj::shape_t &shape, uint32_t off, uint32_t ID) : name(shape.name), id(ID),offset(off),n(shape.mesh.indices.size()/3) {};
    const std::string name;
    const uint32_t id;
    const uint32_t offset;
    const uint32_t n;
};


struct Geometry {
    Geometry() {};
    Geometry(std::vector<glm::vec3> &vertices,
             std::vector<glm::vec3> &normals,
             std::vector<glm::ivec3> &idx,
             std::vector<VkTransformMatrixKHR> & trafo,
             std::vector<Mesh> &m) : vertices(vertices),normals(normals),indices(idx),meshes(m),trafo(trafo) {};
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<glm::ivec3> indices;
    std::vector<VkTransformMatrixKHR> trafo;
    std::vector<Mesh> meshes;
};

glm::vec3 floatTovec3(const float* x);

std::vector<glm::vec3> convertVectorTovec3(const std::vector<tinyobj::real_t>&v);
std::vector<glm::ivec3> convertVectorToivec3(const std::vector<tinyobj::index_t>&v);