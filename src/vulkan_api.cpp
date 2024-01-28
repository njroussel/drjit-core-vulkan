/*
    src/vulkan_api.cpp -- Low-level interface to Vulkan API

    Copyrighk (c) 2023 Nicolas Roussel <nicolas.roussel@epfl.ch>

    All rights reserved. Use of this source code is governed by a BSD-style
    license that can be found in the LICENSE file.
*/

#define DR_VULKAN_SYM(...) __VA_ARGS__ = nullptr;

#include "vulkan_api.h"
#include "log.h"
#include "internal.h"

#if defined(_WIN32)
#  include <windows.h>
#else
#  include <dlfcn.h>
#endif

static void *jitc_vulkan_handle = nullptr;

bool jitc_vulkan_api_init() {
    if (jitc_vulkan_handle)
        return true;

#  if defined(_WIN32)
    const char* vulkan_fname = "vulkan-1.dll", // TODO check me
              * vulkan_glob = nullptr;
#  elif defined(__linux__)
    const char *vulkan_fname  = "libvulkan.so",
               *vulkan_glob   = "/usr/lib/{x86_64-linux-gnu,aarch64-linux-gnu}/libvulkan.so.*";
#  else
    // TODO Lookup MoltenVK support
    //const char *cuda_fname  = "libcuda.dylib",
    //           *cuda_glob   = cuda_fname;
#  endif

#  if !defined(_WIN32)
    // Don't dlopen libvulkan.so if it was loaded by another library
    if (dlsym(RTLD_NEXT, "vkCreateInstance")) // TODO maybe a better symbol ?
        jitc_vulkan_handle = RTLD_NEXT;
#  endif

    if (!jitc_vulkan_handle) {
        jitc_vulkan_handle = jitc_find_library(vulkan_fname, vulkan_glob, "DRJIT_LIBVULKAN_PATH");

        if (!jitc_vulkan_handle) // Vulkan library cannot be loaded, give up
            return false;
    }

    const char *symbol = nullptr;

    do {
        #define LOAD(name, ...)                                      \
            symbol = strlen(__VA_ARGS__ "") > 0                      \
                ? (#name "_" __VA_ARGS__) : #name;                   \
            name = decltype(name)(dlsym(jitc_vulkan_handle, symbol));  \
            if (!name)                                               \
                break;                                               \
            symbol = nullptr

        LOAD(vkCreateInstance);
        LOAD(vkDestroyInstance);
        LOAD(vkEnumerateInstanceLayerProperties);
        LOAD(vkEnumeratePhysicalDevices);
        LOAD(vkGetPhysicalDeviceQueueFamilyProperties);
        LOAD(vkCreateDevice);
        LOAD(vkDestroyDevice);
        LOAD(vkGetDeviceQueue);
        LOAD(vkCreateBuffer);
        LOAD(vkDestroyBuffer);
        LOAD(vkGetPhysicalDeviceMemoryProperties);
        LOAD(vkAllocateMemory);
        LOAD(vkBindBufferMemory);
        LOAD(vkFreeMemory);
        LOAD(vkMapMemory);
        LOAD(vkUnmapMemory);
        LOAD(vkCreateCommandPool);
        LOAD(vkDestroyCommandPool);
        LOAD(vkAllocateCommandBuffers);
        LOAD(vkFreeCommandBuffers);
        LOAD(vkBeginCommandBuffer);
        LOAD(vkEndCommandBuffer);
        LOAD(vkQueueSubmit);
        LOAD(vkCmdCopyBuffer);
        LOAD(vkCreateFence);
        LOAD(vkDestroyFence);
        LOAD(vkWaitForFences);
        LOAD(vkCreateSemaphore);
        LOAD(vkDestroySemaphore);

        #undef LOAD
    } while (false);

    if (symbol) {
        jitc_vulkan_api_shutdown();
        jitc_log(LogLevel::Warn,
                "jit_vulkan_api_init(): could not find symbol \"%s\" -- disabling "
                "Vulkan backend!", symbol);
        return false;
    }

    return true;
}

void jitc_vulkan_api_shutdown() {
    if (!jitc_vulkan_handle)
        return;

    #define Z(x) x = nullptr
    Z(vkCreateInstance);
    Z(vkDestroyInstance);
    Z(vkEnumerateInstanceLayerProperties);
    Z(vkEnumeratePhysicalDevices);
    Z(vkGetPhysicalDeviceQueueFamilyProperties);
    Z(vkCreateDevice);
    Z(vkDestroyDevice);
    Z(vkGetDeviceQueue);
    Z(vkCreateBuffer);
    Z(vkDestroyBuffer);
    Z(vkGetPhysicalDeviceMemoryProperties);
    Z(vkAllocateMemory);
    Z(vkBindBufferMemory);
    Z(vkFreeMemory);
    Z(vkMapMemory);
    Z(vkUnmapMemory);
    Z(vkCreateCommandPool);
    Z(vkDestroyCommandPool);
    Z(vkAllocateCommandBuffers);
    Z(vkFreeCommandBuffers);
    Z(vkBeginCommandBuffer);
    Z(vkEndCommandBuffer);
    Z(vkQueueSubmit);
    Z(vkCmdCopyBuffer);
    Z(vkCreateFence);
    Z(vkDestroyFence);
    Z(vkWaitForFences);
    Z(vkCreateSemaphore);
    Z(vkDestroySemaphore);
    #undef Z

#if !defined(_WIN32)
    if (jitc_vulkan_handle != RTLD_NEXT)
        dlclose(jitc_vulkan_handle);
#else
    FreeLibrary((HMODULE) jitc_vulkan_handle);
#endif

    jitc_vulkan_handle = nullptr;
}

void *jitc_vulkan_lookup(const char *name) {
    void *ptr = dlsym(jitc_vulkan_handle, name);
    if (!ptr)
        jitc_raise("jit_vulkan_lookup(): function \"%s\" not found!", name);
    return ptr;
}
