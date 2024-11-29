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
# Version information
# #####################################################################
include(version.cmake)
set(CYTNX_CORE_VERSION
  ${CYTNX_CORE_VERSION_MAJOR}.${CYTNX_CORE_VERSION_MINOR}.${CYTNX_CORE_VERSION_PATCH}
)

## Project:
project(CYTNX_CORE VERSION ${CYTNX_CORE_VERSION} LANGUAGES CXX C)
add_library(cytnx_core STATIC)
set_property(TARGET cytnx_core PROPERTY C_VISIBILITY_PRESET hidden)
set_property(TARGET cytnx_core PROPERTY VISIBILITY_INLINES_HIDDEN ON)

message(STATUS " Current source dir: ${CMAKE_CURRENT_SOURCE_DIR}")
target_include_directories(cytnx_core
  PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/src/cpp/src
  PUBLIC
  $<INSTALL_INTERFACE:include>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/cpp/include>
)
# cpp source directory
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/cpp/src)
target_compile_definitions(cytnx_core PUBLIC _LIBCPP_DISABLE_AVAILABILITY)
target_compile_definitions(cytnx_core PUBLIC _LIBCPP_ENABLE_CXX17_REMOVED_UNARY_BINARY_FUNCTION)
target_compile_options(cytnx_core PUBLIC -Wformat=0 -w -fsized-deallocation)
target_compile_features(cytnx_core PUBLIC cxx_std_17)


## install
include(GNUInstallDirs)

message(STATUS " install libdir: ${CMAKE_INSTALL_LIBDIR}")
message(STATUS " install includedir: ${CMAKE_INSTALL_INCLUDEDIR}")
set(INSTALL_CONFIGDIR ${CMAKE_INSTALL_LIBDIR}/cmake/cytnx_core)
INSTALL(TARGETS cytnx_core EXPORT cytnx_targets
  LIBRARY
  DESTINATION ${CMAKE_INSTALL_LIBDIR}
  COMPONENT libraries
  ARCHIVE
  DESTINATION ${CMAKE_INSTALL_LIBDIR}
  COMPONENT libraries
  PUBLIC_HEADER
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
  COMPONENT Development
)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/src/cpp/include/
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
  COMPONENT headers
  FILES_MATCHING PATTERN "*.h*")

export(EXPORT cytnx_targets FILE ${CMAKE_CURRENT_BINARY_DIR}/CytnxTargets.cmake NAMESPACE cytnx_core::)
export(PACKAGE cytnx_core)

# #####################################################################
# Pybind
# #####################################################################
set(PYBIND11_FINDPYTHON ON)
find_package(pybind11 CONFIG REQUIRED)

pybind11_add_module(pycytnx MODULE src/cpp/pybind/main.cpp)
target_link_libraries(pycytnx PUBLIC cytnx_core)
#install(TARGETS pycytnx DESTINATION ${SKBUILD_PROJECT_NAME})
install(TARGETS pycytnx DESTINATION ${CMAKE_INSTALL_PREFIX}/cytnx_core)
message(STATUS " skbuild Installation Prefix: ${SKBUILD_PROJECT_NAME}")
