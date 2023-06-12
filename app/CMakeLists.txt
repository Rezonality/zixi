project(Zixi_App VERSION 0.1.0.0)

set(APP_NAME Zixi_App)

# Set additional project information
set(COMPANY "Zixi")
set(COPYRIGHT "Copyright (c) 2019-2023 Chris Maughan. All rights reserved.")
set(IDENTIFIER "com.zing")

set(APP_ROOT ${CMAKE_CURRENT_LIST_DIR})

list(INSERT CMAKE_MODULE_PATH 0 ${CMAKE_CURRENT_LIST_DIR}/cmake)

set(ZIXI_APP_SOURCE
    ${APP_ROOT}/main.cpp
    ${APP_ROOT}/CMakeLists.txt
    )

set(RESOURCE_FOLDER ${CMAKE_CURRENT_LIST_DIR}/res)

add_executable (${APP_NAME}
    ${ZIXI_APP_SOURCE}
    )

target_include_directories(${APP_NAME}
    PRIVATE
    ${ZIXI_ROOT}/libs/zest/include
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_BINARY_DIR}
    )

target_link_libraries (${APP_NAME}
    PRIVATE
    Zixi::Zixi
    ${PLATFORM_LINKLIBS}
    )

target_precompile_headers(${APP_NAME}
  PRIVATE
    ${APP_ROOT}/pch.h
)

target_compile_definitions(${APP_NAME}
    PUBLIC
    _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING)

if (WIN32)
    copy_existing_files(${PROJECT_NAME} "${RESOURCE_DEPLOY_FILES}" ${CMAKE_CURRENT_BINARY_DIR}/$(Configuration) )
endif()

source_group ("Source" FILES ${ZIXI_APP_SOURCE})

