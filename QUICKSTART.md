# Quick Start Guide

Get up and running with FOV Overlay in 5 minutes!

## Prerequisites

- Windows 10/11
- Visual Studio 2019 or later (with C++ workload)
- Git

## Step 1: Clone Repository

```bash
git clone https://github.com/Mason1001/Cc.git
cd Cc
```

## Step 2: Setup Dependencies

Run the dependency setup script:

```bash
# Windows
setup_dependencies.bat

# Linux
./setup_dependencies.sh
```

This will download:
- Dear ImGui (for UI)
- nlohmann/json (for configuration)

## Step 3: Download YOLOv8 Model (Optional)

If you want object detection functionality:

```bash
# Install ultralytics Python package
pip install ultralytics

# Download YOLOv8 nano model
python -c "from ultralytics import YOLO; YOLO('yolov8n.pt')"

# Move to models directory
move yolov8n.pt models\
```

Or download manually from: https://github.com/ultralytics/assets/releases

## Step 4: Build the Project

```bash
# Windows
build.bat

# Or use CMake directly
mkdir build && cd build
cmake .. -G "Visual Studio 17 2022" -A x64
cmake --build . --config Release
```

## Step 5: Configure Settings

Edit `settings.json` to customize:

```json
{
  "fov": {
    "enabled": true,
    "radius": 100,
    "color": {"r": 255, "g": 0, "b": 0, "a": 200}
  }
}
```

## Step 6: Run the Application

```bash
cd build\bin\Release
FOVOverlay.exe
```

Or if built with Visual Studio, find the executable in the output directory.

## Basic Controls

- **INSERT**: Toggle settings menu
- **ESC**: Exit application

## Verify Installation

You should see:
1. A transparent overlay window
2. Red FOV circle in the center
3. FPS counter in console
4. Settings menu (press INSERT)

## Common Issues

### "Cannot find settings.json"
- Ensure you're running from the correct directory
- The file should be in the same folder as the executable

### "Failed to initialize DirectX"
- Update graphics drivers
- Run as administrator
- Check Windows Update for DirectX updates

### "YOLO model not found"
- Check the path in settings.json
- Ensure the model file exists in models/ directory
- You can disable detection by setting `"enabled": false` in settings.json

## Next Steps

- Read [README.md](README.md) for detailed features
- Check [API.md](API.md) for programming reference
- See [DEVELOPMENT.md](DEVELOPMENT.md) for development guide
- Explore [settings.json](settings.json) for all options

## Getting Help

- Open an issue on GitHub
- Check existing documentation
- Review example configurations

## Example Configurations

### High-Performance Gaming
```json
{
  "fov": {
    "target_fps": 144,
    "radius": 80
  },
  "overlay": {
    "vsync": false
  }
}
```

### Object Detection Focus
```json
{
  "detection": {
    "enabled": true,
    "confidence_threshold": 0.6,
    "friend_classes": [0],
    "enemy_classes": [1, 2, 3, 14, 15, 16]
  }
}
```

### Minimal FOV Only
```json
{
  "fov": {
    "enabled": true,
    "radius": 100
  },
  "detection": {
    "enabled": false
  },
  "sound_tracking": {
    "enabled": false
  }
}
```

Happy overlaying! 🎯
