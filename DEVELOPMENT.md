# Development Guide

## Setting Up Development Environment

### Required Tools
1. Visual Studio 2019 or 2022 with C++ Desktop Development workload
2. CMake 3.15 or later
3. Git for version control
4. Windows SDK (included with Visual Studio)

### Recommended Tools
- Visual Studio Code (for quick edits)
- RenderDoc (for graphics debugging)
- Windows Performance Analyzer (for profiling)

## Project Structure

```
Cc/
├── include/              # Public headers
│   ├── overlay/         # Overlay window and rendering
│   ├── detection/       # Object detection and tracking
│   ├── sound/           # Sound tracking
│   ├── config/          # Configuration management
│   └── ui/              # User interface
├── src/                 # Implementation files (mirrors include/)
├── external/            # Third-party dependencies
├── models/              # YOLOv8 model files
├── build/               # Build output (generated)
└── docs/                # Documentation
```

## Building from Source

### Quick Build
```bash
# Windows
build.bat

# Linux (if cross-compiling)
./build.sh
```

### Manual Build
```bash
mkdir build && cd build
cmake .. -G "Visual Studio 17 2022" -A x64
cmake --build . --config Release
```

### Debug Build
```bash
cmake --build . --config Debug
```

## Coding Standards

### Style Guide
- Use C++17 features
- Follow Google C++ Style Guide
- Use `snake_case` for variables and functions
- Use `PascalCase` for classes and structs
- Use `UPPER_CASE` for constants and macros

### Header Files
```cpp
#pragma once

namespace fov {

class MyClass {
public:
    // Public interface
    void DoSomething();
    
private:
    // Private implementation
    int private_member_;
};

} // namespace fov
```

### Implementation Files
```cpp
#include "path/to/header.h"

namespace fov {

void MyClass::DoSomething() {
    // Implementation
}

} // namespace fov
```

## Architecture

### Core Systems

#### 1. Overlay System
- **OverlayManager**: Creates and manages the transparent overlay window
- **FOVRenderer**: DirectX 11 rendering for circles, lines, and shapes

#### 2. Detection System
- **YOLODetector**: Wraps YOLOv8 inference
- **ObjectTracker**: Tracks objects across frames using IOU matching

#### 3. Sound System
- **SoundTracker**: Analyzes audio for directional information

#### 4. Configuration System
- **ConfigManager**: Loads/saves JSON configuration

#### 5. UI System
- **ImGuiManager**: Manages Dear ImGui for settings interface

### Data Flow

```
┌──────────────┐
│  Main Loop   │
└──────┬───────┘
       │
       ├─> Process Input (Keyboard/Mouse)
       │
       ├─> Update Systems
       │   ├─> Sound Tracker
       │   ├─> Object Detector
       │   └─> Object Tracker
       │
       ├─> Render Frame
       │   ├─> Clear Buffer
       │   ├─> FOV Renderer
       │   ├─> Detection Boxes
       │   ├─> Sound Indicators
       │   └─> ImGui UI
       │
       └─> Present Frame
```

## Adding New Features

### Example: Adding a New Renderer Shape

1. **Add method to FOVRenderer.h**:
```cpp
void RenderTriangle(float x, float y, float size, const float color[4]);
```

2. **Implement in FOVRenderer.cpp**:
```cpp
void FOVRenderer::RenderTriangle(float x, float y, float size, const float color[4]) {
    // Create vertices
    std::vector<DirectX::XMFLOAT2> vertices;
    vertices.push_back(DirectX::XMFLOAT2(x, y - size));
    vertices.push_back(DirectX::XMFLOAT2(x - size, y + size));
    vertices.push_back(DirectX::XMFLOAT2(x + size, y + size));
    
    // Render (implementation details)
}
```

3. **Use in main.cpp**:
```cpp
fov_renderer_->RenderTriangle(100, 100, 50, red_color);
```

## Debugging

### Common Issues

#### DirectX Device Creation Failed
- Check Windows SDK installation
- Verify graphics drivers are up to date
- Run as administrator if necessary

#### Low FPS
- Profile with Visual Studio Performance Profiler
- Check GPU usage in Task Manager
- Reduce FOV complexity or detection frequency

#### Detection Not Working
- Verify model file path in settings.json
- Check model format (ONNX vs PyTorch)
- Ensure ONNX Runtime is installed

### Debugging Tools

#### Visual Studio Debugger
- Set breakpoints in critical paths
- Watch window for variable inspection
- Call stack analysis

#### RenderDoc
1. Launch application through RenderDoc
2. Capture frame
3. Analyze draw calls and GPU state

#### Graphics Debugging
```cpp
// Enable D3D11 debug layer
#ifdef _DEBUG
create_flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
```

## Performance Optimization

### Profiling
- Use Visual Studio Performance Profiler
- Monitor FPS counter in application
- Check GPU utilization

### Optimization Tips
1. **Reduce draw calls**: Batch rendering operations
2. **Use instancing**: For repeated shapes
3. **Optimize shaders**: Minimize pixel shader complexity
4. **Buffer management**: Use dynamic buffers efficiently

### Target Metrics
- **FPS**: 120+ for smooth overlay
- **Frame time**: < 8.33ms
- **CPU usage**: < 10%
- **GPU usage**: < 20%

## Testing

### Manual Testing Checklist
- [ ] Overlay window appears correctly
- [ ] FOV circle renders at correct position
- [ ] Settings menu toggles with INSERT key
- [ ] Application exits cleanly with ESC
- [ ] Configuration loads from settings.json
- [ ] FPS counter displays accurate value

### Performance Testing
```bash
# Run for 5 minutes and monitor:
- Average FPS
- Frame time variance
- Memory usage
- GPU utilization
```

## Contributing

### Pull Request Process
1. Fork the repository
2. Create feature branch (`git checkout -b feature/amazing-feature`)
3. Commit changes (`git commit -m 'Add amazing feature'`)
4. Push to branch (`git push origin feature/amazing-feature`)
5. Open Pull Request

### Code Review Checklist
- [ ] Code follows style guide
- [ ] No compiler warnings
- [ ] Documentation updated
- [ ] Manual testing performed
- [ ] Performance impact assessed

## Resources

### DirectX 11
- [Microsoft DirectX Documentation](https://docs.microsoft.com/en-us/windows/win32/direct3d11/dx-graphics-overviews)
- [DirectX SDK Samples](https://github.com/microsoft/DirectX-Graphics-Samples)

### Dear ImGui
- [Official Documentation](https://github.com/ocornut/imgui/wiki)
- [Demo Window Source](https://github.com/ocornut/imgui/blob/master/imgui_demo.cpp)

### YOLOv8
- [Ultralytics Docs](https://docs.ultralytics.com/)
- [YOLOv8 Paper](https://arxiv.org/abs/2305.09972)

### C++ Best Practices
- [C++ Core Guidelines](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines)
- [Modern C++ Features](https://github.com/AnthonyCalandra/modern-cpp-features)
