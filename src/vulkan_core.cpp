#include "vulkan.h"

#include <iostream>

/// Try to load Vulkan
bool jitc_vulkan_init() {
    std::cout << "Loading Vulkan!" << std::endl;

    return true;
}

/// Free any resources allocated by jitc_vulkan_init()
void jitc_vulkan_shutdown(){
    std::cout << "Shutting Vulkan down!" << std::endl;
}
