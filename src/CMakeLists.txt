
add_subdirectory(util)

add_executable(openRayner openRayner.cpp )
target_link_libraries(openRayner PRIVATE Vulkan::Vulkan)
target_link_libraries(openRayner PRIVATE tinyobj)
target_link_libraries(openRayner PUBLIC host)