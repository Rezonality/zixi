cmake_minimum_required(VERSION 3.2)

message(STATUS " CMakeLists: Zixi")

option(BUILD_TESTS "Build Tests" ON)
option(ZIXI_LIBRARY_ONLY "Only build library" OFF)

# Global Settings
set(CMAKE_CXX_STANDARD 17)
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

set(ZIXI_ROOT ${CMAKE_CURRENT_LIST_DIR})
set(ZIXI_INCLUDE_DIR include/)

configure_file(${CMAKE_CURRENT_LIST_DIR}/cmake/config_zixi_app.h.cmake ${CMAKE_BINARY_DIR}/config_zixi_app.h)

include(GNUInstallDirs)

enable_testing()

include(cmake/all.cmake)

add_subdirectory(src)

if (NOT ${ZIXI_LIBRARY_ONLY})
add_subdirectory(app)
add_subdirectory(tests)
endif()

# Make the CMake bits that ensure find_package does the right thing
install(EXPORT zixi-targets
    FILE zixi-targets.cmake
    NAMESPACE zixi::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/zixi
)

include(CMakePackageConfigHelpers)

configure_package_config_file(
    ${ZIXI_ROOT}/cmake/zixi-config.cmake.in
    ${CMAKE_BINARY_DIR}/cmake/zixi-config.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/zixi
)

write_basic_package_version_file(
    ${CMAKE_BINARY_DIR}/cmake/zixi-config-version.cmake
    VERSION 1.0
    COMPATIBILITY AnyNewerVersion
)

install(
    FILES
        ${CMAKE_BINARY_DIR}/cmake/zixi-config.cmake
        ${CMAKE_BINARY_DIR}/cmake/zixi-config-version.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/zixi
)
