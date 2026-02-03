cmake_minimum_required(VERSION 3.15)
project(GamingOverlay)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Add include directories
include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/libs/imgui
    ${CMAKE_SOURCE_DIR}/libs/imgui/backends
)

# Source files
set(SOURCES
    src/main.cpp
    src/overlay_window.cpp
    src/renderer.cpp
    src/ui_manager.cpp
    src/settings_manager.cpp
    src/object_detector.cpp
    src/tracking_system.cpp
)

# ImGui source files
set(IMGUI_SOURCES
    libs/imgui/imgui.cpp
    libs/imgui/imgui_demo.cpp
    libs/imgui/imgui_draw.cpp
    libs/imgui/imgui_tables.cpp
    libs/imgui/imgui_widgets.cpp
    libs/imgui/backends/imgui_impl_win32.cpp
    libs/imgui/backends/imgui_impl_dx11.cpp
)

# Create executable
add_executable(GamingOverlay WIN32 ${SOURCES} ${IMGUI_SOURCES})

# Link libraries
target_link_libraries(GamingOverlay
    d3d11.lib
    d3dcompiler.lib
    dxgi.lib
)

# Set compiler options
if(MSVC)
    target_compile_options(GamingOverlay PRIVATE /W4 /WX-)
    target_compile_definitions(GamingOverlay PRIVATE _CRT_SECURE_NO_WARNINGS)
endif()

# Copy assets and config to build directory
add_custom_command(TARGET GamingOverlay POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:GamingOverlay>/assets
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/config $<TARGET_FILE_DIR:GamingOverlay>/config
)

# Installation
install(TARGETS GamingOverlay DESTINATION bin)
install(DIRECTORY assets DESTINATION bin)
install(DIRECTORY config DESTINATION bin)
