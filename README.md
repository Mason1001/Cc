# FOV Overlay Application with YOLOv8 Integration

A real-time Field of View (FOV) overlay application featuring object detection, sound tracking, and customizable visualization using DirectX 11 and Dear ImGui.

## Features

### 1. Real-time FOV Rendering
- DirectX 11-based overlay rendering optimized for 120+ FPS
- Customizable FOV circle (radius, color, thickness, fill)
- Transparent overlay window with topmost positioning
- Low-latency rendering pipeline

### 2. YOLOv8 Object Detection
- Real-time object detection using YOLOv8 models
- Friend/enemy classification system
- Object tracking with velocity estimation
- Configurable confidence and IoU thresholds
- Visual bounding boxes with color-coded tags

### 3. Sound Tracking
- 3D sound source visualization
- Y-axis sensitivity configuration
- Horizontal and vertical range adjustments
- Visual indicators for sound direction

### 4. Configuration Management
- JSON-based configuration file (settings.json)
- Hot-reload support for live configuration updates
- Per-feature enable/disable options
- Color customization for all visual elements

### 5. User Interface
- Dear ImGui integration for in-overlay settings
- Toggle menu with INSERT key
- Real-time FPS display
- Adjustable font size and UI elements

## Building the Project

### Prerequisites
- Windows 10/11
- Visual Studio 2019 or later (with C++17 support)
- CMake 3.15 or later
- DirectX 11 SDK (included with Windows SDK)

### Dependencies
- Dear ImGui (included in external/)
- nlohmann/json (for JSON parsing)
- YOLOv8 model files (ONNX Runtime or PyTorch C++)

### Build Steps

1. Clone the repository:
```bash
git clone https://github.com/Mason1001/Cc.git
cd Cc
```

2. Create build directory:
```bash
mkdir build
cd build
```

3. Generate Visual Studio solution:
```bash
cmake ..
```

4. Build the project:
```bash
cmake --build . --config Release
```

5. Run the application:
```bash
cd bin/Release
FOVOverlay.exe
```

## Configuration

Edit `settings.json` to customize the application:

### FOV Settings
```json
"fov": {
  "enabled": true,
  "radius": 100,
  "color": {"r": 255, "g": 0, "b": 0, "a": 200},
  "thickness": 2,
  "filled": false,
  "target_fps": 120
}
```

### Detection Settings
```json
"detection": {
  "enabled": true,
  "model_path": "models/yolov8n.pt",
  "confidence_threshold": 0.5,
  "iou_threshold": 0.45,
  "friend_color": {"r": 0, "g": 255, "b": 0, "a": 255},
  "enemy_color": {"r": 255, "g": 0, "b": 0, "a": 255},
  "friend_classes": [0],
  "enemy_classes": [1, 2, 3]
}
```

### Sound Tracking Settings
```json
"sound_tracking": {
  "enabled": true,
  "y_axis_sensitivity": 1.0,
  "horizontal_range": 180,
  "vertical_range": 90,
  "indicator_color": {"r": 255, "g": 255, "b": 0, "a": 200},
  "indicator_size": 10
}
```

## Usage

1. Launch the application - an overlay window will appear
2. Press **INSERT** to toggle the settings menu
3. Press **ESC** to exit the application

### Keyboard Controls
- `INSERT`: Toggle settings menu
- `ESC`: Exit application

### Runtime Configuration
All settings can be adjusted in real-time through the ImGui menu interface.

## Performance Optimization

The application is optimized for high-performance rendering:

- **Target FPS**: Configurable (default 120)
- **VSync**: Disabled by default for minimal latency
- **DirectX 11**: Hardware-accelerated rendering
- **Multi-threaded**: Separate threads for detection and rendering (when implemented)

## YOLOv8 Integration

### Model Setup
1. Download YOLOv8 model (e.g., yolov8n.pt)
2. Place in `models/` directory
3. Update path in `settings.json`

### Supported Formats
- PyTorch (.pt)
- ONNX (.onnx)

## Architecture

### Core Components

1. **OverlayManager**: Window creation and message handling
2. **FOVRenderer**: DirectX 11 rendering for FOV and visual elements
3. **YOLODetector**: YOLOv8 model inference wrapper
4. **ObjectTracker**: Multi-object tracking with velocity estimation
5. **SoundTracker**: Audio direction analysis and visualization
6. **ConfigManager**: JSON configuration file management
7. **ImGuiManager**: User interface rendering

### Directory Structure
```
Cc/
├── include/           # Header files
│   ├── overlay/
│   ├── detection/
│   ├── sound/
│   ├── config/
│   └── ui/
├── src/               # Implementation files
│   ├── overlay/
│   ├── detection/
│   ├── sound/
│   ├── config/
│   ├── ui/
│   └── main.cpp
├── external/          # Third-party libraries
│   ├── imgui/
│   └── json/
├── models/            # YOLOv8 model files
├── settings.json      # Configuration file
└── CMakeLists.txt     # Build configuration
```

## Advanced Features

### Object Detection Pipeline
1. Screen capture (implementation required)
2. Image preprocessing
3. YOLOv8 inference
4. Non-Maximum Suppression (NMS)
5. Object tracking
6. Visual rendering

### Sound Tracking Algorithm
1. Audio capture via Windows Core Audio API
2. Direction-of-arrival (DOA) estimation
3. Y-axis sensitivity adjustment
4. Visual indicator positioning

## Troubleshooting

### Common Issues

**Application won't start**
- Ensure DirectX 11 is installed (comes with Windows 10/11)
- Check if overlay window is being created (may be hidden)

**Low FPS**
- Reduce target FPS in settings.json
- Disable VSync
- Lower detection resolution

**Detection not working**
- Verify model file exists in specified path
- Check model format compatibility
- Ensure sufficient GPU memory

## Development

### Adding New Features
1. Create header in appropriate `include/` subdirectory
2. Implement in corresponding `src/` subdirectory
3. Update CMakeLists.txt if adding new files
4. Update settings.json with new configuration options

### Code Style
- C++17 standard
- Header guards for all headers
- Namespace: `fov`
- Comments for public APIs

## License

[Specify license]

## Contributing

Contributions are welcome! Please submit pull requests or open issues for bugs and feature requests.

## Acknowledgments

- YOLOv8 by Ultralytics
- Dear ImGui by Omar Cornut
- DirectX 11 by Microsoft