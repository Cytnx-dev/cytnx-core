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
set(PKG_NAME "cytnx_core") # only need to change this for different pkg

## Project:
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
