/*
    src/vulkan_api.h -- Low-level interface to Vulkan API

    Copyrighk (c) 2023 Nicolas Roussel <nicolas.roussel@epfl.ch>

    All rights reserved. Use of this source code is governed by a BSD-style
    license that can be found in the LICENSE file.
*/

#pragma once

#include <drjit-core/jit.h>

/// Try to resolve the Vulkan API functions
extern bool jitc_vulkan_spirv_api_init();

/// Free any resources allocated by jitc_vulkan_spirv_api_init()
extern void jitc_vulkan_spirv_api_shutdown();

/// Look up a device driver function
extern void *jitc_vulkan_spirv_lookup(const char *name);

using spv_context = struct spv_context_t*;
using spv_const_context = const spv_context_t;

struct spv_binary_t {
  uint32_t* code;
  size_t wordCount;
};

struct spv_position_t {
  size_t line;
  size_t column;
  size_t index;
};

struct spv_diagnostic_t {
  spv_position_t position;
  char* error;
  bool isTextSource;
};

using spv_binary = spv_binary_t*;
using spv_diagnostic = spv_diagnostic_t*;

#if !defined(DR_VULKAN_SYM)
#  define DR_VULKAN_SYM(x) extern x;
#endif

// SPIR-V tools API
DR_VULKAN_SYM(spv_context (*spvContextCreate)(int));
DR_VULKAN_SYM(uint32_t (*spvTextToBinary)(const spv_const_context,
                                          const char*,
                                          const size_t,
                                          spv_binary*,
                                          spv_diagnostic*));

#if defined(DR_VULKAN_SYM)
#  undef DR_VULKAN_SYM
#endif
