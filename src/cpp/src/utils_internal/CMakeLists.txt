add_subdirectory(cpu)

if(USE_CUDA)
  add_subdirectory(gpu)
endif()
