#include "string"
#include "memory"
#include "iostream"
#include "vector"
#include "array"
#include "./3rdParty/tinyGLTF/tiny_gltf.h"
#include "vulkan/vulkan.h"
//#include "glm/glm.hpp"

class Scene {
    bool loadFromGltf(std::string fileName, tinygltf::Model &model);
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
};

class geometricData {

};

