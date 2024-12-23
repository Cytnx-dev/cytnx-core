message(STATUS "")
message(STATUS "------------------------------------------------------------------------")
message(STATUS "  Project Cytnx (core), A Cross-section of Python & C++,Tensor network library ")
message(STATUS "------------------------------------------------------------------------")
message(STATUS "")


# #####################################################################
# ## CMAKE and CXX VERSION
# #####################################################################
cmake_minimum_required(VERSION 3.20)

include(cmake/target_sources_local.cmake)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

message(STATUS " Generator: ${CMAKE_GENERATOR}")
message(STATUS " Build Target: ${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR}")
message(STATUS " Installation Prefix: ${CMAKE_INSTALL_PREFIX}")
set(CMAKE_POSITION_INDEPENDENT_CODE ON)



# #####################################################################
# Project
# #####################################################################
include(version.cmake)
set(CYTNX_CORE_VERSION
  ${CYTNX_CORE_VERSION_MAJOR}.${CYTNX_CORE_VERSION_MINOR}.${CYTNX_CORE_VERSION_PATCH}
)
set(PKG_NAME "cytnx_core") # only need to change this for different pkg

project(CYTNX_CORE VERSION ${CYTNX_CORE_VERSION} LANGUAGES CXX C)
add_library(${PKG_NAME} STATIC)
set_property(TARGET ${PKG_NAME} PROPERTY C_VISIBILITY_PRESET hidden)
set_property(TARGET ${PKG_NAME} PROPERTY VISIBILITY_INLINES_HIDDEN ON)

message(STATUS " Current source dir: ${CMAKE_CURRENT_SOURCE_DIR}")
target_include_directories(${PKG_NAME}
  PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/src/cpp/src
  PUBLIC
  $<INSTALL_INTERFACE:${PKG_name}/include>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/cpp/include>
)
# cpp source directory
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/cpp/src)
target_compile_definitions(${PKG_NAME} PUBLIC _LIBCPP_DISABLE_AVAILABILITY)
target_compile_definitions(${PKG_NAME} PUBLIC _LIBCPP_ENABLE_CXX17_REMOVED_UNARY_BINARY_FUNCTION)
target_compile_options(${PKG_NAME} PUBLIC -Wformat=0 -w -fsized-deallocation)
target_compile_features(${PKG_NAME} PUBLIC cxx_std_17)

# ##########################
# Pre-define options when build with skbuild
# ##########################

if(DEFINED SKBUILD)

    execute_process(
      COMMAND bash -c "python -c 'import site; print(site.getsitepackages()[0],end=str())'"
      OUTPUT_VARIABLE PY_SITE_PKG_PREFIX
    )

    # get openblas deps
    set(OpenBLAS_LIBRARIES ${PY_SITE_PKG_PREFIX}/scipy_openblas64/lib/libscipy_openblas64_.so)
    set(OpenBLAS_INCLUDE_DIRS ${PY_SITE_PKG_PREFIX}/scipy_openblas64/include)
    option(LAPACK_ILP64 "using openblas with ILP64")
    set(BLAS_LIBRARIES ${OpenBLAS_LIBRARIES})
    set(LAPACK_LIBRARIES ${OpenBLAS_LIBRARIES})
    target_include_directories(${PKG_NAME} SYSTEM
      PUBLIC
      ${OpenBLAS_INCLUDE_DIRS}
    )
    target_link_libraries(${PKG_NAME} PUBLIC ${LAPACK_LIBRARIES})
endif()


# ##########################
# Linalg
# ##########################
if (USE_MKL)
  option(MKL_SDL "Link to a single MKL dynamic libary." ON)
  option(MKL_MLT "Use multi-threading libary. [Default]" ON)
  mark_as_advanced(MKL_SDL MKL_MLT)
  set(CYTNX_VARIANT_INFO "${CYTNX_VARIANT_INFO} UNI_MKL")
  target_compile_definitions(${PKG_NAME} PUBLIC UNI_MKL)
  target_compile_definitions(${PKG_NAME} PUBLIC MKL_ILP64)
