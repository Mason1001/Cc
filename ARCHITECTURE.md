# Gaming Overlay - System Architecture

```
┌─────────────────────────────────────────────────────────────────────────┐
│                          GAMING OVERLAY APPLICATION                      │
│                                                                          │
│  ┌────────────────────────────────────────────────────────────────────┐ │
│  │                         MAIN APPLICATION                           │ │
│  │                          (main.cpp)                                │ │
│  │                                                                    │ │
│  │  • Window Management                                               │ │
│  │  • System Initialization                                           │ │
│  │  • Main Loop (120 FPS)                                            │ │
│  │  • Event Handling                                                 │ │
│  │  • Resource Cleanup                                               │ │
│  └────────────────────────────────────────────────────────────────────┘ │
│                                   │                                      │
│                                   ▼                                      │
│  ┌─────────────────────┬──────────────────────┬──────────────────────┐ │
│  │                     │                      │                      │ │
│  ▼                     ▼                      ▼                      ▼ │
│ ┌──────────────┐  ┌──────────────┐  ┌──────────────┐  ┌──────────────┐│
│ │   OVERLAY    │  │   RENDERER   │  │ UI MANAGER   │  │  SETTINGS   ││
│ │   WINDOW     │  │  (DirectX11) │  │  (ImGui)     │  │  MANAGER    ││
│ │              │  │              │  │              │  │  (JSON)     ││
│ ├──────────────┤  ├──────────────┤  ├──────────────┤  ├──────────────┤│
│ │ • Create     │  │ • Initialize │  │ • Dark Theme │  │ • Load      ││
│ │ • Handle     │  │ • BeginFrame │  │ • Light Theme│  │ • Save      ││
│ │ • Resize     │  │ • EndFrame   │  │ • Controls   │  │ • Reset     ││
│ │ • Destroy    │  │ • FOV Circle │  │ • Sliders    │  │ • Profile   ││
│ │              │  │ • BBox       │  │ • Buttons    │  │ • Validate  ││
│ │              │  │ • Shapes     │  │ • Status     │  │             ││
│ └──────────────┘  └──────────────┘  └──────────────┘  └──────────────┘│
│                                   │                                      │
│                                   ▼                                      │
│  ┌─────────────────────────────────────────────────────────────────┐   │
│  │                     FEATURE SUBSYSTEMS                          │   │
│  │                                                                 │   │
│  │  ┌─────────────────┐  ┌─────────────────┐  ┌────────────────┐ │   │
│  │  │ OBJECT DETECTOR │  │ TRACKING SYSTEM │  │  DETECTION     │ │   │
│  │  │   (YOLOv8)      │  │  (Aim Assist)   │  │   RESULTS      │ │   │
│  │  ├─────────────────┤  ├─────────────────┤  ├────────────────┤ │   │
│  │  │ • Initialize    │  │ • Update        │  │ • TopLeft      │ │   │
│  │  │ • Update        │  │ • SetTarget     │  │ • BottomRight  │ │   │
│  │  │ • GetDetections │  │ • ClearTarget   │  │ • Confidence   │ │   │
│  │  │ • RunInference  │  │ • GetPosition   │  │ • ClassID      │ │   │
│  │  │ • Process       │  │ • Smooth Track  │  │ • IsEnemy      │ │   │
│  │  │ • Filter        │  │ • Natural Move  │  │                │ │   │
│  │  └─────────────────┘  └─────────────────┘  └────────────────┘ │   │
│  └─────────────────────────────────────────────────────────────────┘   │
└─────────────────────────────────────────────────────────────────────────┘

┌─────────────────────────────────────────────────────────────────────────┐
│                           DATA FLOW DIAGRAM                              │
└─────────────────────────────────────────────────────────────────────────┘

  User Input                  Screen Capture              Configuration
      │                             │                           │
      ▼                             ▼                           ▼
┌──────────┐                ┌──────────────┐            ┌──────────────┐
│ Keyboard │───────────────▶│Object Detector│◀───────────│  Settings    │
│  Mouse   │                │  (YOLOv8)    │            │  Manager     │
└──────────┘                └──────────────┘            └──────────────┘
      │                             │                           │
      │                             ▼                           │
      │                      ┌──────────────┐                   │
      └─────────────────────▶│  Detections  │                   │
                             │   Results    │                   │
                             └──────────────┘                   │
                                     │                           │
                                     ▼                           │
                             ┌──────────────┐                   │
                             │  Tracking    │◀──────────────────┘
                             │   System     │
                             └──────────────┘
                                     │
                                     ▼
                             ┌──────────────┐
                             │   Renderer   │
                             │ (DirectX 11) │
                             └──────────────┘
                                     │
                                     ▼
                             ┌──────────────┐
                             │    Screen    │
                             │   Display    │
                             └──────────────┘

┌─────────────────────────────────────────────────────────────────────────┐
│                        RENDERING PIPELINE                                │
└─────────────────────────────────────────────────────────────────────────┘

  BeginFrame()
      │
      ├─▶ Clear RenderTarget (Transparent Black)
      │
      ├─▶ Set Viewport & Shaders
      │
      ├─▶ Enable Alpha Blending
      │
      ├─▶ If FOV Enabled:
      │      └─▶ RenderCircle(center, radius, color, transparency)
      │             └─▶ Generate 64 line segments
      │                    └─▶ DrawLine() x 64
      │
      ├─▶ If Detection Enabled:
      │      └─▶ For each detection:
      │             ├─▶ Set color (red=enemy, green=friend)
      │             └─▶ RenderBox(topLeft, bottomRight, color)
      │                    └─▶ DrawLine() x 4 (box outline)
      │
      ├─▶ Render ImGui UI:
      │      ├─▶ Control Panel
      │      ├─▶ Theme Selection
      │      ├─▶ Sliders & Switches
      │      ├─▶ Color Picker
      │      └─▶ Status Information
      │
      └─▶ EndFrame()
             └─▶ Present() to screen

┌─────────────────────────────────────────────────────────────────────────┐
│                      CONFIGURATION SYSTEM                                │
└─────────────────────────────────────────────────────────────────────────┘

  config/settings.json
         │
         ├─▶ FOV Overlay Settings
         │      ├─ Enabled (bool)
         │      ├─ Size (float: 10-500)
         │      ├─ Color (RGBA: [0-1, 0-1, 0-1, 0-1])
         │      └─ Transparency (float: 0-1)
         │
         ├─▶ Detection Settings
         │      ├─ Enabled (bool)
         │      ├─ Confidence (float: 0-1)
         │      └─ Highlight Intensity (float: 0-2)
         │
         ├─▶ Tracking Settings
         │      ├─ Enabled (bool)
         │      ├─ Precision (float: 0-1)
         │      ├─ Smoothness (float: 0-1)
         │      └─ Speed (float: 0.1-10)
         │
         └─▶ UI Settings
                └─ Dark Theme (bool)

┌─────────────────────────────────────────────────────────────────────────┐
│                        THREADING MODEL                                   │
└─────────────────────────────────────────────────────────────────────────┘

  Main Thread (120 FPS Target)
      │
      ├─▶ Window Events (PeekMessage)
      │
      ├─▶ Update Systems (Delta Time Based)
      │      ├─ Object Detector Update
      │      └─ Tracking System Update
      │
      ├─▶ Render Frame
      │      ├─ FOV Overlay
      │      ├─ Detections
      │      └─ UI
      │
      └─▶ Present to Screen

  Future: Separate Detection Thread (Async)
      └─▶ Background inference for better performance

┌─────────────────────────────────────────────────────────────────────────┐
│                       MEMORY MANAGEMENT                                  │
└─────────────────────────────────────────────────────────────────────────┘

  Smart Pointers (RAII)
      │
      ├─▶ std::unique_ptr for owned resources
      │      ├─ g_window
      │      ├─ g_renderer
      │      ├─ g_uiManager
      │      ├─ g_settingsManager
      │      ├─ g_objectDetector
      │      └─ g_trackingSystem
      │
      ├─▶ DirectX ComPtr for COM objects
      │      ├─ ID3D11Device
      │      ├─ ID3D11DeviceContext
      │      ├─ IDXGISwapChain
      │      ├─ ID3D11RenderTargetView
      │      └─ Shaders & Buffers
      │
      └─▶ Automatic cleanup on scope exit

┌─────────────────────────────────────────────────────────────────────────┐
│                         ERROR HANDLING                                   │
└─────────────────────────────────────────────────────────────────────────┘

  Initialization Phase
      │
      ├─▶ Check prerequisites
      ├─▶ Validate configuration
      ├─▶ Initialize subsystems
      └─▶ Handle failures gracefully
             ├─ Log to console
             ├─ Show error dialog
             └─ Clean exit

  Runtime Phase
      │
      ├─▶ Validate all inputs
      ├─▶ Handle missing resources
      ├─▶ Recover from errors
      └─▶ Continue operation

┌─────────────────────────────────────────────────────────────────────────┐
│                         EXTENSIBILITY                                    │
└─────────────────────────────────────────────────────────────────────────┘

  Plugin Points
      │
      ├─▶ Custom Detectors (inherit ObjectDetector)
      ├─▶ Custom Renderers (extend Renderer)
      ├─▶ Custom UI Themes (modify UIManager)
      ├─▶ Custom Tracking (modify TrackingSystem)
      └─▶ Custom Settings (extend AppSettings)

  Integration Points
      │
      ├─▶ ONNX Runtime (detection backend)
      ├─▶ Different models (detection variants)
      ├─▶ Screen capture APIs (input source)
      └─▶ Input injection (output action)
```

