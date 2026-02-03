#include "config/config_manager.h"
#include <fstream>
#include <iostream>
#include <sstream>

// Note: In a real implementation, would use nlohmann/json or similar
// For simplicity, implementing basic JSON parsing

namespace fov {

ConfigManager::ConfigManager() {
    // Initialize with default values
    fov_config_.enabled = true;
    fov_config_.radius = 100.0f;
    fov_config_.color[0] = 1.0f;
    fov_config_.color[1] = 0.0f;
    fov_config_.color[2] = 0.0f;
    fov_config_.color[3] = 0.8f;
    fov_config_.thickness = 2.0f;
    fov_config_.filled = false;
    fov_config_.target_fps = 120;
    
    detection_config_.enabled = true;
    detection_config_.model_path = "models/yolov8n.pt";
    detection_config_.confidence_threshold = 0.5f;
    detection_config_.iou_threshold = 0.45f;
    detection_config_.friend_color[0] = 0.0f;
    detection_config_.friend_color[1] = 1.0f;
    detection_config_.friend_color[2] = 0.0f;
    detection_config_.friend_color[3] = 1.0f;
    detection_config_.enemy_color[0] = 1.0f;
    detection_config_.enemy_color[1] = 0.0f;
    detection_config_.enemy_color[2] = 0.0f;
    detection_config_.enemy_color[3] = 1.0f;
    detection_config_.friend_classes = {0};
    detection_config_.enemy_classes = {1, 2, 3};
    
    sound_config_.enabled = true;
    sound_config_.y_axis_sensitivity = 1.0f;
    sound_config_.horizontal_range = 180.0f;
    sound_config_.vertical_range = 90.0f;
    sound_config_.indicator_color[0] = 1.0f;
    sound_config_.indicator_color[1] = 1.0f;
    sound_config_.indicator_color[2] = 0.0f;
    sound_config_.indicator_color[3] = 0.8f;
    sound_config_.indicator_size = 10.0f;
    
    overlay_config_.renderer = "DirectX11";
    overlay_config_.width = 1920;
    overlay_config_.height = 1080;
    overlay_config_.transparent = true;
    overlay_config_.topmost = true;
    overlay_config_.vsync = false;
    
    ui_config_.show_menu = true;
    ui_config_.menu_key = "INSERT";
    ui_config_.show_fps = true;
    ui_config_.font_size = 16;
}

ConfigManager::~ConfigManager() {
}

bool ConfigManager::LoadFromFile(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        std::cerr << "Failed to open config file: " << filepath << std::endl;
        return false;
    }
    
    // Simple JSON parsing (in real implementation, would use nlohmann/json)
    std::string line;
    std::string content;
    while (std::getline(file, line)) {
        content += line;
    }
    file.close();
    
    std::cout << "Config loaded from: " << filepath << std::endl;
    
    // For demonstration, using default values
    // In real implementation, would parse JSON and update config structs
    
    return true;
}

bool ConfigManager::SaveToFile(const std::string& filepath) {
    std::ofstream file(filepath);
    if (!file.is_open()) {
        std::cerr << "Failed to open config file for writing: " << filepath << std::endl;
        return false;
    }
    
    // In real implementation, would serialize config structs to JSON
    file << "{\n";
    file << "  \"fov\": {\n";
    file << "    \"enabled\": " << (fov_config_.enabled ? "true" : "false") << ",\n";
    file << "    \"radius\": " << fov_config_.radius << ",\n";
    file << "    \"target_fps\": " << fov_config_.target_fps << "\n";
    file << "  }\n";
    file << "}\n";
    
    file.close();
    
    std::cout << "Config saved to: " << filepath << std::endl;
    return true;
}

} // namespace fov
