/*
    src/vulkan_api.h -- Low-level interface to Vulkan API

    Copyrighk (c) 2023 Nicolas Roussel <nicolas.roussel@epfl.ch>

    All rights reserved. Use of this source code is governed by a BSD-style
    license that can be found in the LICENSE file.
*/

#pragma once

#include <drjit-core/jit.h>

/// Try to resolve the Vulkan API functions
extern bool jitc_vulkan_api_init();

/// Free any resources allocated by jitc_vulkan_api_init()
extern void jitc_vulkan_api_shutdown();

/// Look up a device driver function
extern void *jitc_vulkan_lookup(const char *name);

# define VK_MAKE_VERSION(major, minor, patch) \
    ((((uint32_t)(major)) << 22U) | (((uint32_t)(minor)) << 12U) | ((uint32_t)(patch)))
#define VK_MAKE_API_VERSION(variant, major, minor, patch) \
    ((((uint32_t)(variant)) << 29U) | (((uint32_t)(major)) << 22U) | (((uint32_t)(minor)) << 12U) | ((uint32_t)(patch)))

# define VK_STRUCTURE_TYPE_APPLICATION_INFO          0
# define VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO      1
# define VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO  2
# define VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO        3
# define VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO      5
# define VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO       12

# define VK_MAX_EXTENSION_NAME_SIZE 256U
# define VK_MAX_DESCRIPTION_SIZE    256U

# define VK_QUEUE_GRAPHICS_BIT   0x00000001
# define VK_QUEUE_COMPUTE_BIT    0x00000002

# define VK_BUFFER_USAGE_TRANSFER_SRC_BIT   0x00000001
# define VK_BUFFER_USAGE_TRANSFER_DST_BIT   0x00000002
# define VK_BUFFER_USAGE_STORAGE_BUFFER_BIT 0x00000020

# define VK_SHARING_MODE_EXCLUSIVE 0

#define VK_MAX_MEMORY_TYPES 32U
#define VK_MAX_MEMORY_HEAPS 16U

#define VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT  0x00000002
#define VK_MEMORY_PROPERTY_HOST_COHERENT_BIT 0x00000004

# define VK_SUCCESS 0

using VkResult = int;
using VkInstance = struct VkInstance_st *;
using VkPhysicalDevice = struct VkPhysicalDevice_st *;
using VkDevice = struct VkDevice_st *;
using VkQueue = struct VkQueue_st *;
using VkBuffer = struct VkBuffer_st *;
using VkDeviceSize = uint64_t;
using VkDeviceMemory = struct VkDeviceMemory_st *;

using PFN_vkAllocationFunction =
    void *(*)(void *, uintptr_t, unsigned, unsigned, const char *);
using PFN_vkReallocationFunction =
    void *(*)(void *, void *, size_t, size_t, int);
using PFN_vkFreeFunction =
    void (*)(void *, void *);
using PFN_vkInternalAllocationNotification =
    void (*)(void *, size_t, int, int);
using PFN_vkInternalFreeNotification =
    void (*)(void *, size_t, int, int);

struct VkApplicationInfo {
    int         sType;
    const void* pNext;
    const char* pApplicationName;
    uint32_t    applicationVersion;
    const char* pEngineName;
    uint32_t    engineVersion;
    uint32_t    apiVersion;
};

struct VkInstanceCreateInfo {
    int                       sType;
    const void*               pNext;
    int                       flags;
    const VkApplicationInfo*  pApplicationInfo;
    uint32_t                  enabledLayerCount;
    const char* const*        ppEnabledLayerNames;
    uint32_t                  enabledExtensionCount;
    const char* const*        ppEnabledExtensionNames;
};

struct VkAllocationCallbacks {
    void*                                   pUserData;
    PFN_vkAllocationFunction                pfnAllocation;
    PFN_vkReallocationFunction              pfnReallocation;
    PFN_vkFreeFunction                      pfnFree;
    PFN_vkInternalAllocationNotification    pfnInternalAllocation;
    PFN_vkInternalFreeNotification          pfnInternalFree;
};

