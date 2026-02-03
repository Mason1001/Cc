# Architecture Diagram

## System Architecture

```
┌─────────────────────────────────────────────────────────────────────┐
│                       FOV Overlay Application                        │
└─────────────────────────────────────────────────────────────────────┘

                              ┌─────────────┐
                              │   main.cpp  │
                              │ Application │
                              └──────┬──────┘
                                     │
         ┌───────────────────────────┼───────────────────────────┐
         │                           │                           │
         ▼                           ▼                           ▼
┌─────────────────┐        ┌─────────────────┐        ┌─────────────────┐
│ OverlayManager  │        │  ConfigManager  │        │  ImGuiManager   │
│   (Window)      │        │     (JSON)      │        │      (UI)       │
└────────┬────────┘        └────────┬────────┘        └────────┬────────┘
         │                          │                          │
         │                          │                          │
         ▼                          │                          │
┌─────────────────┐                │                          │
│   DirectX 11    │◄───────────────┘                          │
│  Device/Context │                                            │
└────────┬────────┘                                            │
         │                                                     │
         ├─────────────────────────────────────────────────────┘
         │
         ▼
┌─────────────────────────────────────────────────────────────────────┐
│                         Rendering Pipeline                           │
├─────────────────────────────────────────────────────────────────────┤
│                                                                       │
│  ┌─────────────┐    ┌──────────────┐    ┌──────────────┐           │
│  │FOVRenderer  │    │YOLODetector  │    │SoundTracker  │           │
│  │             │    │              │    │              │           │
│  │• Circle     │    │• Detection   │    │• Sources     │           │
│  │• Line       │    │• Tracking    │    │• Indicators  │           │
│  │• Box        │    │• Boxes       │    │• Direction   │           │
│  └─────────────┘    └──────┬───────┘    └──────────────┘           │
│                             │                                        │
│                             ▼                                        │
│                     ┌───────────────┐                                │
│                     │ObjectTracker  │                                │
│                     │               │                                │
│                     │• IOU Match    │                                │
│                     │• Velocity     │                                │
│                     │• Track ID     │                                │
│                     └───────────────┘                                │
│                                                                       │
└───────────────────────────────────────────────────────────────────────┘
         │
         ▼
┌─────────────────────────────────────────────────────────────────────┐
│                            Output                                     │
├─────────────────────────────────────────────────────────────────────┤
│  • Transparent overlay window                                         │
│  • FOV circle visualization                                           │
│  • Detection boxes (friend/enemy)                                     │
│  • Sound direction indicators                                         │
│  • ImGui settings menu                                                │
│  • FPS counter                                                        │
└───────────────────────────────────────────────────────────────────────┘
```

## Component Dependencies

```
main.cpp
  ├── OverlayManager (Window creation & management)
  │   └── Windows API (CreateWindowEx, message loop)
  │
  ├── FOVRenderer (Rendering primitives)
  │   ├── DirectX 11 (Device, Context, Buffers)
  │   └── DirectXMath (Vector calculations)
  │
  ├── YOLODetector (Object detection)
  │   ├── Model Loading (ONNX/PyTorch)
  │   └── Inference Pipeline
  │
  ├── ObjectTracker (Multi-object tracking)
  │   └── IOU Matching Algorithm
  │
  ├── SoundTracker (Audio visualization)
  │   └── Direction Analysis
  │
  ├── ConfigManager (Settings)
  │   └── nlohmann/json (JSON parsing)
  │
  └── ImGuiManager (UI)
      └── Dear ImGui (Rendering & input)
```

## Data Flow

