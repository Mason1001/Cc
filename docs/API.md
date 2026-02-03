# API Documentation

This document provides detailed information about the Gaming Overlay API and class interfaces.

## Table of Contents

1. [Core Classes](#core-classes)
2. [Data Structures](#data-structures)
3. [Settings API](#settings-api)
4. [Rendering API](#rendering-api)
5. [Detection API](#detection-api)
6. [Tracking API](#tracking-api)

---

## Core Classes

### OverlayWindow

Manages the Windows API window and event handling.

#### Constructor
```cpp
OverlayWindow(HINSTANCE hInstance, WNDPROC wndProc);
```

#### Methods

**Initialize**
```cpp
bool Initialize(int width, int height, const std::string& title);
```
Creates and initializes the overlay window.

- **Parameters**:
  - `width`: Window width in pixels
  - `height`: Window height in pixels
  - `title`: Window title
- **Returns**: `true` if successful, `false` otherwise

**GetHWND**
```cpp
HWND GetHWND() const;
```
Returns the window handle.

**GetWidth / GetHeight**
```cpp
int GetWidth() const;
int GetHeight() const;
```
Returns window dimensions.

---

### Renderer

Handles DirectX 11 rendering operations.

#### Constructor
```cpp
Renderer();
```

#### Methods

**Initialize**
```cpp
bool Initialize(HWND hwnd);
```
Initializes DirectX 11 with the given window.

**BeginFrame / EndFrame**
```cpp
void BeginFrame();
void EndFrame();
```
Frame rendering lifecycle methods.

**RenderFOVOverlay**
```cpp
void RenderFOVOverlay(float size, const float color[4], float transparency);
```
Renders the field-of-view circle overlay.

- **Parameters**:
  - `size`: Radius in pixels
  - `color`: RGBA color array [0.0-1.0]
  - `transparency`: Alpha multiplier [0.0-1.0]

**RenderDetections**
```cpp
void RenderDetections(const std::vector<DetectionResult>& detections);
```
Renders bounding boxes for detected objects.

**RenderBox**
```cpp
void RenderBox(const XMFLOAT2& topLeft, const XMFLOAT2& bottomRight, 
               const float color[4], float thickness = 2.0f);
```
Renders a rectangular bounding box.

**RenderCircle**
```cpp
void RenderCircle(const XMFLOAT2& center, float radius, 
                  const float color[4], int segments = 64);
```
Renders a circle with specified parameters.

---

### UIManager

Manages the Dear ImGui user interface.

#### Constructor
```cpp
UIManager();
```

#### Methods

**Initialize**
```cpp
bool Initialize(HWND hwnd, ID3D11Device* device, ID3D11DeviceContext* context);
```
Initializes ImGui with DirectX 11 backend.

**Render**
```cpp
void Render(SettingsManager* settings, ObjectDetector* detector, TrackingSystem* tracker);
```
Renders the UI and handles user input.

**ApplyTheme**
```cpp
void ApplyTheme(bool isDark);
```
Applies dark or light theme to the UI.

---

### SettingsManager

Handles configuration persistence and management.

#### Constructor
```cpp
SettingsManager(const std::string& configFile);
```

#### Methods

**LoadSettings**
```cpp
bool LoadSettings();
```
Loads settings from JSON file.

**SaveSettings**
```cpp
bool SaveSettings();
```
Saves current settings to JSON file.

**ResetToDefaults**
```cpp
void ResetToDefaults();
```
Resets all settings to default values.

**GetSettings**
```cpp
AppSettings& GetSettings();
const AppSettings& GetSettings() const;
```
Returns reference to settings structure.

---

### ObjectDetector

Handles YOLOv8-based object detection.

#### Constructor
```cpp
ObjectDetector();
```

#### Methods

**Initialize**
```cpp
bool Initialize(const std::string& modelPath);
```
Loads and initializes the YOLOv8 model.

**Update**
```cpp
void Update();
```
Runs detection inference for the current frame.

**GetDetections**
```cpp
std::vector<DetectionResult> GetDetections() const;
```
Returns all current detections.

**IsInitialized**
```cpp
bool IsInitialized() const;
```
Returns whether detector is ready to use.

---

### TrackingSystem

Implements smooth aim tracking algorithm.

#### Constructor
```cpp
TrackingSystem();
```

#### Methods

**Update**
```cpp
void Update(float deltaTime);
```
Updates tracking position based on time delta.

**SetTarget**
```cpp
void SetTarget(const XMFLOAT2& target);
```
Sets the tracking target position.

**ClearTarget**
```cpp
void ClearTarget();
```
Clears current tracking target.

**HasTarget**
```cpp
bool HasTarget() const;
```
Returns whether a target is currently tracked.

**GetCurrentPosition / GetTargetPosition**
```cpp
XMFLOAT2 GetCurrentPosition() const;
XMFLOAT2 GetTargetPosition() const;
```
Returns current/target positions.

**SetPrecision / SetSmoothness / SetSpeed**
```cpp
void SetPrecision(float precision);
void SetSmoothness(float smoothness);
void SetSpeed(float speed);
```
Configures tracking parameters [0.0-1.0 or higher for speed].

---

## Data Structures

### AppSettings

Configuration structure for all application settings.

```cpp
struct AppSettings
{
    // FOV Overlay Settings
    bool fovOverlayEnabled = true;
    float fovSize = 100.0f;
    float fovColor[4] = { 1.0f, 1.0f, 0.0f, 1.0f };
    float fovTransparency = 0.8f;

    // Detection Settings
    bool detectionEnabled = false;
    float detectionConfidence = 0.5f;
    float highlightIntensity = 1.0f;

    // Tracking Settings
    bool trackingEnabled = false;
    float trackingPrecision = 0.7f;
    float trackingSmoothness = 0.5f;
    float trackingSpeed = 3.0f;

    // UI Settings
    bool darkTheme = true;
};
```

### DetectionResult

Represents a detected object.

```cpp
struct DetectionResult
{
    XMFLOAT2 topLeft;         // Top-left corner
    XMFLOAT2 bottomRight;     // Bottom-right corner
    float confidence;         // Detection confidence [0.0-1.0]
    int classId;              // Object class ID
    bool isEnemy;             // Enemy/friend classification
};
```

---

## Settings API

### JSON Format

Settings are stored in JSON format:

```json
{
  "fovOverlayEnabled": true,
  "fovSize": 100.0,
  "fovColor": [1.0, 1.0, 0.0, 1.0],
  "fovTransparency": 0.8,
  "detectionEnabled": false,
  "detectionConfidence": 0.5,
  "highlightIntensity": 1.0,
  "trackingEnabled": false,
  "trackingPrecision": 0.7,
  "trackingSmoothness": 0.5,
  "trackingSpeed": 3.0,
  "darkTheme": true
}
```

### Loading Settings

```cpp
SettingsManager settings("config/settings.json");
if (settings.LoadSettings()) {
    // Settings loaded successfully
    auto& config = settings.GetSettings();
    // Use config...
}
```

### Saving Settings

```cpp
settings.GetSettings().fovSize = 150.0f;
settings.SaveSettings();
```

### Profile Management

To support multiple profiles:

```cpp
// Load specific profile
SettingsManager profile1("config/profile1.json");
profile1.LoadSettings();

// Switch profiles
SettingsManager profile2("config/profile2.json");
profile2.LoadSettings();
```

---

## Rendering API

### Basic Rendering Loop

```cpp
// Initialize
Renderer renderer;
renderer.Initialize(hwnd);

// Render loop
while (running) {
    renderer.BeginFrame();
    
    // Draw FOV overlay
    float color[4] = { 1.0f, 1.0f, 0.0f, 1.0f };
    renderer.RenderFOVOverlay(100.0f, color, 0.8f);
    
    // Draw detections
    renderer.RenderDetections(detections);
    
    renderer.EndFrame();
}
```

### Custom Rendering

```cpp
// Draw custom box
XMFLOAT2 topLeft(-0.5f, -0.5f);
XMFLOAT2 bottomRight(0.5f, 0.5f);
float red[4] = { 1.0f, 0.0f, 0.0f, 1.0f };
renderer.RenderBox(topLeft, bottomRight, red, 3.0f);

// Draw custom circle
XMFLOAT2 center(0.0f, 0.0f);
float green[4] = { 0.0f, 1.0f, 0.0f, 1.0f };
renderer.RenderCircle(center, 0.2f, green, 32);
```

### Color Formats

All colors use RGBA format with float values [0.0-1.0]:

```cpp
float red[4]    = { 1.0f, 0.0f, 0.0f, 1.0f };
float green[4]  = { 0.0f, 1.0f, 0.0f, 1.0f };
float blue[4]   = { 0.0f, 0.0f, 1.0f, 1.0f };
float yellow[4] = { 1.0f, 1.0f, 0.0f, 1.0f };
float white[4]  = { 1.0f, 1.0f, 1.0f, 1.0f };
float black[4]  = { 0.0f, 0.0f, 0.0f, 1.0f };
```

### Coordinate System

The rendering system uses Normalized Device Coordinates (NDC):
- **X**: -1.0 (left) to +1.0 (right)
- **Y**: -1.0 (top) to +1.0 (bottom)
- **Origin**: Center of screen (0.0, 0.0)

---

## Detection API

### Initializing Detection

```cpp
ObjectDetector detector;
if (detector.Initialize("assets/models/yolov8n.onnx")) {
    // Detection ready
}
```

### Running Detection

```cpp
// Update detection each frame
detector.Update();

// Get results
auto detections = detector.GetDetections();
for (const auto& det : detections) {
    if (det.isEnemy) {
        // Handle enemy detection
    }
}
```

### Filtering Detections

```cpp
// Filter by confidence
auto highConfidence = detections | 
    std::views::filter([](const auto& d) { 
        return d.confidence > 0.7f; 
    });

// Filter by class
auto enemies = detections | 
    std::views::filter([](const auto& d) { 
        return d.isEnemy; 
    });
```

---

## Tracking API

### Basic Tracking

```cpp
TrackingSystem tracker;

// Set tracking parameters
tracker.SetPrecision(0.7f);
tracker.SetSmoothness(0.5f);
tracker.SetSpeed(3.0f);

// Track a target
XMFLOAT2 target(0.2f, 0.1f);
tracker.SetTarget(target);

// Update each frame
float deltaTime = 0.016f; // 60 FPS
tracker.Update(deltaTime);

// Get current position
auto pos = tracker.GetCurrentPosition();
```

### Advanced Tracking

```cpp
// Dynamic target tracking
if (!detections.empty()) {
    // Track nearest enemy
    auto nearest = std::min_element(detections.begin(), detections.end(),
        [](const auto& a, const auto& b) {
            // Compare distances to center
            float distA = sqrt(a.topLeft.x * a.topLeft.x + a.topLeft.y * a.topLeft.y);
            float distB = sqrt(b.topLeft.x * b.topLeft.x + b.topLeft.y * b.topLeft.y);
            return distA < distB;
        });
    
    if (nearest->isEnemy) {
        // Calculate center of bounding box
        XMFLOAT2 center(
            (nearest->topLeft.x + nearest->bottomRight.x) / 2.0f,
            (nearest->topLeft.y + nearest->bottomRight.y) / 2.0f
        );
        tracker.SetTarget(center);
    }
} else {
    tracker.ClearTarget();
}
```

---

## Error Handling

All initialization methods return `bool`:

```cpp
if (!window->Initialize(1920, 1080, "Overlay")) {
    std::cerr << "Window initialization failed!" << std::endl;
    return -1;
}

if (!renderer->Initialize(window->GetHWND())) {
    std::cerr << "Renderer initialization failed!" << std::endl;
    return -1;
}
```

Check console output for detailed error messages.

---

## Performance Tips

### Frame Rate Management

```cpp
auto lastTime = std::chrono::high_resolution_clock::now();
float targetFrameTime = 1.0f / 120.0f; // 120 FPS

while (running) {
    auto currentTime = std::chrono::high_resolution_clock::now();
    float deltaTime = std::chrono::duration<float>(currentTime - lastTime).count();
    
    if (deltaTime >= targetFrameTime) {
        lastTime = currentTime;
        // Update and render
    }
}
```

### Conditional Rendering

```cpp
// Only render when enabled
if (settings.fovOverlayEnabled) {
    renderer.RenderFOVOverlay(/*...*/);
}

if (settings.detectionEnabled && detector.IsInitialized()) {
    renderer.RenderDetections(/*...*/);
}
```

### Batch Operations

```cpp
// Batch all rendering operations
renderer.BeginFrame();
// ... all drawing here ...
renderer.EndFrame();
```

---

## Extension Points

### Custom Detectors

Inherit from ObjectDetector:

```cpp
class CustomDetector : public ObjectDetector {
public:
    bool Initialize(const std::string& modelPath) override {
        // Custom initialization
    }
    
    void Update() override {
        // Custom detection logic
    }
};
```

### Custom Renderers

Add new rendering methods:

```cpp
class ExtendedRenderer : public Renderer {
public:
    void RenderCustomShape() {
        // Custom rendering
    }
};
```

---

## Integration Examples

### Full Integration

```cpp
int main() {
    // Initialize all components
    auto settings = std::make_unique<SettingsManager>("config/settings.json");
    settings->LoadSettings();
    
    auto window = std::make_unique<OverlayWindow>(hInstance, WndProc);
    window->Initialize(1920, 1080, "Overlay");
    
    auto renderer = std::make_unique<Renderer>();
    renderer->Initialize(window->GetHWND());
    
    auto ui = std::make_unique<UIManager>();
    ui->Initialize(window->GetHWND(), renderer->GetDevice(), renderer->GetDeviceContext());
    
    auto detector = std::make_unique<ObjectDetector>();
    detector->Initialize("assets/models/yolov8n.onnx");
    
    auto tracker = std::make_unique<TrackingSystem>();
    
    // Main loop
    while (running) {
        // Update
        if (settings->GetSettings().detectionEnabled) {
            detector->Update();
        }
        
        if (settings->GetSettings().trackingEnabled) {
            tracker->Update(deltaTime);
        }
        
        // Render
        renderer->BeginFrame();
        
        if (settings->GetSettings().fovOverlayEnabled) {
            renderer->RenderFOVOverlay(
                settings->GetSettings().fovSize,
                settings->GetSettings().fovColor,
                settings->GetSettings().fovTransparency
            );
        }
        
        if (settings->GetSettings().detectionEnabled) {
            renderer->RenderDetections(detector->GetDetections());
        }
        
        ui->Render(settings.get(), detector.get(), tracker.get());
        
        renderer->EndFrame();
    }
    
    // Cleanup handled by smart pointers
    settings->SaveSettings();
    return 0;
}
```

---

## Additional Resources

- [Dear ImGui Documentation](https://github.com/ocornut/imgui)
- [DirectX 11 Documentation](https://docs.microsoft.com/en-us/windows/win32/direct3d11)
- [YOLOv8 Documentation](https://docs.ultralytics.com/)
- [ONNX Runtime Documentation](https://onnxruntime.ai/docs/)