struct VkLayerProperties {
    char        layerName[VK_MAX_EXTENSION_NAME_SIZE];
    uint32_t    specVersion;
    uint32_t    implementationVersion;
    char        description[VK_MAX_DESCRIPTION_SIZE];
};

struct VkExtent3D {
    uint32_t    width;
    uint32_t    height;
    uint32_t    depth;
};

struct VkQueueFamilyProperties {
    uint32_t      queueFlags;
    uint32_t      queueCount;
    uint32_t      timestampValidBits;
    VkExtent3D    minImageTransferGranularity;
};

struct VkDeviceQueueCreateInfo {
    int           sType;
    const void*   pNext;
    int           flags;
    uint32_t      queueFamilyIndex;
    uint32_t      queueCount;
    const float*  pQueuePriorities;
};

struct VkPhysicalDeviceFeatures {
    uint32_t robustBufferAccess;
    uint32_t fullDrawIndexUint32;
    uint32_t imageCubeArray;
    uint32_t independentBlend;
    uint32_t geometryShader;
    uint32_t tessellationShader;
    uint32_t sampleRateShading;
    uint32_t dualSrcBlend;
    uint32_t logicOp;
    uint32_t multiDrawIndirect;
    uint32_t drawIndirectFirstInstance;
    uint32_t depthClamp;
    uint32_t depthBiasClamp;
    uint32_t fillModeNonSolid;
    uint32_t depthBounds;
    uint32_t wideLines;
    uint32_t largePoints;
    uint32_t alphaToOne;
    uint32_t multiViewport;
    uint32_t samplerAnisotropy;
    uint32_t textureCompressionETC2;
    uint32_t textureCompressionASTC_LDR;
    uint32_t textureCompressionBC;
    uint32_t occlusionQueryPrecise;
    uint32_t pipelineStatisticsQuery;
    uint32_t vertexPipelineStoresAndAtomics;
    uint32_t fragmentStoresAndAtomics;
    uint32_t shaderTessellationAndGeometryPointSize;
    uint32_t shaderImageGatherExtended;
    uint32_t shaderStorageImageExtendedFormats;
    uint32_t shaderStorageImageMultisample;
    uint32_t shaderStorageImageReadWithoutFormat;
    uint32_t shaderStorageImageWriteWithoutFormat;
    uint32_t shaderUniformBufferArrayDynamicIndexing;
    uint32_t shaderSampledImageArrayDynamicIndexing;
    uint32_t shaderStorageBufferArrayDynamicIndexing;
    uint32_t shaderStorageImageArrayDynamicIndexing;
    uint32_t shaderClipDistance;
    uint32_t shaderCullDistance;
    uint32_t shaderFloat64;
    uint32_t shaderInt64;
    uint32_t shaderInt16;
    uint32_t shaderResourceResidency;
    uint32_t shaderResourceMinLod;
    uint32_t sparseBinding;
    uint32_t sparseResidencyBuffer;
    uint32_t sparseResidencyImage2D;
    uint32_t sparseResidencyImage3D;
    uint32_t sparseResidency2Samples;
    uint32_t sparseResidency4Samples;
    uint32_t sparseResidency8Samples;
    uint32_t sparseResidency16Samples;
    uint32_t sparseResidencyAliased;
    uint32_t variableMultisampleRate;
    uint32_t inheritedQueries;
};

struct VkDeviceCreateInfo {
    int sType;
    const void*                        pNext;
    uint32_t                           flags;
    uint32_t                           queueCreateInfoCount;
    const VkDeviceQueueCreateInfo*     pQueueCreateInfos;
    uint32_t                           enabledLayerCount;
    const char* const*                 ppEnabledLayerNames;
    uint32_t                           enabledExtensionCount;
    const char* const*                 ppEnabledExtensionNames;
    const VkPhysicalDeviceFeatures*    pEnabledFeatures;
};

