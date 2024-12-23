#ifndef CYTNX_BACKEND_UTILS_INTERNAL_GPU_CUALLOC_GPU_H_
#define CYTNX_BACKEND_UTILS_INTERNAL_GPU_CUALLOC_GPU_H_

#include <cstdio>
#include <cstdlib>
#include <stdint.h>
#include <climits>
#include <cytnx_core/Type.hpp>
#include <cytnx_core/errors/cytnx_error.hpp>
namespace cytnx_core {
  namespace utils_internal {

#ifdef UNI_GPU
    void* cuCalloc_gpu(const cytnx_uint64& N, const cytnx_uint64& perelem_bytes);
    void* cuMalloc_gpu(const cytnx_uint64& bytes);
#endif
  }  // namespace utils_internal
}  // namespace cytnx_core

#endif  // CYTNX_BACKEND_UTILS_INTERNAL_GPU_CUALLOC_GPU_H_
