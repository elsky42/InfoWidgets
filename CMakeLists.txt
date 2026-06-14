set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

cmake_minimum_required(VERSION 3.21)

project(
    InfoWidgets
    VERSION 0.0.6
    LANGUAGES CXX
)

set(PROJECT_AUTHOR "Elsky")
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Version.h.in"
    "${CMAKE_CURRENT_BINARY_DIR}/include/Version.h"
    @ONLY
)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)

set(BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(ENABLE_SKYRIM_VR OFF CACHE BOOL "" FORCE)
add_subdirectory(extern/CommonLibSSE-NG)
find_package(spdlog CONFIG REQUIRED)

file(GLOB_RECURSE PLUGIN_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h"
)

set(IMGUI_DIR "${CMAKE_CURRENT_SOURCE_DIR}/extern/imgui")
set(IMGUI_SOURCES
    "${IMGUI_DIR}/imgui.cpp"
    "${IMGUI_DIR}/imgui_draw.cpp"
    "${IMGUI_DIR}/imgui_tables.cpp"
    "${IMGUI_DIR}/imgui_widgets.cpp"
    "${IMGUI_DIR}/imgui_impl_dx11.cpp"
)

add_library(${PROJECT_NAME} SHARED ${PLUGIN_SOURCES} ${IMGUI_SOURCES})

target_include_directories(${PROJECT_NAME} BEFORE PRIVATE
    "${CMAKE_CURRENT_BINARY_DIR}/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/extern/imgui"
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_include_directories(${PROJECT_NAME} SYSTEM PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/extern/CommonLibSSE-NG/include"
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    CommonLibSSE::CommonLibSSE
    spdlog::spdlog
    d3d11
    dxgi
    d3dcompiler
)

target_compile_definitions(${PROJECT_NAME} PRIVATE
    SKSE_SUPPORT_XBYAK
    WIN32_LEAN_AND_MEAN
    NOMINMAX
    UNICODE
    _UNICODE
)

target_compile_options(${PROJECT_NAME} PRIVATE
    /W4
    /WX
    /permissive-
    /Zc:preprocessor
    /utf-8
)

set(FA_FONT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/fonts")
set(FA_FONTS_DEST "${CMAKE_BINARY_DIR}/InfoWidgets/SKSE/Plugins/InfoWidgets/Fonts")

set(FONT_COPY_COMMANDS
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${FA_FONT_DIR}" "${FA_FONTS_DEST}"
)

add_custom_command(
    TARGET ${PROJECT_NAME}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_BINARY_DIR}/InfoWidgets/SKSE/Plugins"
    COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE:${PROJECT_NAME}>" "${CMAKE_BINARY_DIR}/InfoWidgets/SKSE/Plugins"
    ${FONT_COPY_COMMANDS}
    COMMAND ${CMAKE_COMMAND} -E chdir "${CMAKE_BINARY_DIR}/InfoWidgets"
            ${CMAKE_COMMAND} -E tar "cfv" "${CMAKE_BINARY_DIR}/InfoWidget-${PROJECT_VERSION}.zip" --format=zip
                                    "SKSE"
    COMMENT "Packaging output..."
)
