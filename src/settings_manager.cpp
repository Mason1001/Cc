/*
 * Settings Manager Implementation
 */

#include "settings_manager.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <filesystem>

// Simple JSON serialization (avoiding external dependencies)
// For production, use nlohmann/json or similar library

SettingsManager::SettingsManager(const std::string& configFile)
    : m_configFile(configFile)
{
}

SettingsManager::~SettingsManager()
{
}

bool SettingsManager::LoadSettings()
{
    std::ifstream file(m_configFile);
    if (!file.is_open())
    {
        std::cout << "Config file not found, using defaults." << std::endl;
        ResetToDefaults();
        return false;
    }

    try
    {
        // Simple JSON parsing (manual for this example)
        std::string line;
        while (std::getline(file, line))
        {
            // Remove whitespace
            line.erase(0, line.find_first_not_of(" \t\r\n"));
            line.erase(line.find_last_not_of(" \t\r\n") + 1);

            // Skip empty lines and comments
            if (line.empty() || line[0] == '{' || line[0] == '}' || line[0] == '/') continue;

            // Parse key-value pairs
            size_t colonPos = line.find(':');
            if (colonPos == std::string::npos) continue;

            std::string key = line.substr(0, colonPos);
            std::string value = line.substr(colonPos + 1);

            // Clean up key and value
            key.erase(0, key.find_first_not_of(" \t\""));
            key.erase(key.find_last_not_of(" \t\",\"") + 1);
            value.erase(0, value.find_first_not_of(" \t"));
            value.erase(value.find_last_not_of(" \t,") + 1);

            // Parse values
            if (key == "fovOverlayEnabled") m_settings.fovOverlayEnabled = (value == "true");
            else if (key == "fovSize") m_settings.fovSize = std::stof(value);
            else if (key == "fovTransparency") m_settings.fovTransparency = std::stof(value);
            else if (key == "detectionEnabled") m_settings.detectionEnabled = (value == "true");
            else if (key == "detectionConfidence") m_settings.detectionConfidence = std::stof(value);
            else if (key == "highlightIntensity") m_settings.highlightIntensity = std::stof(value);
            else if (key == "trackingEnabled") m_settings.trackingEnabled = (value == "true");
            else if (key == "trackingPrecision") m_settings.trackingPrecision = std::stof(value);
            else if (key == "trackingSmoothness") m_settings.trackingSmoothness = std::stof(value);
            else if (key == "trackingSpeed") m_settings.trackingSpeed = std::stof(value);
            else if (key == "darkTheme") m_settings.darkTheme = (value == "true");
        }

        file.close();
        std::cout << "Settings loaded from " << m_configFile << std::endl;
        return true;
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error loading settings: " << e.what() << std::endl;
        ResetToDefaults();
        return false;
    }
}

bool SettingsManager::SaveSettings()
{
    // Create directory if it doesn't exist
    std::filesystem::path configPath(m_configFile);
    if (configPath.has_parent_path())
    {
        std::filesystem::create_directories(configPath.parent_path());
    }

    std::ofstream file(m_configFile);
    if (!file.is_open())
    {
        std::cerr << "Failed to open config file for writing: " << m_configFile << std::endl;
        return false;
    }

    // Write JSON format
    file << "{\n";
    file << "  \"fovOverlayEnabled\": " << (m_settings.fovOverlayEnabled ? "true" : "false") << ",\n";
    file << "  \"fovSize\": " << m_settings.fovSize << ",\n";
    file << "  \"fovColor\": [" << m_settings.fovColor[0] << ", " 
         << m_settings.fovColor[1] << ", " << m_settings.fovColor[2] << ", " 
         << m_settings.fovColor[3] << "],\n";
    file << "  \"fovTransparency\": " << m_settings.fovTransparency << ",\n";
    file << "  \"detectionEnabled\": " << (m_settings.detectionEnabled ? "true" : "false") << ",\n";
    file << "  \"detectionConfidence\": " << m_settings.detectionConfidence << ",\n";
    file << "  \"highlightIntensity\": " << m_settings.highlightIntensity << ",\n";
    file << "  \"trackingEnabled\": " << (m_settings.trackingEnabled ? "true" : "false") << ",\n";
    file << "  \"trackingPrecision\": " << m_settings.trackingPrecision << ",\n";
    file << "  \"trackingSmoothness\": " << m_settings.trackingSmoothness << ",\n";
    file << "  \"trackingSpeed\": " << m_settings.trackingSpeed << ",\n";
    file << "  \"darkTheme\": " << (m_settings.darkTheme ? "true" : "false") << "\n";
    file << "}\n";

    file.close();
    std::cout << "Settings saved to " << m_configFile << std::endl;
    return true;
}

void SettingsManager::ResetToDefaults()
{
    m_settings = AppSettings();
    std::cout << "Settings reset to defaults." << std::endl;
}