```
┌─────────────┐
│   CONFIG    │  settings.json
└──────┬──────┘
       │
       ▼
┌─────────────────────────────────────────────────────────────┐
│                        MAIN LOOP                             │
├─────────────────────────────────────────────────────────────┤
│                                                               │
│  1. Process Input                                             │
│     ├─► Keyboard (INSERT, ESC)                               │
│     └─► Mouse (if not captured by ImGui)                     │
│                                                               │
│  2. Update Systems                                            │
│     ├─► Sound Tracker (audio analysis)                       │
│     ├─► Object Detector (screen capture → inference)         │
│     └─► Object Tracker (match & track)                       │
│                                                               │
│  3. Render Frame                                              │
│     ├─► Clear (transparent background)                       │
│     ├─► FOV Circle (center of screen)                        │
│     ├─► Detection Boxes (friend=green, enemy=red)            │
│     ├─► Sound Indicators (directional arrows/dots)           │
│     └─► ImGui UI (settings menu if enabled)                  │
│                                                               │
│  4. Present Frame                                             │
│     └─► DirectX Swap Chain (vsync optional)                  │
│                                                               │
│  5. Frame Timing                                              │
│     └─► Limit to target FPS (120 default)                    │
│                                                               │
└───────────────────────────────────────────────────────────────┘
       │
       ▼
┌─────────────┐
│   DISPLAY   │  Transparent overlay on screen
└─────────────┘
```

## Configuration Flow

```
┌──────────────┐
│settings.json │
└──────┬───────┘
       │
       ▼
┌───────────────────────────────────────────────────────────┐
│              ConfigManager::LoadFromFile()                 │
├───────────────────────────────────────────────────────────┤
│                                                             │
│  Parse JSON ──┬──► FOVConfig                               │
│               │    • enabled, radius, color, etc.          │
│               │                                             │
│               ├──► DetectionConfig                         │
│               │    • model_path, thresholds, classes       │
│               │                                             │
│               ├──► SoundTrackingConfig                     │
│               │    • sensitivity, ranges, indicators       │
│               │                                             │
│               ├──► OverlayConfig                           │
│               │    • renderer, size, transparency          │
│               │                                             │
│               └──► UIConfig                                │
│                    • show_menu, show_fps, font_size        │
│                                                             │
└───────────────────────────────────────────────────────────┘
       │
       ▼
┌─────────────────────────────────────────────────────────┐
│           Apply to Components                            │
├─────────────────────────────────────────────────────────┤
│  • FOVRenderer::SetConfig(fov_config)                    │
│  • YOLODetector::Initialize(detection_config)            │
│  • SoundTracker::SetConfig(sound_config)                 │
│  • OverlayManager::Initialize(overlay_config)            │
└──────────────────────────────────────────────────────────┘
```

## Rendering Pipeline

```
┌────────────────────────────────────────────────────────────┐
│                    Frame Rendering                          │
├────────────────────────────────────────────────────────────┤
│                                                              │
│  1. BeginFrame()                                             │
│     └─► Set render target, clear to transparent             │
│                                                              │
│  2. Render FOV (if enabled)                                  │
│     ├─► Calculate circle vertices                           │
│     ├─► Set blend state (transparency)                      │
│     ├─► Draw circle primitive                               │
│     └─► Apply color & thickness                             │
│                                                              │
│  3. Render Detections (if enabled)                           │
│     ├─► For each detected object:                           │
│     │   ├─► Determine color (friend/enemy)                  │
│     │   ├─► Draw bounding box                               │
│     │   └─► Draw label (optional)                           │
│     └─► Use ObjectTracker for consistent IDs                │
│                                                              │
│  4. Render Sound Indicators (if enabled)                     │
│     ├─► For each sound source:                              │
│     │   ├─► Convert azimuth/elevation to screen coords      │
│     │   └─► Draw indicator (circle/arrow)                   │
│     └─► Apply y-axis sensitivity                            │
│                                                              │
│  5. Render UI (if menu shown)                                │
│     ├─► ImGui::NewFrame()                                   │
│     ├─► Draw settings windows                               │
│     │   ├─► FOV settings (sliders, color picker)            │
│     │   ├─► Detection settings (thresholds)                 │
│     │   ├─► Sound settings (sensitivity)                    │
│     │   └─► Performance (FPS, vsync)                        │
│     └─► ImGui::Render()                                     │
│                                                              │
│  6. EndFrame()                                               │
│     └─► Present swap chain (display on screen)              │
│                                                              │
└──────────────────────────────────────────────────────────────┘
```

