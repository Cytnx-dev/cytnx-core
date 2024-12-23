if(USE_CUDA)
    message(STATUS " Enable CUDA Support")
    set(CYTNX_VARIANT_INFO "${CYTNX_VARIANT_INFO} UNI_CUDA")
    enable_language(CUDA)
    find_package(CUDAToolkit REQUIRED)
    message(STATUS "CUDA: ${CUDA_TOOLKIT_FOUND}")
    if(NOT DEFINED CMAKE_CUDA_STANDARD)
        set(CMAKE_CUDA_STANDARD 17)
        set(CMAKE_CUDA_STANDARD_REQUIRED ON)
    endif()


    set_target_properties(${PKG_NAME} PROPERTIES
        CUDA_SEPARABLE_COMPILATION ON
                                    )
    set_target_properties(${PKG_NAME} PROPERTIES CUDA_RESOLVE_DEVICE_SYMBOLS ON)
    set(CMAKE_CUDA_FLAGS "${CMAKE_CUDA_FLAGS} -Xcudafe=--display_error_number -lineinfo -m64")
    #set(CMAKE_CUDA_FLAGS "-Xcompiler=-Wall -Xcompiler=-Wno-deprecated-gpu-targets -Xcudafe=--display_error_number")
    ##set(CMAKE_CUDA_FLAGS "-Xcompiler=-Wall -Wno-deprecated-gpu-targets -Xcudafe=--display_error_number")
    ##  set(CUDA_NVCC_FLAGS ${CUDA_NVCC_FLAGS}  "-DUNI_GPU")
    #  set(CUDA_NVCC_FLAGS ${CUDA_NVCC_FLAGS}  "-arch=sm_50 \
    #      -gencode=arch=compute_50,code=sm_50 \
    #      -gencode=arch=compute_52,code=sm_52 \
    #      -gencode=arch=compute_60,code=sm_60 \
    #      -gencode=arch=compute_61,code=sm_61 \
    #      -gencode=arch=compute_70,code=sm_70 \
    #      -gencode=arch=compute_75,code=sm_75 \
    #      -gencode=arch=compute_75,code=compute_75 ")
    set_property(TARGET ${PKG_NAME} PROPERTY CUDA_ARCHITECTURES "80;86;90")
    #et_property(TARGET ${PKG_NAME} PROPERTIES CUDA_ARCHITECTURES "80;86;90")
    target_compile_definitions(${PKG_NAME} PUBLIC UNI_GPU)
    target_include_directories(${PKG_NAME} PRIVATE ${CUDAToolkit_INCLUDE_DIRS})
    target_link_libraries(${PKG_NAME} PUBLIC CUDA::toolkit)
    target_link_libraries(${PKG_NAME} PUBLIC CUDA::cudart CUDA::cublas CUDA::cusparse CUDA::curand CUDA::cusolver)
    target_link_libraries(${PKG_NAME} PUBLIC -lcudadevrt)
else()
    message( STATUS " Build CUDA Support: NO")
endif()
