############################################################################
# Copyright (c) 2019, Martin Renou, Ian Bell, Gracjan Adamus               #
#                                                                          #
# Distributed under the terms of the BSD 3-Clause License.                 #
#                                                                          #
# The full license is in the file LICENSE, distributed with this software. #
############################################################################

cmake_minimum_required(VERSION 3.15)
project(nanobind_json)

set(NANOBIND_JSON_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(${PROJECT_NAME}_VERSION_MAJOR 0)
set(${PROJECT_NAME}_VERSION_MINOR 0)
set(${PROJECT_NAME}_VERSION_PATCH 1)
set(${PROJECT_NAME}_VERSION ${${PROJECT_NAME}_VERSION_MAJOR}.${${PROJECT_NAME}_VERSION_MINOR}.${${PROJECT_NAME}_VERSION_PATCH})

##########
# Target #
##########

include(GNUInstallDirs)

set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib; ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}")

set(NANOBIND_JSON_HEADERS
    include/nanobind_json/nanobind_json.h
)

add_library(${PROJECT_NAME} INTERFACE)

target_include_directories(
	${PROJECT_NAME}
    INTERFACE
    ${PYTHON_INCLUDE_DIRS}
    $<BUILD_INTERFACE:${NANOBIND_JSON_INCLUDE_DIR}>
    $<INSTALL_INTERFACE:include>
)

#########
# Tests #
#########

OPTION(NBJSON_TEST "nanobind_json test suite" OFF)

if(NBJSON_TEST)
    add_subdirectory(tests)
endif()

################
# Installation #
################

include(CMakePackageConfigHelpers)

set(NANOBIND_JSON_CMAKECONFIG_INSTALL_DIR "${CMAKE_INSTALL_DATAROOTDIR}/cmake/${PROJECT_NAME}" CACHE STRING "install path for nanobind_jsonConfig.cmake")

# Install nanobind_json
install(
	TARGETS ${PROJECT_NAME}
    EXPORT ${PROJECT_NAME}-targets
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}
)

# Makes the project importable from the build directory
export(
	EXPORT ${PROJECT_NAME}-targets
    FILE "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Targets.cmake"
)

# Configure 'nanobind_jsonConfig.cmake' for a build tree
set(NANOBIND_JSON_CONFIG_CODE "####### Expanded from \@NANOBIND_JSON_CONFIG_CODE\@ #######\n")
set(NANOBIND_JSON_CONFIG_CODE "${NANOBIND_JSON_CONFIG_CODE}set(CMAKE_MODULE_PATH \"${CMAKE_CURRENT_SOURCE_DIR}/cmake;\${CMAKE_MODULE_PATH}\")\n")
set(NANOBIND_JSON_CONFIG_CODE "${NANOBIND_JSON_CONFIG_CODE}##################################################")
configure_package_config_file(
	${PROJECT_NAME}Config.cmake.in
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
    INSTALL_DESTINATION ${PROJECT_BINARY_DIR}
)

# Configure 'nanobind_jsonConfig.cmake' for an install tree
set(NANOBIND_JSON_CONFIG_CODE "")
configure_package_config_file(
	${PROJECT_NAME}Config.cmake.in
    "${CMAKE_CURRENT_BINARY_DIR}/CMakeFiles/${PROJECT_NAME}Config.cmake"
    INSTALL_DESTINATION ${NANOBIND_JSON_CMAKECONFIG_INSTALL_DIR}
)

write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
    VERSION ${${PROJECT_NAME}_VERSION}
    COMPATIBILITY AnyNewerVersion
    ARCH_INDEPENDENT
)

install(
	FILES ${CMAKE_CURRENT_BINARY_DIR}/CMakeFiles/${PROJECT_NAME}Config.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake    
	DESTINATION ${NANOBIND_JSON_CMAKECONFIG_INSTALL_DIR}
)

install(
	EXPORT ${PROJECT_NAME}-targets
    FILE ${PROJECT_NAME}Targets.cmake
    DESTINATION ${NANOBIND_JSON_CMAKECONFIG_INSTALL_DIR}
)