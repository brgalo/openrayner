add_library(host host.cpp host.hpp)
target_link_libraries(host PRIVATE Vulkan::Vulkan)