project(Zixi
    LANGUAGES CXX C
    VERSION 0.5.0
)

find_package(fmt CONFIG REQUIRED)

set(ZIXI_SOURCE
    ${ZIXI_ROOT}/src/zixi.cpp
    ${ZIXI_ROOT}/src/CMakeLists.txt
    ${ZIXI_ROOT}/include/zixi/zixi.h
)

set(ZIXI_SOURCE
    ${ZIXI_SOURCE}
    ${ZIXI_ROOT}/CMakeLists.txt
    ${ZIXI_ROOT}/project.natvis
)

add_library(Zixi STATIC ${ZIXI_SOURCE})
add_library(Zixi::Zixi ALIAS Zixi)

target_include_directories(Zixi
PRIVATE
    ${CMAKE_BINARY_DIR}
PUBLIC
    $<BUILD_INTERFACE:${ZIXI_ROOT}/libs>
    $<BUILD_INTERFACE:${ZIXI_ROOT}/include>
)

target_compile_definitions(Zixi
    PUBLIC
    NO_LIBSNDFILE
    _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING)

target_link_libraries(Zixi
PUBLIC
    Zest::Zest
)

if("${CMAKE_SYSTEM_NAME}" STREQUAL "Linux")
    target_link_libraries(Zixi PUBLIC atomic)
endif()

target_precompile_headers(Zixi
  PRIVATE
    ${ZIXI_ROOT}/include/zixi/pch.h
)

source_group ("zixi" FILES ${ZIXI_SOURCE})