## Key Architecture Decisions

### 1. **Modular Design**
- Each component is self-contained
- Clear interfaces between modules
- Easy to test and maintain

### 2. **Smart Resource Management**
- RAII principles throughout
- Smart pointers for automatic cleanup
- No manual memory management
- DirectX ComPtr for COM objects

### 3. **Performance First**
- Hardware acceleration (DirectX 11)
- Efficient rendering pipeline
- Minimal state changes
- Optimized update loops

### 4. **Extensibility**
- Plugin architecture ready
- Virtual functions for overriding
- Configuration-driven behavior
- Easy to add features

### 5. **Error Resilience**
- Graceful degradation
- Comprehensive error handling
- Clear error messages
- Recovery mechanisms

## Component Responsibilities

### OverlayWindow
- Window creation and management
- Event handling
- Screen positioning
- Transparency control

### Renderer
- DirectX 11 initialization
- Frame rendering
- Shape drawing primitives
- Shader management
- Resource lifecycle

### UIManager
- ImGui integration
- Theme management
- Control widgets
- User interaction
- Status display

### SettingsManager
- Configuration persistence
- JSON serialization
- Profile management
- Validation
- Default handling

### ObjectDetector
- Model loading
- Inference execution
- Result processing
- Detection filtering
- Demo mode

### TrackingSystem
- Target acquisition
- Smooth movement
- Natural behavior
- Parameter tuning
- Position calculation

## Performance Characteristics

- **Frame Rate**: 120 FPS (8.3ms per frame)
- **CPU Usage**: <5% (without detection)
- **GPU Usage**: <10%
- **Memory**: 100-200 MB
- **Latency**: <1 frame (sub-8ms)
- **Startup Time**: <1 second

## Scalability

- **Multiple Monitors**: Supported
- **Various Resolutions**: 720p to 4K+
- **Different GPUs**: Any DX11 compatible
- **Model Sizes**: Nano to Extra Large
- **Detection Rate**: Configurable (10-120 FPS)
