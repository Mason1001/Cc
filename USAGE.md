# Usage Examples

Practical examples of using the FOV Overlay application.

## Basic Usage

### Starting the Application

```bash
cd build/bin/Release
FOVOverlay.exe
```

The overlay will appear as a transparent window on top of other applications.

### Toggle Settings Menu

Press `INSERT` to show/hide the settings menu while the overlay is running.

### Exit Application

Press `ESC` to cleanly exit the application.

## Configuration Examples

### Example 1: Simple FOV Circle

```json
{
  "fov": {
    "enabled": true,
    "radius": 100,
    "color": {"r": 255, "g": 0, "b": 0, "a": 200},
    "thickness": 2,
    "filled": false,
    "target_fps": 120
  },
  "detection": {"enabled": false},
  "sound_tracking": {"enabled": false}
}
```

This creates a red outline circle in the center of your screen.

### Example 2: Filled Crosshair

```json
{
  "fov": {
    "enabled": true,
    "radius": 5,
    "color": {"r": 0, "g": 255, "b": 0, "a": 255},
    "thickness": 1,
    "filled": true,
    "target_fps": 120
  }
}
```

Creates a small green dot as a crosshair.

### Example 3: Large FOV with Low Opacity

```json
{
  "fov": {
    "enabled": true,
    "radius": 300,
    "color": {"r": 255, "g": 255, "b": 255, "a": 50},
    "thickness": 3,
    "filled": false,
    "target_fps": 120
  }
}
```

Large, nearly transparent white circle.

### Example 4: Object Detection Enabled

```json
{
  "fov": {
    "enabled": true,
    "radius": 100,
    "color": {"r": 255, "g": 0, "b": 0, "a": 200}
  },
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
}
```

Enables both FOV and object detection. Friends appear in green, enemies in red.

### Example 5: Sound Tracking

```json
{
  "sound_tracking": {
    "enabled": true,
    "y_axis_sensitivity": 1.5,
    "horizontal_range": 180,
    "vertical_range": 90,
    "indicator_color": {"r": 255, "g": 255, "b": 0, "a": 200},
    "indicator_size": 15
  }
}
```

Enables sound source visualization with increased vertical sensitivity.

### Example 6: Competitive Gaming Setup

```json
{
  "fov": {
    "enabled": true,
    "radius": 50,
    "color": {"r": 0, "g": 255, "b": 255, "a": 180},
    "thickness": 1,
    "filled": false,
    "target_fps": 144
  },
  "overlay": {
    "renderer": "DirectX11",
    "width": 1920,
    "height": 1080,
    "transparent": true,
    "topmost": true,
    "vsync": false
  },
  "ui": {
    "show_menu": false,
    "show_fps": true
  }
}
```

Minimal overlay for competitive gaming:
- Small cyan FOV circle
- 144 FPS target
- VSync disabled
- Menu hidden by default

### Example 7: Streaming/Content Creation

```json
{
  "fov": {
    "enabled": true,
    "radius": 120,
    "color": {"r": 255, "g": 0, "b": 255, "a": 150},
    "thickness": 3,
    "filled": false,
    "target_fps": 60
  },
  "overlay": {
    "vsync": true
  },
  "ui": {
    "show_fps": true,
    "font_size": 20
  }
}
```

Settings optimized for streaming:
- Visible but not distracting (magenta, semi-transparent)
- 60 FPS (standard streaming framerate)
- VSync enabled
- Larger UI text

## YOLO Class IDs

Common COCO dataset class IDs for object detection:

| Class ID | Label | Description |
|----------|-------|-------------|
| 0 | person | Human detection |
| 1 | bicycle | Bicycle |
| 2 | car | Car |
| 3 | motorcycle | Motorcycle |
| 5 | bus | Bus |
| 7 | truck | Truck |
| 14 | bird | Bird |
| 15 | cat | Cat |
| 16 | dog | Dog |

### Example: Detect Only People

```json
{
  "detection": {
    "enabled": true,
    "friend_classes": [0],
    "enemy_classes": []
  }
}
```

