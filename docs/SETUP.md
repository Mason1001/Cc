# Quick Setup Guide

This guide will help you get the Gaming Overlay up and running quickly.

## Prerequisites Installation

### 1. Install Visual Studio 2022

Download and install Visual Studio 2022 Community (free):
https://visualstudio.microsoft.com/downloads/

**Required Workloads**:
- Desktop development with C++
- Windows 10 SDK (10.0.19041.0 or later)

### 2. Install CMake

Download and install CMake 3.15 or later:
https://cmake.org/download/

Make sure to add CMake to your system PATH during installation.

### 3. Install Git

Download and install Git:
https://git-scm.com/downloads

## Project Setup

### Step 1: Clone or Download the Repository

```bash
git clone https://github.com/Mason1001/Cc.git
cd Cc
```

Or download and extract the ZIP file.

### Step 2: Download Dear ImGui

Dear ImGui is required for the user interface:

```bash
# Create libs directory if it doesn't exist
mkdir -p libs
cd libs

# Clone Dear ImGui
git clone https://github.com/ocornut/imgui.git

cd ..
```

### Step 3: (Optional) Download YOLOv8 Model

If you want to use object detection features:

1. Visit: https://github.com/ultralytics/ultralytics
2. Download a YOLOv8 ONNX model (e.g., `yolov8n.onnx`)
3. Place it in `assets/models/yolov8n.onnx`

**Note**: Detection features will work in demo mode without the actual model, but won't perform real detection.

### Step 4: (Optional) Install ONNX Runtime

For full detection capabilities:

1. Download ONNX Runtime from: https://github.com/microsoft/onnxruntime/releases
2. Download the Windows x64 version (e.g., `onnxruntime-win-x64-1.16.3.zip`)
3. Extract to `libs/onnxruntime/`

## Building the Project

### Quick Build (Recommended)

**Windows PowerShell**:
```powershell
.\build.ps1
```

This will:
1. Create a `build` directory
2. Configure CMake
3. Build the project in Release mode
4. Output executable to `build/bin/Release/GamingOverlay.exe`

### Manual Build

If you prefer to build manually:

```bash
# Create build directory
mkdir build
cd build

# Configure with CMake
cmake .. -G "Visual Studio 17 2022" -A x64

# Build
cmake --build . --config Release

# Executable will be at: build/bin/Release/GamingOverlay.exe
```

### Visual Studio

1. Open Visual Studio 2022
2. Select "Open a local folder"
3. Navigate to the project directory
4. Visual Studio will detect CMakeLists.txt
5. Select "x64-Release" configuration from the dropdown
6. Build > Build All (Ctrl+Shift+B)

## Running the Application

### First Run

1. Navigate to the executable location:
   ```
   cd build/bin/Release
   ```

2. Run the application:
   ```
   GamingOverlay.exe
   ```

3. The control panel will appear with all settings

### Configuration

On first run, default settings will be created in `config/settings.json`.

**Default Settings**:
- FOV Overlay: Enabled (yellow circle, 100px radius)
- Detection: Disabled (requires ONNX model)
- Tracking: Disabled
- Theme: Dark

### Testing Features

#### Test FOV Overlay

1. Enable "FOV Overlay" (should be on by default)
2. Adjust the size slider
3. Change the color using the color picker
4. Adjust transparency

You should see a colored circle in the center of your screen.

#### Test Detection (Demo Mode)

1. Enable "Object Detection"
2. You should see animated bounding boxes (demo data)
3. Red boxes = enemies, Green boxes = friends

**Note**: This is demo mode. For real detection, install ONNX Runtime and YOLOv8 model.

#### Test Tracking

1. Enable "Tracking System"
2. Adjust precision, smoothness, and speed
3. The system will track demo targets

#### Test Themes

1. Click "Light Theme" button
2. UI should change to light colors
3. Click "Dark Theme" to switch back

#### Test Settings Persistence

1. Change some settings
2. Click "Save Profile"
3. Close the application
4. Reopen - your settings should be restored

## Troubleshooting Quick Fixes

### Build Fails

**"imgui.h not found"**
```bash
cd libs
git clone https://github.com/ocornut/imgui.git
```

**"d3d11.h not found"**
- Reinstall Visual Studio with C++ workload
- Ensure Windows SDK is installed

**CMake errors**
```bash
# Delete build directory and try again
rm -rf build
mkdir build
cd build
cmake .. -G "Visual Studio 17 2022" -A x64
```

### Runtime Issues

**Application crashes on start**
- Update graphics drivers
- Install DirectX End-User Runtime
- Install Visual C++ Redistributable 2022

**Black screen**
- Check if window is on correct monitor
- Verify DirectX 11 is supported by GPU

**No detection**
- This is expected without ONNX Runtime
- Demo mode will show animated boxes
- Install ONNX Runtime for real detection

### Performance Issues

**Low FPS**
- Disable detection if not needed
- Reduce FOV circle segments in code
- Lower overlay complexity

**High CPU/GPU usage**
- Normal for overlays and detection
- Reduce detection frequency
- Disable unused features

## Directory Structure After Setup

```
Cc/
├── build/                    # Build output (created after build)
│   └── bin/
│       └── Release/
│           └── GamingOverlay.exe
├── libs/
│   └── imgui/               # Dear ImGui (downloaded)
├── assets/
│   └── models/
│       └── yolov8n.onnx     # (optional, downloaded)
├── config/
│   └── settings.json        # (created on first run)
├── src/                     # Source code
├── include/                 # Headers
├── docs/                    # Documentation
├── CMakeLists.txt
├── build.ps1
├── build.sh
└── README.md
```

## Next Steps

1. **Read the full README.md** for detailed feature descriptions
2. **Check docs/API.md** for API documentation
3. **Review docs/TROUBLESHOOTING.md** for common issues
4. **Customize settings** through the UI or JSON file
5. **Experiment with features** to find your preferred configuration

## Quick Reference

### Default Keybindings

- **Insert**: Toggle UI visibility (planned feature)

### Configuration File

Location: `config/settings.json`

Edit manually or use UI controls.

### Build Configurations

- **Debug**: `cmake --build . --config Debug`
- **Release**: `cmake --build . --config Release`

### Rebuild

```bash
cd build
cmake --build . --config Release --clean-first
```

### Clean Build

```bash
rm -rf build
mkdir build
cd build
cmake .. -G "Visual Studio 17 2022" -A x64
cmake --build . --config Release
```

## Support

- **Documentation**: Check `docs/` directory
- **Issues**: Open issue on GitHub
- **Questions**: Review README.md and API.md first

## Summary Checklist

- [ ] Visual Studio 2022 installed
- [ ] CMake installed
- [ ] Git installed
- [ ] Dear ImGui downloaded to `libs/imgui/`
- [ ] (Optional) YOLOv8 model in `assets/models/`
- [ ] (Optional) ONNX Runtime in `libs/onnxruntime/`
- [ ] Project built successfully
- [ ] Application runs and shows UI
- [ ] FOV overlay visible and working
- [ ] Settings save and load correctly

If all items are checked, you're ready to use the Gaming Overlay!

---

**Enjoy your gaming overlay!** 🎮
