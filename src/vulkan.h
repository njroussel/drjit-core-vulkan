/*
    src/vulkan.h -- Vulkan backend functionality

    Copyright (c) 2023 Nicolas Roussel <nicolas.roussel@epfl.ch>

    All rights reserved. Use of this source code is governed by a BSD-style
    license that can be found in the LICENSE file.
*/

#pragma once

#include "hash.h"
#include "tsl/robin_map.h"
#include "vulkan_api.h"
#include "vulkan_spirv_api.h"

// Forward declarations
struct Kernel;

struct VkDeviceMemoryHasher {
    size_t operator()(VkDeviceMemory ptr) const {
        return (size_t) hasher((uint64_t) ptr);
    }

    UInt64Hasher hasher;
};

using VkBufferMemMap = tsl::robin_map<VkDeviceMemory, VkBuffer, VkDeviceMemoryHasher>;

extern VkInstance jitc_vulkan_instance;
extern VkDevice jitc_vulkan_device;
extern VkQueue jitc_vulkan_queue;
extern uint32_t jitc_vulkan_mem_type_idx;
extern VkBufferMemMap jitc_vulkan_buffer_mem_map;
extern VkCommandPool jitc_vulkan_cmd_pool;
extern VkSemaphore jitc_vulkan_semaphore;
extern spv_diagnostic jitc_vulkan_spv_diagnostic;

/// Try to load Vulkan
extern bool jitc_vulkan_init();

/// Free any resources allocated by jitc_vulkan_init()
extern void jitc_vulkan_shutdown();

/// Assert that a Vulkan operation is correctly issued
#define vulkan_check(err) vulkan_check_impl(err, __FILE__, __LINE__)
extern void vulkan_check_impl(VkResult errval, const char *file, const int line);

/// Create a semaphore
extern VkSemaphore jitc_vulkan_create_semaphore();

/// Compile
extern void jitc_vulkan_compile(Kernel &kernel);

/// Assert that a SPIR-V tools operation is correctly issued
#define spv_check(err) spv_check_impl(err, __FILE__, __LINE__)
extern void spv_check_impl(spv_result_t errval, const char *file, const int line);
