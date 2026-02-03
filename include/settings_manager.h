/*
 * Settings Manager for Configuration Management
 */

#pragma once
#include <string>

struct AppSettings
{
    // FOV Overlay Settings
    bool fovOverlayEnabled = true;
    float fovSize = 100.0f;
    float fovColor[4] = { 1.0f, 1.0f, 0.0f, 1.0f }; // Yellow
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

class SettingsManager
{
public:
    SettingsManager(const std::string& configFile);
    ~SettingsManager();

    bool LoadSettings();
    bool SaveSettings();
    void ResetToDefaults();

    AppSettings& GetSettings() { return m_settings; }
    const AppSettings& GetSettings() const { return m_settings; }

private:
    std::string m_configFile;
    AppSettings m_settings;
};
