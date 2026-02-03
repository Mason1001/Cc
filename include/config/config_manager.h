#pragma once

#include <string>
#include <memory>
#include "overlay/fov_renderer.h"
#include "sound/sound_tracker.h"

namespace fov {

struct DetectionConfig {
    bool enabled;
    std::string model_path;
    float confidence_threshold;
    float iou_threshold;
    float friend_color[4];
    float enemy_color[4];
    std::vector<int> friend_classes;
    std::vector<int> enemy_classes;
};

struct OverlayConfig {
    std::string renderer;
    int width;
    int height;
    bool transparent;
    bool topmost;
    bool vsync;
};

struct UIConfig {
    bool show_menu;
    std::string menu_key;
    bool show_fps;
    int font_size;
};

class ConfigManager {
public:
    ConfigManager();
    ~ConfigManager();

    bool LoadFromFile(const std::string& filepath);
    bool SaveToFile(const std::string& filepath);
    
    FOVConfig& GetFOVConfig() { return fov_config_; }
    DetectionConfig& GetDetectionConfig() { return detection_config_; }
    SoundTrackingConfig& GetSoundTrackingConfig() { return sound_config_; }
    OverlayConfig& GetOverlayConfig() { return overlay_config_; }
    UIConfig& GetUIConfig() { return ui_config_; }
    
    const FOVConfig& GetFOVConfig() const { return fov_config_; }
    const DetectionConfig& GetDetectionConfig() const { return detection_config_; }
    const SoundTrackingConfig& GetSoundTrackingConfig() const { return sound_config_; }
    const OverlayConfig& GetOverlayConfig() const { return overlay_config_; }
    const UIConfig& GetUIConfig() const { return ui_config_; }

private:
    FOVConfig fov_config_;
    DetectionConfig detection_config_;
    SoundTrackingConfig sound_config_;
    OverlayConfig overlay_config_;
    UIConfig ui_config_;
};

} // namespace fov
