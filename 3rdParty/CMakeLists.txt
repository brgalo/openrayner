cmake_minimum_required(VERSION 3.10)

project(3rdParty LANGUAGES C CXX)

add_library(tinyobj INTERFACE)
set(TINYOBJ_DIR ${CMAKE_CURRENT_SOURCE_DIR/tinyOBJ})
target_sources(tinyobj INTERFACE ${TINYOBJ_DIR}/tiny_obj_loader.h)
#might need json for tinygltf
target_include_directories(tinyobj INTERFACE ${TINYOBJ_DIR})