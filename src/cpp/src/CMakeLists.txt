target_sources_local(cytnx_core
  PRIVATE

  Device.cpp
  Type.cpp

)


add_subdirectory(storage_impl)
add_subdirectory(utils_internal)
