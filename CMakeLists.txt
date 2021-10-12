cmake_minimum_required(VERSION 3.7 FATAL_ERROR)

set(NAME openRayner)
project (openRayner VERSION 0.0)

find_package(Vulkan REQUIRED)

##add_definitions(-DVK_USE_PLATFORM_XLIB_KHR)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(3rdParty)

add_executable(openRayner openRayner.cpp)
target_link_libraries(openRayner Vulkan::Vulkan)
target_link_libraries(openRayner glm)
target_link_libraries(openRayner tinyobj)