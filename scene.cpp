#include "scene.hpp"

bool Scene::loadFromGltf(std::string fileName, tinygltf::Model &model) {
    tinygltf::TinyGLTF loader;
    std::string err, warn;

    bool NO_ERROR = loader.LoadASCIIFromFile(&model, &err, &warn, fileName);

    if(!err.empty()) {
        std::cout << "The following warning(s) occured during loading: " << std::endl;
        std::cout << err;
    }

    if(!warn.empty()) {
        std::cout << "The following warning(s) occured during loading: " << std::endl;
        std::cout << warn;
    }

    return NO_ERROR;
};

void Scene::loadFromFile(std::string fileName) {
    
    // loads file from GLTF
    tinygltf::Model model;
    loadFromGltf(fileName, model);


}

void Scene::loadScene() {

}