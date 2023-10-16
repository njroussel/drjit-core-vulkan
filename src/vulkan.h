/*
    src/vulkan.h -- Vulkan backend functionality

    Copyright (c) 2023 Nicolas Roussel <nicolas.roussel@epfl.ch>

    All rights reserved. Use of this source code is governed by a BSD-style
    license that can be found in the LICENSE file.
*/

#pragma once

#include "vulkan_api.h"

/// Try to load Vulkan
extern bool jitc_vulkan_init();

/// Free any resources allocated by jitc_vulkan_init()
extern void jitc_vulkan_shutdown();
