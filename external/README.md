# External Dependencies

This directory contains third-party libraries used by the FOV Overlay application.

## Dear ImGui

Dear ImGui is a bloat-free graphical user interface library for C++.

**Required files:**
- imgui.h, imgui.cpp
- imgui_draw.cpp
- imgui_tables.cpp
- imgui_widgets.cpp
- backends/imgui_impl_dx11.h, backends/imgui_impl_dx11.cpp
- backends/imgui_impl_win32.h, backends/imgui_impl_win32.cpp

**Download from:** https://github.com/ocornut/imgui

## nlohmann/json

JSON for Modern C++ - a header-only JSON library.

**Required files:**
- include/nlohmann/json.hpp

**Download from:** https://github.com/nlohmann/json

## Installation Instructions

1. Download Dear ImGui:
```bash
cd external
git clone https://github.com/ocornut/imgui.git
```

2. Download nlohmann/json:
```bash
cd external
mkdir -p json/include
cd json/include
wget https://github.com/nlohmann/json/releases/download/v3.11.3/json.hpp
mkdir nlohmann
mv json.hpp nlohmann/
```

## Optional: ONNX Runtime (for YOLOv8)

If using ONNX models for YOLOv8:

**Download from:** https://github.com/microsoft/onnxruntime/releases

Extract and add to your system path or CMake configuration.
