cmake_minimum_required(VERSION 3.20)
cmake_policy(SET CMP0091 NEW)
include(cmake/CMake.cmake)

## Project Definition ##

dump_variables()

project(dscrd
    VERSION 0.1.0
    DESCRIPTION "A C++23 Discord bot library built off of coroutines"
    HOMEPAGE_URL "https://git.camora.dev/asriel/dscrd"
    LANGUAGES CXX)

retrieve_version_info()

message(STATUS "Compiling ${PROJECT_NAME} ${PROJECT_VERSION_LONG} for ${CMAKE_SYSTEM_NAME} (${VCPKG_TARGET_TRIPLET})")

## Set output directories ##

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/ar)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

## Add projects ##

add_subdirectories_in(deps)
add_subdirectory(src)
add_subdirectory(test)