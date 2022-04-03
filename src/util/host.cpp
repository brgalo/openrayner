#define TINYOBJLOADER_IMPLEMENTATION

#include "geometry.hpp"
#include "vulkan/vulkan.hpp"
#include "host.hpp"
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <locale>
#include <string>
#include <vector>
#include <vulkan/vulkan_core.h>








void Application::loadModel() {
    tinyobj::ObjReaderConfig conf;
    conf.mtl_search_path = mtlName;
    tinyobj::ObjReader reader;

    if (!reader.ParseFromFile(modelName, conf)) {
        if (!reader.Error().empty()) {
            std::cerr << "TinyObjReader: " << reader.Error();
        }
        exit(EXIT_FAILURE);
    } else {
        std::cout << "Model Loaded!\n";
        if (!reader.Warning().empty()) {
            std::cerr << "TinyObjReader: " << reader.Warning();
        }
    }

    uint32_t offset = 0;
    uint32_t currMeshNo = 0;
    std::vector<Mesh> meshes;
    size_t nVertices = reader.GetAttrib().GetVertices().size()/3;
    size_t nNormals = reader.GetAttrib().normals.size()/3;
    // copy like this to make sure, the data aligned correctly!
    geom.vertexBuf = convertVectorTovec3(reader.GetAttrib().GetVertices());
    // copy if file has normals
    if(nNormals == 0) {
        geom.normBuf = convertVectorTovec3(reader.GetAttrib().normals);
    }

    std::vector<glm::ivec3> idx;
    for (auto &shape : reader.GetShapes()) {
        // store mesh info
        meshes.push_back(Mesh(shape, currMeshNo, offset));

        // copy indices
        std::vector<glm::ivec3> currIdx = convertVectorToivec3(shape.mesh.indices);
        idx.insert(idx.end(),currIdx.begin(),currIdx.end());

        // calc normals, if not provided!

        // increment counters
        ++currMeshNo;
        offset += currIdx.size();
    }

    if (nNormals != 0) getNormals();
}


void Application::getNormals() {
    size_t i = 0;
    geom.normBuf.resize(geom.indices.size());

    for (auto &idx : geom.indices)
     {
        glm::vec3 A = glm::vec3(geom.vertexBuf.at(idx.x));
        glm::vec3 B = glm::vec3(geom.vertexBuf.at(idx.y));
        glm::vec3 C = glm::vec3(geom.vertexBuf.at(idx.z));

        geom.normBuf.at(i) = glm::cross(glm::cross(A, B),C);
         ++i;
     }
}

