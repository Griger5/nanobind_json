############################################################################
# Copyright (c) 2019, Martin Renou, Ian Bell, Gracjan Adamus               #
#                                                                          #
# Distributed under the terms of the BSD 3-Clause License.                 #
#                                                                          #
# The full license is in the file LICENSE, distributed with this software. #
############################################################################

cmake_minimum_required(VERSION 3.5)
project(nanobind_json)

set(NANOBIND_JSON_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(${PROJECT_NAME}_VERSION_MAJOR 0)
set(${PROJECT_NAME}_VERSION_MINOR 0)
set(${PROJECT_NAME}_VERSION_PATCH 1)
set(${PROJECT_NAME}_VERSION ${${PROJECT_NAME}_VERSION_MAJOR}.${${PROJECT_NAME}_VERSION_MINOR}.${${PROJECT_NAME}_VERSION_PATCH})

################
# Dependencies #
################

include(FetchContent)
        
FetchContent_Declare(
        nanobind
        GIT_REPOSITORY https://github.com/wjakob/nanobind.git
        GIT_TAG master
)

FetchContent_Declare(
        nlohmann_json
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG master
)

FetchContent_MakeAvailable(nanobind nlohmann_json)

include_directories(build/_deps/nlohmann_json-src/include)

set(nanobind_REQUIRED_VERSION 2.2.0)
set(nlohmann_json_REQUIRED_VERSION 3.2.0)

if (NOT TARGET nanobind::headers)
find_package(Python COMPONENTS Interpreter Development REQUIRED)
endif()
if (NOT TARGET nlohmann_json)
endif()

##########
# Target #
##########

include(GNUInstallDirs)

set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib; ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}")

set(NANOBIND_JSON_HEADERS
    include/nanobind_json/nanobind_json.h
)

add_library(${PROJECT_NAME} INTERFACE)

target_include_directories(${PROJECT_NAME}
                           INTERFACE
                           ${PYTHON_INCLUDE_DIRS}
                           $<BUILD_INTERFACE:${NANOBIND_JSON_INCLUDE_DIR}>
                           $<INSTALL_INTERFACE:include>)

# The PUBLIC_HEADER property can be set for an INTERFACE library
# as of cmake 3.15
if(NOT (CMAKE_VERSION VERSION_LESS 3.15))  # CMake >= 3.15
set_target_properties(${PROJECT_NAME}
        PROPERTIES
        PUBLIC_HEADER "${NANOBIND_JSON_HEADERS}")
endif()

#########
# Tests #
#########

OPTION(BUILD_TESTS "nanobind_json test suite" OFF)

if(BUILD_TESTS)
        include_directories(include)
        nanobind_add_module(test_json_ext tests/test_json.cpp STABLE_ABI)
        nanobind_install_runtime_dlls(test_json_ext)
endif()

################
# Installation #
################

include(CMakePackageConfigHelpers)

set(NANOBIND_JSON_CMAKECONFIG_INSTALL_DIR "${CMAKE_INSTALL_DATAROOTDIR}/cmake/${PROJECT_NAME}" CACHE STRING "install path for nanobind_jsonConfig.cmake")

# Install nanobind_json
if(CMAKE_VERSION VERSION_LESS 3.15)  # CMake < 3.15
install(TARGETS ${PROJECT_NAME}
        EXPORT ${PROJECT_NAME}-targets)
else()
install(TARGETS ${PROJECT_NAME}
        EXPORT ${PROJECT_NAME}-targets
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME})
endif()

# Makes the project importable from the build directory
export(EXPORT ${PROJECT_NAME}-targets
       FILE "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Targets.cmake")

if(CMAKE_VERSION VERSION_LESS 3.15)  # CMake < 3.15
install(FILES ${NANOBIND_JSON_HEADERS}
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME})
endif()

# Configure 'nanobind_jsonConfig.cmake' for a build tree
set(NANOBIND_JSON_CONFIG_CODE "####### Expanded from \@NANOBIND_JSON_CONFIG_CODE\@ #######\n")
set(NANOBIND_JSON_CONFIG_CODE "${NANOBIND_JSON_CONFIG_CODE}set(CMAKE_MODULE_PATH \"${CMAKE_CURRENT_SOURCE_DIR}/cmake;\${CMAKE_MODULE_PATH}\")\n")
set(NANOBIND_JSON_CONFIG_CODE "${NANOBIND_JSON_CONFIG_CODE}##################################################")
configure_package_config_file(${PROJECT_NAME}Config.cmake.in
                              "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
                              INSTALL_DESTINATION ${PROJECT_BINARY_DIR})

# Configure 'nanobind_jsonConfig.cmake' for an install tree
set(NANOBIND_JSON_CONFIG_CODE "")
configure_package_config_file(${PROJECT_NAME}Config.cmake.in
                              "${CMAKE_CURRENT_BINARY_DIR}/CMakeFiles/${PROJECT_NAME}Config.cmake"
                              INSTALL_DESTINATION ${NANOBIND_JSON_CMAKECONFIG_INSTALL_DIR})

# ARCH_INDEPENDENT feature was introduced in cmake 3.14
if (${CMAKE_VERSION} VERSION_LESS "3.14.0")
  write_basic_package_version_file(
      ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
      VERSION ${${PROJECT_NAME}_VERSION}
      COMPATIBILITY AnyNewerVersion)
else ()
  write_basic_package_version_file(
      ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
      VERSION ${${PROJECT_NAME}_VERSION}
      COMPATIBILITY AnyNewerVersion
      ARCH_INDEPENDENT)
endif ()

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/CMakeFiles/${PROJECT_NAME}Config.cmake
              ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
        DESTINATION ${NANOBIND_JSON_CMAKECONFIG_INSTALL_DIR})
install(EXPORT ${PROJECT_NAME}-targets
        FILE ${PROJECT_NAME}Targets.cmake
        DESTINATION ${NANOBIND_JSON_CMAKECONFIG_INSTALL_DIR})
