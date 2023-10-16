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

# define VK_STRUCTURE_TYPE_APPLICATION_INFO 0

using VkResult   = int;
using VkInstance = struct VkInstance_st *;

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

#if !defined(DR_VULKAN_SYM)
#  define DR_VULKAN_SYM(x) extern x;
#endif

// Driver API
DR_VULKAN_SYM(VkResult (*vkCreateInstance)(const VkInstanceCreateInfo *,
                                           const VkAllocationCallbacks *,
                                           VkInstance *));

#if defined(DR_VULKAN_SYM)
#  undef DR_VULKAN_SYM
#endif
