add_library(device STATIC device.cpp device.hpp vkutils.hpp buffer.hpp)
target_include_directories(device PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")