## Class Relationships

```
Application (main.cpp)
    │
    ├── owns ──► OverlayManager
    │              └── creates Windows API window
    │
    ├── owns ──► FOVRenderer
    │              ├── uses DirectX 11 Device/Context
    │              └── renders shapes
    │
    ├── owns ──► YOLODetector
    │              ├── loads ONNX/PyTorch model
    │              └── runs inference
    │
    ├── owns ──► ObjectTracker
    │              ├── uses DetectionBox from YOLODetector
    │              └── produces TrackedObject
    │
    ├── owns ──► SoundTracker
    │              └── produces SoundSource
    │
    ├── owns ──► ConfigManager
    │              ├── loads settings.json
    │              └── provides config structs
    │
    └── owns ──► ImGuiManager
                   ├── uses Dear ImGui library
                   └── renders UI overlay
```

## Thread Model (Current - Single Thread)

```
┌───────────────────────────────────────────────────────────┐
│                      Main Thread                           │
├───────────────────────────────────────────────────────────┤
│                                                             │
│  ┌─────────────────────────────────────────────┐          │
│  │ Message Loop (non-blocking)                  │          │
│  └───────────────┬─────────────────────────────┘          │
│                  │                                          │
│                  ▼                                          │
│  ┌─────────────────────────────────────────────┐          │
│  │ Update (Sound, Detection, Tracking)          │          │
│  └───────────────┬─────────────────────────────┘          │
│                  │                                          │
│                  ▼                                          │
│  ┌─────────────────────────────────────────────┐          │
│  │ Render (FOV, Boxes, UI)                      │          │
│  └───────────────┬─────────────────────────────┘          │
│                  │                                          │
│                  ▼                                          │
│  ┌─────────────────────────────────────────────┐          │
│  │ Present (Display on screen)                  │          │
│  └───────────────┬─────────────────────────────┘          │
│                  │                                          │
│                  └──► Loop (target FPS limiting)            │
│                                                             │
└─────────────────────────────────────────────────────────────┘

Future Enhancement: Multi-threaded Detection
┌──────────────┐    ┌──────────────┐    ┌──────────────┐
│ Main Thread  │    │Detect Thread │    │Sound Thread  │
│ (Rendering)  │◄──►│  (YOLO)      │    │  (Audio)     │
└──────────────┘    └──────────────┘    └──────────────┘
        │                  │                    │
        └──────────────────┴────────────────────┘
                     Mutex/Queue
```

## External Dependencies

```
Application
    │
    ├─► DirectX 11 (Built into Windows)
    │   ├── d3d11.lib
    │   ├── dxgi.lib
    │   └── d3dcompiler.lib
    │
    ├─► Windows API (Built into Windows)
    │   └── System headers
    │
    ├─► Dear ImGui (Third-party, auto-download)
    │   ├── imgui.cpp
    │   ├── imgui_impl_dx11.cpp
    │   └── imgui_impl_win32.cpp
    │
    ├─► nlohmann/json (Third-party, auto-download)
    │   └── json.hpp (header-only)
    │
    └─► YOLOv8 (Optional, manual download)
        ├── Model file (.pt or .onnx)
        └── Runtime (ONNX Runtime or LibTorch)
```

## Build System Architecture

```
Source Files
    │
    ├─► CMakeLists.txt ──► CMake ──► Makefile/VS Solution
    │                                      │
    └─► FOVOverlay.vcxproj ──────────────┘
                                           │
                                           ▼
                                     ┌──────────┐
                                     │ Compiler │
                                     └─────┬────┘
                                           │
                                           ▼
                                    ┌────────────┐
                                    │   Linker   │
                                    └─────┬──────┘
                                          │
                                          ▼
                                 ┌─────────────────┐
                                 │ FOVOverlay.exe  │
                                 └─────────────────┘
                                          │
                                          ▼
                                 ┌─────────────────┐
                                 │  bin/Release/   │
                                 │  + settings.json│
                                 └─────────────────┘
```
