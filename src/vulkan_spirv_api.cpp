/*
    src/vulkan_api.cpp -- Low-level interface to Vulkan API

    Copyrighk (c) 2023 Nicolas Roussel <nicolas.roussel@epfl.ch>

    All rights reserved. Use of this source code is governed by a BSD-style
    license that can be found in the LICENSE file.
*/

#define DR_VULKAN_SYM(...) __VA_ARGS__ = nullptr;

#include "vulkan_spirv_api.h"
#include "log.h"
#include "internal.h"

#if defined(_WIN32)
#  include <windows.h>
#else
#  include <dlfcn.h>
#endif

static void *jitc_vulkan_spirv_handle = nullptr;

bool jitc_vulkan_spirv_api_init() {
    if (jitc_vulkan_spirv_handle)
        return true;

#  if defined(_WIN32)
    const char* spirv_fname = "libSPIRV-Tools.dll", // TODO check me
              * spirv_glob = nullptr;
#  elif defined(__linux__)
    const char *spirv_fname  = "libSPIRV-Tools.so",
               *spirv_glob   = "/usr/lib/{x86_64-linux-gnu,aarch64-linux-gnu}/libSPIRV-Tools.so.*";
#  else
    const char *spirv_fname  = "libSPIRV-Tools.so",
    //           *spirv_glob   = spirv_fname; //TODO Lookup path on OS X
#  endif

#  if !defined(_WIN32)
    // Don't dlopen libvulkan.so if it was loaded by another library
    if (dlsym(RTLD_NEXT, "spvTextToBinary"))
        jitc_vulkan_spirv_handle = RTLD_NEXT;
#  endif

    if (!jitc_vulkan_spirv_handle) {
        jitc_vulkan_spirv_handle = jitc_find_library(spirv_fname, spirv_glob, "DRJIT_LIBSPIRVTOOLS_PATH");

        if (!jitc_vulkan_spirv_handle) // SPIR-V tools library cannot be loaded, give up
            return false;
    }

    const char *symbol = nullptr;

    do {
        #define LOAD(name, ...)                                              \
            symbol = strlen(__VA_ARGS__ "") > 0                              \
                ? (#name "_" __VA_ARGS__) : #name;                           \
            name = decltype(name)(dlsym(jitc_vulkan_spirv_handle, symbol));  \
            if (!name)                                                       \
                break;                                                       \
            symbol = nullptr

        LOAD(spvContextCreate);
        LOAD(spvContextDestroy);
        LOAD(spvTextToBinary);
        LOAD(spvBinaryDestroy);
        LOAD(spvDiagnosticPrint);
        LOAD(spvDiagnosticDestroy);

        #undef LOAD
    } while (false);

    if (symbol) {
        jitc_vulkan_spirv_api_shutdown();
        jitc_log(LogLevel::Warn,
                 "jitc_vulkan_spirv_api_init(): could not find symbol \"%s\" "
                 "-- disabling Vulkan backend!",
                 symbol);
        return false;
    }

    return true;
}

void jitc_vulkan_spirv_api_shutdown() {
    if (!jitc_vulkan_spirv_handle)
        return;

    #define Z(x) x = nullptr
    Z(spvContextCreate);
    Z(spvContextDestroy);
    Z(spvTextToBinary);
    Z(spvBinaryDestroy);
    Z(spvDiagnosticPrint);
    Z(spvDiagnosticDestroy);
    #undef Z

#if !defined(_WIN32)
    if (jitc_vulkan_spirv_handle != RTLD_NEXT)
        dlclose(jitc_vulkan_spirv_handle);
#else
    FreeLibrary((HMODULE) jitc_vulkan_spirv_handle);
#endif

    jitc_vulkan_spirv_handle = nullptr;
}

void *jitc_vulkan_spirv_lookup(const char *name) {
    void *ptr = dlsym(jitc_vulkan_spirv_handle, name);
    if (!ptr)
        jitc_raise("jitc_vulkan_spirv_lookup(): function \"%s\" not found!",
                   name);
    return ptr;
}
