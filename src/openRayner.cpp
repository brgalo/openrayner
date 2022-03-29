
#include <iostream>
#include "util/host.hpp"
#include "vulkan/vulkan.hpp"


int main(int argc, char* argv[]) {
    std::cout<< "Hello Rayner!\n";
    Application app = {};
    app.initInstance();
    app.setupDevices();
}