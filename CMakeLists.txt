cmake_minimum_required(VERSION 3.7 FATAL_ERROR)
cmake_policy(VERSION 2.8)

##set(NAME openRayner)
project (openRayner)

find_package(Vulkan REQUIRED)

##add_definitions(-DVK_USE_PLATFORM_XLIB_KHR)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(openRayner openRayner.cpp)
target_link_libraries(openRayner Vulkan::Vulkan)