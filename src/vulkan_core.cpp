#include "vulkan.h"
#include "log.h"

/// Try to load Vulkan
bool jitc_vulkan_init() {
    // First, dynamically load Vulkan into the process
    if (!jitc_vulkan_api_init())
        return false;

    jitc_log(Info, "jit_vulkan_init(): loaded API");

    return true;
}

/// Free any resources allocated by jitc_vulkan_init()
void jitc_vulkan_shutdown(){
    jitc_log(Info, "jit_vulkan_shutdown()");
    jitc_vulkan_api_shutdown();
}
