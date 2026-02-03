# Troubleshooting Guide

This guide helps resolve common issues you may encounter when building or running the Gaming Overlay application.

## Table of Contents

1. [Build Issues](#build-issues)
2. [Runtime Issues](#runtime-issues)
3. [Performance Issues](#performance-issues)
4. [Detection Issues](#detection-issues)
5. [UI Issues](#ui-issues)

---

## Build Issues

### CMake Configuration Fails

**Problem**: CMake fails to configure the project

**Solutions**:
- Ensure CMake 3.15 or later is installed
- Check that Visual Studio 2022 is installed with C++ workload
- Run CMake from Visual Studio Developer Command Prompt
- Delete `build/` directory and try again

**Example**:
```bash
# Remove build directory
rm -rf build

# Try again
mkdir build
cd build
cmake .. -G "Visual Studio 17 2022" -A x64
```

### Missing Dear ImGui

**Problem**: Build fails with "imgui.h not found"

**Solution**:
```bash
cd libs
git clone https://github.com/ocornut/imgui.git
```

### DirectX SDK Not Found

**Problem**: "d3d11.h not found" or similar DirectX errors

**Solutions**:
- Install Windows SDK (comes with Visual Studio)
- Ensure you have Windows 10 SDK version 10.0.19041.0 or later
- Reinstall Visual Studio with "Desktop development with C++" workload

### Linker Errors

**Problem**: Unresolved external symbols or linker errors

**Solutions**:
- Ensure all source files are included in CMakeLists.txt
- Verify all required libraries are linked (d3d11.lib, d3dcompiler.lib)
- Clean and rebuild the project

```bash
cmake --build . --config Release --clean-first
```

---

## Runtime Issues

### Application Crashes on Startup

**Problem**: Application crashes immediately when launched

**Possible Causes & Solutions**:

1. **Missing DirectX Runtime**
   - Install DirectX End-User Runtime
   - Download from: https://www.microsoft.com/en-us/download/details.aspx?id=35

2. **Outdated Graphics Drivers**
   - Update to latest GPU drivers
   - NVIDIA: https://www.nvidia.com/drivers
   - AMD: https://www.amd.com/support

3. **Missing Visual C++ Redistributable**
   - Install Visual C++ 2022 Redistributable
   - Download from Microsoft website

4. **Config File Issues**
   - Delete `config/settings.json` to force defaults
   - Create `config/` directory if missing

### Black Screen / No Overlay Visible

**Problem**: Application runs but nothing is visible on screen

**Solutions**:

1. **Check Window Transparency**
   - The overlay uses layered windows
   - Ensure Desktop Window Manager is running

2. **Monitor Configuration**
   - Try running on primary monitor
   - Check if window is offscreen

3. **DirectX Initialization**
   - Check console output for errors
   - Verify GPU supports DirectX 11

### Console Window Not Showing

**Problem**: Can't see debug output

**Solution**:
- The console is allocated in the code
- If not visible, check if stdout is redirected
- Add debug breakpoints to verify execution

---

## Performance Issues

### Low FPS

**Problem**: Application runs at less than 60 FPS

**Solutions**:

1. **Disable VSync**
   - Already disabled by default
   - Check `Present(0, 0)` in renderer.cpp

2. **Reduce Overlay Complexity**
   - Decrease FOV circle segments (currently 64)
   - Disable unused features

3. **Graphics Settings**
   - Lower screen resolution
   - Reduce FOV size
   - Disable detection when not needed

### High CPU Usage

**Problem**: Application uses excessive CPU

**Solutions**:

1. **Frame Rate Limiting**
   - Adjust target frame time in main.cpp
   - Consider adding sleep if needed

2. **Detection Optimization**
   - Increase detection interval
   - Use smaller YOLOv8 model
   - Reduce detection confidence threshold

### High GPU Usage

**Problem**: Application uses too much GPU

**Solutions**:

1. **Rendering Optimization**
   - Reduce overlay complexity
   - Lower circle segment count
   - Disable unnecessary rendering passes

2. **Resolution Scaling**
   - Reduce render target resolution
   - Use lower FOV sizes

### Memory Leaks

**Problem**: Memory usage increases over time

**Solutions**:
- All resources use smart pointers
- Check for unreleased DirectX resources
- Monitor with Task Manager or Performance Profiler

---

## Detection Issues

### "Detector Not Available" Message

**Problem**: Detection features are disabled

**Causes**:
1. Missing ONNX Runtime library
2. Missing YOLOv8 model file
3. Model file in wrong location

**Solutions**:

1. **Install ONNX Runtime**
   ```bash
   # Download from https://github.com/microsoft/onnxruntime/releases
   # Extract to libs/onnxruntime/
   ```

2. **Download Model**
   ```bash
   # Place yolov8n.onnx in assets/models/
   ```

3. **Verify Paths**
   - Ensure model path is correct in main.cpp
   - Check console output for loading errors

### No Objects Detected

**Problem**: Detection is enabled but finds nothing

**Solutions**:

1. **Lower Confidence Threshold**
   - Adjust slider to 0.3 or lower
   - Check if objects are in FOV

2. **Model Compatibility**
   - Ensure model is YOLOv8 ONNX format
   - Try different model version

3. **Input Preprocessing**
   - Verify screen capture is working
   - Check image format and resolution

### False Detections

**Problem**: Too many incorrect detections

**Solutions**:

1. **Increase Confidence Threshold**
   - Adjust slider to 0.7 or higher

2. **Filter by Class**
   - Modify object_detector.cpp to filter specific classes

3. **Use Better Model**
   - Download yolov8m or yolov8l for better accuracy

---

## UI Issues

### UI Not Responding

**Problem**: Can't interact with UI elements

**Solutions**:

1. **Window Focus**
   - Ensure window has focus
   - Try clicking on window border

2. **Input Handling**
   - Check if ImGui is properly initialized
   - Verify WndProc is handling messages

3. **Transparency Issues**
   - The overlay uses click-through transparency
   - May need to adjust window flags

### Theme Not Changing

**Problem**: Theme buttons don't work

**Solutions**:

1. **Force Theme Application**
   - Click both theme buttons
   - Restart application

2. **ImGui Style**
   - Check ApplyTheme() function
   - Verify ImGui context is valid

### Settings Not Saving

**Problem**: Changes lost after restart

**Solutions**:

1. **File Permissions**
   - Ensure write access to config/ directory
   - Run as Administrator if needed

2. **JSON Format**
   - Check config/settings.json for corruption
   - Delete file to regenerate defaults

3. **Save Explicitly**
   - Click "Save Profile" button
   - Don't just close window

### Color Picker Not Working

**Problem**: Can't change FOV color

**Solutions**:

1. **ImGui Color Edit**
   - Click on color square
   - Drag in color picker window

2. **Alpha Channel**
   - Enable alpha preview in flags
   - Already enabled in code

---

## Platform-Specific Issues

### Windows 11 Issues

**Problem**: Compatibility issues on Windows 11

**Solutions**:
- Run in Windows 10 compatibility mode
- Disable Windows 11 security features temporarily
- Update to latest Windows 11 version

### Multi-Monitor Issues

**Problem**: Overlay appears on wrong monitor

**Solutions**:
- Move window to desired monitor
- Adjust window creation position in code
- Use Windows Display Settings to set primary monitor

---

## Debug Mode

To enable detailed logging:

1. Build in Debug configuration:
   ```bash
   cmake --build . --config Debug
   ```

2. Check console output for detailed messages

3. Use Visual Studio debugger:
   - Set breakpoints
   - Watch variables
   - Step through code

---

## Getting Help

If you continue to experience issues:

1. Check console output for error messages
2. Review Windows Event Viewer (Application logs)
3. Open an issue on GitHub with:
   - System specifications
   - Error messages
   - Steps to reproduce
   - Screenshots if applicable

---

## Known Issues

### Current Limitations

1. **YOLOv8 Detection**: Currently uses stub implementation
   - Full ONNX Runtime integration needed
   - Detection features are demonstration only

2. **Multi-Monitor**: Limited testing on multi-monitor setups
   - May need manual window positioning

3. **Anti-Cheat**: May be detected by anti-cheat systems
   - Use at your own risk
   - Only for educational purposes

### Planned Fixes

- Full ONNX Runtime integration
- Better multi-monitor support
- Improved error handling
- More detailed logging

---

## Contact

For additional support, please open an issue on GitHub with detailed information about your problem.
