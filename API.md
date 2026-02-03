# API Documentation

## Core Classes

### OverlayManager

Manages the overlay window creation, event handling, and frame timing.

#### Methods

```cpp
bool Initialize(int width, int height, bool transparent = true, bool topmost = true)
```
Creates and initializes the overlay window.
- **Parameters:**
  - `width`: Window width in pixels
  - `height`: Window height in pixels
  - `transparent`: Enable window transparency
  - `topmost`: Keep window on top of other windows
- **Returns:** true on success, false on failure

```cpp
void ProcessMessages()
```
Processes Windows message queue. Call once per frame.

```cpp
void BeginFrame()
```
Starts a new frame and updates timing information.

```cpp
void EndFrame()
```
Completes the current frame.

```cpp
float GetFPS() const
```
Returns current frames per second.

---

### FOVRenderer

Handles DirectX 11 rendering for FOV overlay and visual elements.

#### Methods

```cpp
bool Initialize(ID3D11Device* device, ID3D11DeviceContext* context)
```
Initializes the renderer with DirectX device and context.

```cpp
void SetConfig(const FOVConfig& config)
```
Updates FOV rendering configuration.

```cpp
void RenderFOV(float center_x, float center_y)
```
Renders the Field of View circle at specified position.

```cpp
void RenderCircle(float x, float y, float radius, const float color[4], float thickness, bool filled = false)
```
Renders a circle at specified position.
- **Parameters:**
  - `x, y`: Center position
  - `radius`: Circle radius
  - `color`: RGBA color array (0.0-1.0)
  - `thickness`: Line thickness
  - `filled`: Fill the circle

```cpp
void RenderLine(float x1, float y1, float x2, float y2, const float color[4], float thickness)
```
Renders a line between two points.

```cpp
void RenderBox(float x, float y, float width, float height, const float color[4], float thickness)
```
Renders a rectangular box.

---

### YOLODetector

Wrapper for YOLOv8 object detection model.

#### Methods

```cpp
bool Initialize(const std::string& model_path, float conf_threshold = 0.5f, float iou_threshold = 0.45f)
```
Loads YOLOv8 model from specified path.
- **Parameters:**
  - `model_path`: Path to model file (.pt or .onnx)
  - `conf_threshold`: Confidence threshold for detections (0.0-1.0)
  - `iou_threshold`: IoU threshold for NMS (0.0-1.0)

```cpp
std::vector<DetectionBox> Detect(const unsigned char* image_data, int width, int height, int channels)
```
Performs object detection on image data.
- **Parameters:**
  - `image_data`: Raw image data (RGB or BGR)
  - `width, height`: Image dimensions
  - `channels`: Number of channels (3 for RGB)
- **Returns:** Vector of detected objects

```cpp
void SetConfidenceThreshold(float threshold)
```
Updates confidence threshold.

```cpp
void SetIOUThreshold(float threshold)
```
Updates IoU threshold for NMS.

---

### ObjectTracker

Tracks detected objects across frames.

#### Methods

```cpp
void SetFriendClasses(const std::vector<int>& classes)
```
Sets class IDs to be marked as friendly.

```cpp
void SetEnemyClasses(const std::vector<int>& classes)
```
Sets class IDs to be marked as enemy.

```cpp
std::vector<TrackedObject> Update(const std::vector<DetectionBox>& detections)
```
Updates tracking with new detections.
- **Returns:** Vector of tracked objects with velocity information

```cpp
void Clear()
```
Clears all tracked objects.

---

### SoundTracker

Tracks and visualizes sound sources.

#### Methods

```cpp
bool Initialize()
```
Initializes sound tracking system.

```cpp
void SetConfig(const SoundTrackingConfig& config)
```
Updates sound tracking configuration.

```cpp
void Update(float delta_time)
```
Updates sound tracking (called each frame).

```cpp
std::vector<SoundSource> GetActiveSources() const
```
Returns list of active sound sources.

```cpp
void AddSoundSource(float azimuth, float elevation, float intensity)
```
Manually adds a sound source (for testing or external integration).

---

### ConfigManager

Manages application configuration.

#### Methods

```cpp
bool LoadFromFile(const std::string& filepath)
```
Loads configuration from JSON file.

```cpp
bool SaveToFile(const std::string& filepath)
```
Saves current configuration to JSON file.

```cpp
FOVConfig& GetFOVConfig()
DetectionConfig& GetDetectionConfig()
SoundTrackingConfig& GetSoundTrackingConfig()
OverlayConfig& GetOverlayConfig()
UIConfig& GetUIConfig()
```
Getters for configuration sections.

---

### ImGuiManager

Manages Dear ImGui user interface.

#### Methods

```cpp
bool Initialize(HWND hwnd, ID3D11Device* device, ID3D11DeviceContext* context)
```
Initializes ImGui with window and DirectX context.

```cpp
void BeginFrame()
```
Starts ImGui frame (call before rendering UI).

```cpp
void EndFrame()
```
Ends ImGui frame and renders (call after UI code).

```cpp
void RenderUI()
```
Renders the settings UI.

```cpp
bool WantCaptureMouse() const
bool WantCaptureKeyboard() const
```
Returns whether ImGui wants to capture input.

---

## Data Structures

### FOVConfig

```cpp
struct FOVConfig {
    bool enabled;           // Enable FOV rendering
    float radius;          // FOV radius in pixels
    float color[4];        // RGBA color (0.0-1.0)
    float thickness;       // Line thickness
    bool filled;          // Fill the circle
    int target_fps;       // Target frame rate
};
```

