cmake_minimum_required(VERSION 3.10)

project(3rdParty LANGUAGES C CXX)


# glm
add_library(glm INTERFACE)
set(GLM_DIR ${CMAKE_CURRENT_SOURCE_DIR}/glm)
target_sources(glm INTERFACE ${GLM_DIR}/glm/glm.hpp)
target_include_directories(glm INTERFACE ${GLM_DIR})

# tinyobj
add_library(tinyobj INTERFACE)
set(TINYOBJ_DIR ${CMAKE_CURRENT_SOURCE_DIR}/tinyOBJ)
target_sources(tinyobj INTERFACE ${TINYOBJ_DIR}/tiny_obj_loader.h)
target_include_directories(tinyobj INTERFACE ${TINYGLTF_DIR})