#include "test.h"

namespace dr = drjit;

using Float = dr::VulkanArray<float>;

TEST_VULKAN(01_tmp_vulkan) {
    jit_assert(true);
}

TEST_LLVM(02_llvm_malloc) {
    void* ptr = jit_malloc(JitBackend::LLVM, AllocType::Host, 1024);
    jit_assert(true);
    jit_free(ptr);
    jit_flush_malloc_cache();
}

TEST_VULKAN(03_vulkan_malloc) {
    void* mem = jit_malloc(JitBackend::Vulkan, AllocType::Device, 1024);
    jit_assert(true);
    jit_free(mem);
    jit_flush_malloc_cache();
}

TEST_VULKAN(04_vulkan_var_mem_copy) {
    Float a(1, 2, 3, 4);
    Float b(1, 2, 3, 4);

    uint32_t tmp =
        jit_var_mem_copy(JitBackend::Vulkan, AllocType::Device,
                         VarType::Float32, b.data(), b.size() * sizeof(float));
    jit_var_dec_ref(tmp);
}

TEST_VULKAN(05_vulkan_add) {
    Float a(1, 2, 3, 4);
    Float b(2, 3, 4, 5);

    Float c = a + b;

    jit_assert(strcmp(c.str(), "[3, 5, 7, 9]") == 0);
}