### DetectionBox

```cpp
struct DetectionBox {
    float x, y;           // Top-left position
    float width, height;  // Box dimensions
    int class_id;         // Object class ID
    float confidence;     // Detection confidence (0.0-1.0)
    std::string label;    // Class label
    bool is_friend;       // Friend/enemy classification
};
```

### TrackedObject

```cpp
struct TrackedObject {
    DetectionBox box;                                  // Current detection
    int track_id;                                      // Unique tracking ID
    std::chrono::steady_clock::time_point last_seen;  // Last detection time
    float velocity_x, velocity_y;                      // Estimated velocity
};
```

### SoundSource

```cpp
struct SoundSource {
    float azimuth;      // Horizontal angle (-180 to 180)
    float elevation;    // Vertical angle (-90 to 90)
    float distance;     // Distance estimate
    float intensity;    // Sound intensity/volume
};
```

### SoundTrackingConfig

```cpp
struct SoundTrackingConfig {
    bool enabled;                 // Enable sound tracking
    float y_axis_sensitivity;     // Y-axis sensitivity multiplier
    float horizontal_range;       // Horizontal detection range (degrees)
    float vertical_range;         // Vertical detection range (degrees)
    float indicator_color[4];     // Indicator RGBA color
    float indicator_size;         // Indicator size in pixels
};
```

### DetectionConfig

```cpp
struct DetectionConfig {
    bool enabled;                     // Enable object detection
    std::string model_path;           // Path to YOLOv8 model
    float confidence_threshold;       // Minimum confidence
    float iou_threshold;              // NMS IoU threshold
    float friend_color[4];            // Friend box color
    float enemy_color[4];             // Enemy box color
    std::vector<int> friend_classes;  // Friend class IDs
    std::vector<int> enemy_classes;   // Enemy class IDs
};
```

### OverlayConfig

```cpp
struct OverlayConfig {
    std::string renderer;  // Renderer type ("DirectX11")
    int width;            // Window width
    int height;           // Window height
    bool transparent;     // Enable transparency
    bool topmost;         // Keep window on top
    bool vsync;          // Enable VSync
};
```

### UIConfig

```cpp
struct UIConfig {
    bool show_menu;        // Show settings menu
    std::string menu_key;  // Key to toggle menu ("INSERT")
    bool show_fps;         // Show FPS counter
    int font_size;         // UI font size
};
```

---

## Usage Examples

### Basic Initialization

```cpp
#include "overlay/overlay_manager.h"
#include "overlay/fov_renderer.h"
#include "config/config_manager.h"

int main() {
    // Load configuration
    ConfigManager config;
    config.LoadFromFile("settings.json");
    
    // Create overlay
    OverlayManager overlay;
    overlay.Initialize(1920, 1080, true, true);
    
    // Initialize DirectX and renderer
    // ... (see main.cpp for full example)
    
    // Main loop
    while (overlay.IsRunning()) {
        overlay.ProcessMessages();
        overlay.BeginFrame();
        
        // Render
        fov_renderer.RenderFOV(960, 540);
        
        overlay.EndFrame();
    }
    
    return 0;
}
```

### Object Detection

```cpp
// Initialize detector
YOLODetector detector;
detector.Initialize("models/yolov8n.pt", 0.5f, 0.45f);

// Initialize tracker
ObjectTracker tracker;
tracker.SetFriendClasses({0});  // Class 0 = person
tracker.SetEnemyClasses({1, 2}); // Classes 1, 2 = enemies

// Detection loop
unsigned char* image = GetScreenCapture();
auto detections = detector.Detect(image, 1920, 1080, 3);
auto tracked = tracker.Update(detections);

// Render boxes
for (const auto& obj : tracked) {
    auto color = obj.box.is_friend ? friend_color : enemy_color;
    fov_renderer.RenderBox(obj.box.x, obj.box.y, 
                          obj.box.width, obj.box.height, 
                          color, 2.0f);
}
```

### Sound Tracking

```cpp
// Initialize sound tracker
SoundTracker sound_tracker;
sound_tracker.Initialize();

SoundTrackingConfig config;
config.enabled = true;
config.y_axis_sensitivity = 1.5f;
sound_tracker.SetConfig(config);

// Update and render
sound_tracker.Update(delta_time);
auto sources = sound_tracker.GetActiveSources();

for (const auto& source : sources) {
    // Convert to screen coordinates
    float x = screen_center_x + cos(source.azimuth * PI / 180.0f) * 200.0f;
    float y = screen_center_y + sin(source.elevation * PI / 180.0f) * 200.0f;
    
    fov_renderer.RenderCircle(x, y, 10.0f, indicator_color, 2.0f, true);
}
```

---

## Error Handling

All initialization methods return `bool` indicating success/failure:

```cpp
if (!overlay.Initialize(1920, 1080)) {
    std::cerr << "Failed to initialize overlay" << std::endl;
    return 1;
}
```

Check console output for detailed error messages.

---

## Thread Safety

⚠️ **Warning:** Core classes are **not thread-safe**. All methods should be called from the main rendering thread.

For multi-threaded detection:
```cpp
// Detection thread
std::thread detection_thread([&]() {
    while (running) {
        auto detections = detector.Detect(...);
        {
            std::lock_guard<std::mutex> lock(detection_mutex);
            latest_detections = detections;
        }
    }
});

// Main thread - render latest detections
{
    std::lock_guard<std::mutex> lock(detection_mutex);
    RenderDetections(latest_detections);
}
```