### Example: Detect Vehicles as Enemies

```json
{
  "detection": {
    "enabled": true,
    "friend_classes": [0],
    "enemy_classes": [1, 2, 3, 5, 7]
  }
}
```

## Runtime Adjustments

### Via ImGui Menu (Press INSERT)

The settings menu allows real-time adjustment of:

1. **FOV Settings**
   - Radius slider (0-500)
   - Color picker
   - Thickness slider
   - Fill toggle
   - Enable/disable

2. **Detection Settings**
   - Confidence threshold slider
   - IoU threshold slider
   - Enable/disable
   - Class selection

3. **Sound Tracking**
   - Sensitivity sliders
   - Range adjustments
   - Enable/disable

4. **Performance**
   - FPS target
   - VSync toggle
   - Show FPS counter

### Via settings.json

Edit `settings.json` while the application is running (requires restart):

```bash
# 1. Edit settings.json
notepad settings.json

# 2. Save changes

# 3. Restart application
FOVOverlay.exe
```

## Advanced Usage

### Multiple Overlays

You can run multiple instances with different configurations:

```bash
# Instance 1: FOV only
FOVOverlay.exe

# Instance 2: Detection only (different config)
FOVOverlay.exe --config detection_config.json
```

(Note: Multi-config support requires implementation)

### Performance Monitoring

Monitor performance in real-time:

1. Enable FPS display in settings.json:
```json
{
  "ui": {
    "show_fps": true
  }
}
```

2. Watch console output for FPS reports

3. Use Task Manager to monitor CPU/GPU usage

### Integration with Other Tools

The overlay can run alongside:
- Game overlays (Steam, Discord, etc.)
- Recording software (OBS, etc.)
- Performance monitors

### Custom Models

Use your own trained YOLOv8 models:

1. Train model using Ultralytics:
```python
from ultralytics import YOLO

model = YOLO('yolov8n.pt')
model.train(data='custom.yaml', epochs=100)
```

2. Export to ONNX:
```python
model.export(format='onnx')
```

3. Update settings.json:
```json
{
  "detection": {
    "model_path": "models/custom_model.onnx"
  }
}
```

## Troubleshooting Common Scenarios

### Overlay Not Visible

1. Check if window is created: Look for window in Alt+Tab
2. Try disabling transparency:
```json
{"overlay": {"transparent": false}}
```
3. Adjust color opacity (increase alpha value)

### Low FPS

1. Reduce target FPS:
```json
{"fov": {"target_fps": 60}}
```

2. Disable unused features:
```json
{
  "detection": {"enabled": false},
  "sound_tracking": {"enabled": false}
}
```

3. Simplify rendering:
```json
{"fov": {"radius": 50, "thickness": 1}}
```

### Detection Not Accurate

1. Adjust confidence threshold:
```json
{"detection": {"confidence_threshold": 0.3}}
```

2. Use larger model:
```json
{"detection": {"model_path": "models/yolov8m.pt"}}
```

3. Check model format compatibility

## Tips and Best Practices

1. **Start Simple**: Begin with just FOV enabled, then add features
2. **Save Configs**: Create multiple settings files for different scenarios
3. **Monitor Performance**: Keep FPS above 60 for smooth experience
4. **Adjust Colors**: Use high-contrast colors for visibility
5. **Test Settings**: Try different configurations to find what works best

## Community Configurations

Share your configurations or try community favorites!

### Pro Gamer Setup
```json
{
  "fov": {"radius": 40, "color": {"r": 0, "g": 255, "b": 0, "a": 255}},
  "overlay": {"vsync": false},
  "ui": {"show_menu": false}
}
```

### Casual Use
```json
{
  "fov": {"radius": 100, "color": {"r": 255, "g": 255, "b": 255, "a": 100}},
  "overlay": {"vsync": true}
}
```

---

For more information, see:
- [README.md](README.md) - Full feature documentation
- [API.md](API.md) - Programming reference
- [DEVELOPMENT.md](DEVELOPMENT.md) - Development guide
