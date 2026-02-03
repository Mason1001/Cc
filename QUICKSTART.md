# Gaming Overlay - Quick Start Guide

Get up and running in 5 minutes!

## Prerequisites ✅

- Windows 10/11 (64-bit)
- Visual Studio 2022 (with C++ workload)
- CMake 3.15+
- Git

## Step 1: Get the Code 📥

```bash
git clone https://github.com/Mason1001/Cc.git
cd Cc
```

## Step 2: Setup Dependencies 🔧

**Easy Way (Automated):**
```powershell
.\setup-dependencies.ps1
```

**Manual Way:**
```bash
# Download Dear ImGui
cd libs
git clone https://github.com/ocornut/imgui.git
cd ..
```

## Step 3: Build 🔨

```powershell
.\build.ps1
```

Wait for compilation to complete (~2-5 minutes first time).

## Step 4: Run 🚀

```bash
cd build\bin\Release
.\GamingOverlay.exe
```

## What You'll See 👀

1. **Console Window**: Debug output and status messages
2. **Control Panel**: Main UI with all settings
3. **FOV Overlay**: Yellow circle in the center of screen (default)

## First-Time Setup ⚙️

### Configure FOV Overlay
1. ✅ "Enable FOV Overlay" should already be checked
2. Drag the "FOV Size" slider (10-500 pixels)
3. Click the colored box to change color
4. Adjust "FOV Transparency" for visibility

### Try the UI Themes
- Click "Light Theme" button
- Click "Dark Theme" to switch back

### Save Your Settings
- Click "Save Profile" button
- Settings saved to `config/settings.json`

## Demo Features 🎮

### FOV Overlay (Ready to Use)
- ✅ Fully functional
- ✅ Real-time rendering
- ✅ Customizable colors and size
- ✅ Adjustable transparency

### Object Detection (Demo Mode)
- ⚠️ Shows animated test boxes
- ⚠️ Not real detection (needs ONNX Runtime + model)
- ✅ Try enabling to see the visualization

### Aim Tracking (Demo Mode)
- ⚠️ Simulated tracking
- ✅ Adjust precision, smoothness, speed sliders
- ✅ See the tracking status indicator

## Next Steps 📚

### Enable Real Detection
1. Download ONNX Runtime
2. Download YOLOv8 model
3. See `docs/SETUP.md` for details

### Learn More
- 📖 **Full Documentation**: `README.md`
- 🔧 **API Reference**: `docs/API.md`
- ❓ **Troubleshooting**: `docs/TROUBLESHOOTING.md`

### Customize
- Edit `config/settings.json` manually
- Modify source code in `src/`
- Add your own features!

## Common Issues 🐛

### Build Fails
```bash
# Clean and rebuild
rm -rf build
.\build.ps1
```

### ImGui Not Found
```bash
cd libs
git clone https://github.com/ocornut/imgui.git
cd ..
```

### Black Screen
- Check if FOV overlay is enabled
- Try adjusting transparency
- Verify DirectX 11 is working

## Keyboard Shortcuts ⌨️

- **F1**: Toggle UI (planned)
- **ESC**: Close application (standard Windows)

## Performance Tips 🎯

- Disable unused features
- Reduce FOV circle size
- Lower detection frequency
- Close other applications

## Configuration Examples 💡

### Minimal Overlay
```json
{
  "fovOverlayEnabled": true,
  "fovSize": 50.0,
  "fovTransparency": 0.5,
  "detectionEnabled": false,
  "trackingEnabled": false
}
```

### Full Features
```json
{
  "fovOverlayEnabled": true,
  "fovSize": 150.0,
  "detectionEnabled": true,
  "detectionConfidence": 0.7,
  "trackingEnabled": true,
  "trackingPrecision": 0.8
}
```

## Default Keybinds (Planned)

| Key | Action |
|-----|--------|
| F1 | Toggle UI |
| F2 | Toggle FOV |
| F3 | Toggle Detection |
| F4 | Toggle Tracking |

## Support 💬

- **Issues**: GitHub Issues
- **Docs**: Check `docs/` folder
- **Community**: Discussions

## Success Checklist ✅

- [ ] Project cloned
- [ ] Dear ImGui downloaded
- [ ] Project built successfully
- [ ] Application runs
- [ ] UI visible and responsive
- [ ] FOV overlay visible
- [ ] Settings save/load working
- [ ] No crashes or errors

## You're Ready! 🎉

Enjoy your gaming overlay! Customize it to your needs and have fun!

---

**Pro Tip**: Start with default settings, then gradually customize to find what works best for you.

**Warning**: Always respect game terms of service and anti-cheat policies.