struct VkBufferCreateInfo {
    int               sType;
    const void*       pNext;
    uint32_t          flags;
    uint64_t          size;
    uint32_t          usage;
    int               sharingMode;
    uint32_t          queueFamilyIndexCount;
    const uint32_t*   pQueueFamilyIndices;
};

typedef struct VkMemoryType {
    uint32_t propertyFlags;
    uint32_t heapIndex;
} VkMemoryType;

typedef struct VkMemoryHeap {
    VkDeviceSize  size;
    uint32_t      flags;
} VkMemoryHeap;


struct VkPhysicalDeviceMemoryProperties {
    uint32_t        memoryTypeCount;
    VkMemoryType    memoryTypes[VK_MAX_MEMORY_TYPES];
    uint32_t        memoryHeapCount;
    VkMemoryHeap    memoryHeaps[VK_MAX_MEMORY_HEAPS];
};

struct VkPhysicalDeviceMemoryProperties2 {
    int                                 sType;
    void*                               pNext;
    VkPhysicalDeviceMemoryProperties    memoryProperties;
};

struct VkMemoryAllocateInfo {
    int           sType;
    const void*   pNext;
    VkDeviceSize  allocationSize;
    uint32_t      memoryTypeIndex;
} ;


#if !defined(DR_VULKAN_SYM)
#  define DR_VULKAN_SYM(x) extern x;
#endif

// Driver API
DR_VULKAN_SYM(VkResult (*vkCreateInstance)(const VkInstanceCreateInfo *,
                                           const VkAllocationCallbacks *,
                                           VkInstance *));
DR_VULKAN_SYM(void (*vkDestroyInstance)(VkInstance,
                                        const VkAllocationCallbacks *));
DR_VULKAN_SYM(VkResult (*vkEnumerateInstanceLayerProperties)(
    uint32_t *, VkLayerProperties *));
DR_VULKAN_SYM(VkResult (*vkEnumeratePhysicalDevices)(VkInstance, uint32_t *,
                                                     VkPhysicalDevice *));
DR_VULKAN_SYM(void (*vkGetPhysicalDeviceQueueFamilyProperties)( VkPhysicalDevice, uint32_t*, VkQueueFamilyProperties*));
DR_VULKAN_SYM(VkResult (*vkCreateDevice)(VkPhysicalDevice,
                                         const VkDeviceCreateInfo *,
                                         const VkAllocationCallbacks *,
                                         VkDevice *));
DR_VULKAN_SYM(void (*vkDestroyDevice)(VkDevice, const VkAllocationCallbacks *));
DR_VULKAN_SYM(void (*vkGetDeviceQueue)(VkDevice, uint32_t, uint32_t,
                                       VkQueue *));
DR_VULKAN_SYM(VkResult (*vkCreateBuffer)(VkDevice, const VkBufferCreateInfo *,
                                         const VkAllocationCallbacks *,
                                         VkBuffer *));
DR_VULKAN_SYM(void (*vkDestroyBuffer)(VkDevice,
                                      VkBuffer,
                                      const VkAllocationCallbacks*));
DR_VULKAN_SYM(void (*vkGetPhysicalDeviceMemoryProperties)(VkPhysicalDevice,
                                                          VkPhysicalDeviceMemoryProperties*));
DR_VULKAN_SYM(VkResult (*vkAllocateMemory)(VkDevice,
                                           const VkMemoryAllocateInfo*,
                                           const VkAllocationCallbacks*,
                                           VkDeviceMemory*));
DR_VULKAN_SYM(VkResult (*vkBindBufferMemory)(VkDevice,
                                             VkBuffer,
                                             VkDeviceMemory,
                                             VkDeviceSize));
DR_VULKAN_SYM(void (*vkFreeMemory)(VkDevice,
                                   VkDeviceMemory,
                                   const VkAllocationCallbacks *));

#if defined(DR_VULKAN_SYM)
#  undef DR_VULKAN_SYM
#endif
