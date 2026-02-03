/*
 * UI Manager for Dear ImGui Integration
 */

#pragma once
#include <d3d11.h>
#include <Windows.h>

class SettingsManager;
class ObjectDetector;
class TrackingSystem;

class UIManager
{
public:
    UIManager();
    ~UIManager();

    bool Initialize(HWND hwnd, ID3D11Device* device, ID3D11DeviceContext* context);
    void Cleanup();

    void Render(SettingsManager* settings, ObjectDetector* detector, TrackingSystem* tracker);

private:
    void RenderMainWindow(SettingsManager* settings, ObjectDetector* detector, TrackingSystem* tracker);
    void ApplyTheme(bool isDark);

    bool m_initialized;
    bool m_showMainWindow;
    bool m_isDarkTheme;
};