endif() #use_mkl

if( NOT (DEFINED BLAS_LIBRARIES AND DEFINED LAPACK_LIBRARIES))
  if (USE_MKL)
    #set(BLA_VENDOR Intel10_64ilp)
    set(BLA_VENDOR Intel10_64_dyn)
    find_package( BLAS REQUIRED)
    find_package( LAPACK REQUIRED)
    #find_package(MKL REQUIRED)
    target_link_libraries(${PKG_NAME} PUBLIC ${LAPACK_LIBRARIES})
    message( STATUS "LAPACK found: ${LAPACK_LIBRARIES}" )
  else()
    set(BLA_VENDOR OpenBLAS)
    find_package( BLAS REQUIRED)
    find_package( LAPACK REQUIRED)
    target_link_libraries(${PKG_NAME} PUBLIC ${LAPACK_LIBRARIES})
    message( STATUS "LAPACK found: ${LAPACK_LIBRARIES}" )
  endif()

else()
  set(LAPACK_LIBRARIES  ${BLAS_LIBRARIES}  ${LAPACK_LIBRARIES})
  message( STATUS "LAPACK found: ${LAPACK_LIBRARIES}")
  target_link_libraries(${PKG_NAME} PUBLIC ${LAPACK_LIBRARIES})
endif()

# ###########
# Options
# ###########
option(USE_CUDA "Build using Nvidia CUDA for GPU library" OFF)
if(USE_CUDA)
  include(cmake/config_cuda.cmake)
endif()


## install
include(GNUInstallDirs)

message(STATUS " install libdir: ${CMAKE_INSTALL_LIBDIR}")
message(STATUS " install includedir: ${CMAKE_INSTALL_INCLUDEDIR}")
set(INSTALL_CONFIGDIR ${CMAKE_INSTALL_LIBDIR}/cmake/${PKG_NAME})
INSTALL(TARGETS ${PKG_NAME} EXPORT ${PKG_NAME}_targets
  LIBRARY
  DESTINATION ${PKG_NAME}/cpp/${CMAKE_INSTALL_LIBDIR}
  COMPONENT libraries
  ARCHIVE
  DESTINATION ${PKG_NAME}/cpp/${CMAKE_INSTALL_LIBDIR}
  COMPONENT libraries
  PUBLIC_HEADER
  DESTINATION ${PKG_NAME}/cpp/${CMAKE_INSTALL_INCLUDEDIR}
  COMPONENT Development
)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/src/cpp/include/
  DESTINATION ${PKG_NAME}/cpp/${CMAKE_INSTALL_INCLUDEDIR}
  COMPONENT headers
  FILES_MATCHING PATTERN "*.h*")


export(EXPORT ${PKG_NAME}_targets FILE ${CMAKE_CURRENT_BINARY_DIR}/${PKG_NAME}_targets.cmake NAMESPACE ${PKG_NAME}::)
export(PACKAGE ${PKG_NAME})

# #####################################################################
# Pybind
# #####################################################################
set(PYBIND11_FINDPYTHON ON)
find_package(pybind11 CONFIG REQUIRED)

pybind11_add_module(_core MODULE src/cpp/pybind/main.cpp)
target_link_libraries(_core PUBLIC ${PKG_NAME})

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/src/${PKG_NAME}
    DESTINATION ${CMAKE_INSTALL_PREFIX}
)
install(TARGETS _core LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/${PKG_NAME})
#install(TARGETS pycytnx DESTINATION ${SKBUILD_PROJECT_NAME})
message(STATUS " skbuild Installation Prefix: ${SKBUILD_PROJECT_NAME}")
message(STATUS " skbuild Installation Prefix: ${SKBUILD_HEADERS_DIR}")
