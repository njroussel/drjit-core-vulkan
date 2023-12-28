#include "test.h"

TEST_VULKAN(01_tmp_vulkan) {
    jit_assert(true);
}

TEST_VULKAN(02_tmp_vulkan) {
    jit_assert(true);
}

TEST_LLVM(03_llvm_malloc) {
    void* ptr = jit_malloc(JitBackend::LLVM, AllocType::Host, 1024);
    jit_assert(true);
    jit_free(ptr);
    jit_flush_malloc_cache();
}

TEST_VULKAN(04_vulkan_malloc) {
    void* mem = jit_malloc(JitBackend::Vulkan, AllocType::Device, 1024);
    jit_assert(true);
    jit_free(mem);
    jit_flush_malloc_cache();
}

