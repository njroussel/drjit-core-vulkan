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

# define SPV_SUCCESS 0

using spv_result_t = int;
//using spv_context = struct spv_context_t*;
//using spv_const_context = const spv_context_t;
typedef struct spv_context_t spv_context_t;
typedef const spv_context_t* spv_const_context;
typedef spv_context_t* spv_context;

//typedef spv_context_t* spv_context;
//typedef const spv_context_t* spv_const_context;

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

typedef enum {
  SPV_ENV_UNIVERSAL_1_0,
  SPV_ENV_VULKAN_1_0,
  SPV_ENV_UNIVERSAL_1_1,
  SPV_ENV_OPENCL_2_1,
  SPV_ENV_OPENCL_2_2,
  SPV_ENV_OPENGL_4_0,
  SPV_ENV_OPENGL_4_1,
  SPV_ENV_OPENGL_4_2,
  SPV_ENV_OPENGL_4_3,
  SPV_ENV_OPENGL_4_5,
  SPV_ENV_UNIVERSAL_1_2,
  SPV_ENV_OPENCL_1_2,
  SPV_ENV_OPENCL_EMBEDDED_1_2,
  SPV_ENV_OPENCL_2_0,
  SPV_ENV_OPENCL_EMBEDDED_2_0,
  SPV_ENV_OPENCL_EMBEDDED_2_1,
  SPV_ENV_OPENCL_EMBEDDED_2_2,
  SPV_ENV_UNIVERSAL_1_3,
  SPV_ENV_VULKAN_1_1,
  SPV_ENV_WEBGPU_0,
  SPV_ENV_UNIVERSAL_1_4,
  SPV_ENV_VULKAN_1_1_SPIRV_1_4,
  SPV_ENV_UNIVERSAL_1_5,
  SPV_ENV_VULKAN_1_2,
  SPV_ENV_UNIVERSAL_1_6,
  SPV_ENV_VULKAN_1_3,
  SPV_ENV_MAX
} spv_target_env;

using spv_binary = spv_binary_t*;
using spv_diagnostic = spv_diagnostic_t*;

#if !defined(DR_VULKAN_SYM)
#  define DR_VULKAN_SYM(x) extern x;
#endif

// SPIR-V tools API
DR_VULKAN_SYM(spv_context (*spvContextCreate)(int));
DR_VULKAN_SYM(void (*spvContextDestroy)(spv_context context));
DR_VULKAN_SYM(spv_result_t (*spvTextToBinary)(const spv_const_context context,
                                              const char *,
                                              const size_t,
                                              spv_binary *, 
                                              spv_diagnostic *));
DR_VULKAN_SYM(void (*spvBinaryDestroy)(spv_binary binary));
DR_VULKAN_SYM(spv_result_t (*spvDiagnosticPrint)(const spv_diagnostic diagnostic));
DR_VULKAN_SYM(void (*spvDiagnosticDestroy)(spv_diagnostic diagnostic));

#if defined(DR_VULKAN_SYM)
#  undef DR_VULKAN_SYM
#endif
