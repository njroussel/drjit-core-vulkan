/*
    src/vulkan.h -- Vulkan backend functionality

    Copyright (c) 2023 Nicolas Roussel <nicolas.roussel@epfl.ch>

    All rights reserved. Use of this source code is governed by a BSD-style
    license that can be found in the LICENSE file.
*/

#pragma once

#include "vulkan_api.h"

extern VkInstance jitc_vulkan_instance;
extern VkDevice jitc_vulkan_device;
extern VkQueue jitc_vulkan_queue;


/// Try to load Vulkan
extern bool jitc_vulkan_init();

/// Free any resources allocated by jitc_vulkan_init()
extern void jitc_vulkan_shutdown();

/// Assert that a Vulkan operation is correctly issued
#define vulkan_check(err) vulkan_check_impl(err, __FILE__, __LINE__)
extern void vulkan_check_impl(VkResult errval, const char *file, const int line);
