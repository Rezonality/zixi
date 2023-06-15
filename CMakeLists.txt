cmake_minimum_required(VERSION 3.2)

message(STATUS " CMakeLists: Zixi")

option(BUILD_TESTS "Build Tests" ON)
option(ZIXI_LIBRARY_ONLY "Only build library" OFF)

# Global Settings
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(CMAKE_DEBUG_POSTFIX "-debug")
set(CMAKE_RELWITHDEBINFO_POSTFIX "-reldbg")

if (WIN32)
set(VCPKG_TARGET_TRIPLET x64-windows-static-md)
endif()

set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake CACHE STRING "Vcpkg toolchain file")

project(Zixi
    LANGUAGES CXX C
    VERSION 0.5.0
)

include(cmake/utils.cmake)
find_package(fmt REQUIRED)
find_path(TINYDIR_INCLUDE_DIRS "tinydir.h")

set(ZIXI_ROOT ${CMAKE_CURRENT_LIST_DIR})
set(ZIXI_DIR ${CMAKE_CURRENT_LIST_DIR})
set(ZIXI_INCLUDE_DIR include/)

configure_file(${CMAKE_CURRENT_LIST_DIR}/cmake/config_zixi_app.h.cmake ${CMAKE_BINARY_DIR}/config_zixi_app.h)

enable_testing()

include(cmake/all.cmake)

add_subdirectory(libs/zest)
add_subdirectory(src)

if (NOT ${ZIXI_LIBRARY_ONLY})
add_subdirectory(app)
add_subdirectory(tests)
endif()

