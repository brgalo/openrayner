
add_subdirectory(util)

add_executable(openRayner openRayner.cpp)
target_link_libraries(openRayner PRIVATE Vulkan::Vulkan)
target_link_libraries(openRayner PRIVATE host)