add_library(host host.cpp host.hpp geometry.hpp geometry.cpp buffers.hpp buffers.cpp vulkaninstance.hpp vulkaninstance.cpp)
target_link_libraries(host INTERFACE tinyobj)
target_link_libraries(host PRIVATE Vulkan::Vulkan)