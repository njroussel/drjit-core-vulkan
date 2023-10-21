#include "vulkan_ts.h"

Task *VulkanThreadState::launch(Kernel kernel, uint32_t size,
                              std::vector<void *> *kernel_params,
                              uint32_t kernel_param_count,
                              const uint8_t *kernel_params_global) {
    return nullptr;
}

/// Fill a device memory region with constants of a given type
void VulkanThreadState::memset_async(void *ptr, uint32_t size_, uint32_t isize,
                                     const void *src) {}

void VulkanThreadState::reduce(VarType vt, ReduceOp op, const void *ptr,
                               uint32_t size, void *out) {}

void VulkanThreadState::block_reduce(VarType vt, ReduceOp op, const void *in,
                                     uint32_t size, uint32_t block_size,
                                     void *out) {}

void VulkanThreadState::reduce_dot(VarType vt, const void *ptr_1,
                                   const void *ptr_2, uint32_t size,
                                   void *out) {}

bool VulkanThreadState::all(uint8_t *values, uint32_t size) {}

bool VulkanThreadState::any(uint8_t *values, uint32_t size) {}

void VulkanThreadState::prefix_sum(VarType vt, bool exclusive, const void *in,
                                   uint32_t size, void *out) {}

uint32_t VulkanThreadState::compress(const uint8_t *in, uint32_t size,
                                     uint32_t *out) {
  return 0;
}

uint32_t VulkanThreadState::mkperm(const uint32_t *ptr, uint32_t size,
                                   uint32_t bucket_count, uint32_t *perm,
                                   uint32_t *offsets) {
  return 0;
}

void VulkanThreadState::memcpy(void *dst, const void *src, size_t size) {}

void VulkanThreadState::memcpy_async(void *dst, const void *src, size_t size) {}

void VulkanThreadState::poke(void *dst, const void *src, uint32_t size) {}

void VulkanThreadState::aggregate(void *dst_, AggregationEntry *agg,
                                  uint32_t size) {}

void VulkanThreadState::enqueue_host_func(void (*callback)(void *),
                                          void *payload) {}
