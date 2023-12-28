#include "hash.h"
#include "tsl/robin_map.h"
#include "log.h"
#include "vulkan.h"

VkInstance jitc_vulkan_instance = nullptr;
VkDevice jitc_vulkan_device = nullptr;
VkQueue jitc_vulkan_queue = nullptr;
uint32_t jitc_vulkan_mem_type_idx = -1;
VkBufferMemMap jitc_vulkan_buffer_mem_map;

/// Find validation layers
static void jitc_add_validation_layer(std::vector<std::string> &layers) {
    uint32_t layer_count;
    vkEnumerateInstanceLayerProperties(&layer_count, nullptr);

    std::vector<VkLayerProperties> available_layers(layer_count);
    vkEnumerateInstanceLayerProperties(&layer_count, available_layers.data());

    const char* layer_name = "VK_LAYER_KHRONOS_validation";

    bool layer_found = false;
    for (const auto &layerProperties : available_layers) {
        if (strcmp(layer_name, layerProperties.layerName) == 0) {
            layer_found = true;
            break;
        }
    }

    if (!layer_found)
        jitc_log(Warn, "jitc_vulkan_init(): Validation layers were requested, "
                       "but they could not be found! Did you setup your Vulkan "
                       "SDK path correctly?");
    else
        layers.push_back(layer_name);
}

/// Try to load Vulkan
bool jitc_vulkan_init() {
    // First, dynamically load Vulkan into the process
    if (!jitc_vulkan_api_init())
        return false;

    jitc_log(Info, "jit_vulkan_init(): loaded API");


    ///
    /// Create instance
    ///
    VkApplicationInfo app_info{};
    app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    app_info.pApplicationName = "Dr.Jit";
    app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0); // TODO: insert drjit version
    app_info.apiVersion = VK_MAKE_API_VERSION(0, 1, 0, 0);

    VkInstanceCreateInfo create_info{};
    create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    create_info.pApplicationInfo = &app_info;
    std::vector<std::string> layers;
#if !defined(NDEBUG) // TODO: Make this a CMake Option
    jitc_add_validation_layer(layers);
#endif
    size_t layer_count = layers.size();
    std::vector<const char *> layer_names(layer_count);
    for (uint32_t i = 0; i < layers.size(); ++i)
        layer_names[i] = layers[i].c_str();
    create_info.enabledLayerCount = layer_count;
    create_info.ppEnabledLayerNames = layer_names.data();

    vulkan_check(vkCreateInstance(&create_info, nullptr, &jitc_vulkan_instance));

    ///
    /// Setup physical device
    ///
    uint32_t physical_device_count = 0;
    vulkan_check(vkEnumeratePhysicalDevices(jitc_vulkan_instance,
                                            &physical_device_count, nullptr));
    if (physical_device_count == 0) {
        jitc_log(
            LogLevel::Warn,
            "jit_vulkan_init(): No devices found -- disabling Vulkan backend!");
        return false;
    }

    std::vector<VkPhysicalDevice> physical_devices(physical_device_count);
    vulkan_check(vkEnumeratePhysicalDevices(
        jitc_vulkan_instance, &physical_device_count, physical_devices.data()));

    size_t physical_device_index = 0; // TODO: expose this as an envvar
    VkPhysicalDevice physical_device = physical_devices[physical_device_index];

    ///
    /// Find family queue
    ///
    uint32_t queue_family_count = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queue_family_count,
                                             nullptr);
    std::vector<VkQueueFamilyProperties> queue_families(queue_family_count);
    vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queue_family_count,
                                             queue_families.data());

    size_t queue_index = 0;
    bool found_queue = false;
    for (; queue_index < queue_family_count; ++queue_index) {
        if ((queue_families[queue_index].queueFlags & VK_QUEUE_GRAPHICS_BIT) &&
            (queue_families[queue_index].queueFlags & VK_QUEUE_COMPUTE_BIT)) {
            found_queue = true;
            break;
        }
    }
    if (!found_queue){
        // The Vulkan spec guarantees that this should never be the case, but hey...
        jitc_log(
            LogLevel::Warn,
            "jit_vulkan_init(): Could not find a vulkan queue family which "
            "supports both graphics and compute -- disabling Vulkan backend!");
        return false;
    }

    ///
    /// Create logical device and get queue
    ///
    VkDeviceQueueCreateInfo queue_create_info{};
    queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queue_create_info.queueFamilyIndex = queue_index;
    queue_create_info.queueCount = 1;
    float queue_priority = 1.0f;
    queue_create_info.pQueuePriorities = &queue_priority;

    VkPhysicalDeviceFeatures device_features{};

    VkDeviceCreateInfo device_create_info{};
    device_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    device_create_info.pQueueCreateInfos = &queue_create_info;
    device_create_info.queueCreateInfoCount = 1;
    device_create_info.pEnabledFeatures = &device_features;
    device_create_info.enabledExtensionCount = 0;

#if !defined(NDEBUG) // TODO: Make this a CMake Option
    device_create_info.enabledLayerCount =
        static_cast<uint32_t>(layers.size());
    device_create_info.ppEnabledLayerNames = layer_names.data();
#elif
    device_create_info.enabledLayerCount = 0;
#endif

    vulkan_check(vkCreateDevice(physical_device, &device_create_info, nullptr,
                                &jitc_vulkan_device));
    vkGetDeviceQueue(jitc_vulkan_device, queue_index, 0, &jitc_vulkan_queue);

    ///
    /// Find memory type index
    ///
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(physical_device, &memProperties);

    uint32_t necessary_flags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                               VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;

    auto has_all_flags = [](uint32_t input, uint32_t flags) {
      return (input & flags) == flags;
    };

    for (uint32_t i = 0; i < memProperties.memoryTypeCount; ++i) {
        if (has_all_flags(memProperties.memoryTypes[i].propertyFlags,
                          necessary_flags)) {
            jitc_vulkan_mem_type_idx = memProperties.memoryTypes[i].heapIndex;
            break;
        }
    }

    if (jitc_vulkan_mem_type_idx == (uint32_t) -1) {
        jitc_log(LogLevel::Warn,
                 "jit_vulkan_init(): Could not find a suitable memory type on "
                 "discrete GPU -- disabling Vulkan backend!");
        return false;
    }

    return true;
}

/// Free any resources allocated by jitc_vulkan_init()
void jitc_vulkan_shutdown(){
    jitc_log(Info, "jit_vulkan_shutdown()");

    #define Z(x) x = nullptr
    vkDestroyDevice(jitc_vulkan_device, nullptr);
    Z(jitc_vulkan_device);

    vkDestroyInstance(jitc_vulkan_instance, nullptr);
    Z(jitc_vulkan_instance);
    #undef Z

    jitc_vulkan_api_shutdown();
}

void vulkan_check_impl(VkResult errval, const char *file, const int line) {
    if (unlikely(errval != VK_SUCCESS)) {
        const char *name = nullptr, *msg = nullptr;
        //cuGetErrorName(errval, &name);
        //cuGetErrorString(errval, &msg);
        jitc_fail("vulkan_check(): API error %04i (%s): \"%s\" in "
                  "%s:%i.", (int) errval, name, msg, file, line);
    }
}
