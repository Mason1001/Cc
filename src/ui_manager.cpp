/*
 * UI Manager Implementation
 */

#include "ui_manager.h"
#include "settings_manager.h"
#include "object_detector.h"
#include "tracking_system.h"
#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>
#include <iostream>

UIManager::UIManager()
    : m_initialized(false)
    , m_showMainWindow(true)
    , m_isDarkTheme(true)
{
}

UIManager::~UIManager()
{
    Cleanup();
}

bool UIManager::Initialize(HWND hwnd, ID3D11Device* device, ID3D11DeviceContext* context)
{
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    // Setup Platform/Renderer backends
    if (!ImGui_ImplWin32_Init(hwnd))
    {
        std::cerr << "Failed to initialize ImGui Win32!" << std::endl;
        return false;
    }

    if (!ImGui_ImplDX11_Init(device, context))
    {
        std::cerr << "Failed to initialize ImGui DX11!" << std::endl;
        return false;
    }

    // Apply default dark theme
    ApplyTheme(true);

    m_initialized = true;
    std::cout << "UI Manager initialized successfully!" << std::endl;

    return true;
}

void UIManager::Cleanup()
{
    if (m_initialized)
    {
        ImGui_ImplDX11_Shutdown();
        ImGui_ImplWin32_Shutdown();
        ImGui::DestroyContext();
        m_initialized = false;
    }
}

void UIManager::Render(SettingsManager* settings, ObjectDetector* detector, TrackingSystem* tracker)
{
    if (!m_initialized) return;

    // Start the Dear ImGui frame
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    // Render main window
    if (m_showMainWindow)
    {
        RenderMainWindow(settings, detector, tracker);
    }

    // Rendering
    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void UIManager::RenderMainWindow(SettingsManager* settings, ObjectDetector* detector, TrackingSystem* tracker)
{
    ImGui::SetNextWindowSize(ImVec2(450, 600), ImGuiCond_FirstUseEver);
    ImGui::Begin("Gaming Overlay Control Panel", &m_showMainWindow);

    auto& config = settings->GetSettings();

    // Theme Selection
    ImGui::Text("Theme Settings");
    ImGui::Separator();
    if (ImGui::Checkbox("Dark Theme", &m_isDarkTheme))
    {
        ApplyTheme(m_isDarkTheme);
    }
    ImGui::SameLine();
    if (ImGui::Button("Light Theme"))
    {
        m_isDarkTheme = false;
        ApplyTheme(false);
    }
    ImGui::Spacing();

    // FOV Overlay Settings
    ImGui::Text("FOV Overlay Settings");
    ImGui::Separator();
    ImGui::Checkbox("Enable FOV Overlay", &config.fovOverlayEnabled);
    
    if (config.fovOverlayEnabled)
    {
        ImGui::SliderFloat("FOV Size", &config.fovSize, 10.0f, 500.0f, "%.1f px");
        ImGui::SliderFloat("FOV Transparency", &config.fovTransparency, 0.0f, 1.0f, "%.2f");
        ImGui::ColorEdit4("FOV Color", config.fovColor, ImGuiColorEditFlags_AlphaPreview);
    }
    ImGui::Spacing();

    // Detection Settings
    ImGui::Text("Object Detection Settings");
    ImGui::Separator();
    ImGui::Checkbox("Enable Object Detection", &config.detectionEnabled);
    
    if (config.detectionEnabled)
    {
        ImGui::SliderFloat("Detection Confidence", &config.detectionConfidence, 0.0f, 1.0f, "%.2f");
        ImGui::SliderFloat("Highlight Intensity", &config.highlightIntensity, 0.0f, 2.0f, "%.2f");
        
        if (detector && detector->IsInitialized())
        {
            ImGui::Text("Detector Status: Active");
            auto detections = detector->GetDetections();
            ImGui::Text("Detected Objects: %d", (int)detections.size());
        }
        else
        {
            ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Detector Status: Not Available");
        }
    }
    ImGui::Spacing();

    // Tracking System Settings
    ImGui::Text("Aim Tracking System");
    ImGui::Separator();
    ImGui::Checkbox("Enable Tracking", &config.trackingEnabled);
    
    if (config.trackingEnabled)
    {
        ImGui::SliderFloat("Tracking Precision", &config.trackingPrecision, 0.0f, 1.0f, "%.2f");
        ImGui::SliderFloat("Tracking Smoothness", &config.trackingSmoothness, 0.0f, 1.0f, "%.2f");
        ImGui::SliderFloat("Tracking Speed", &config.trackingSpeed, 0.1f, 10.0f, "%.1f");
        
        if (tracker)
        {
            if (tracker->HasTarget())
            {
                ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "Tracking Status: Locked");
            }
            else
            {
                ImGui::Text("Tracking Status: Searching");
            }
        }
    }
    ImGui::Spacing();

    // Profile Management
    ImGui::Text("Profile Management");
    ImGui::Separator();
    
    static char profileName[128] = "default";
    ImGui::InputText("Profile Name", profileName, IM_ARRAYSIZE(profileName));
    
    ImGui::BeginGroup();
    if (ImGui::Button("Save Profile", ImVec2(140, 0)))
    {
        settings->SaveSettings();
        ImGui::OpenPopup("SavedPopup");
    }
    ImGui::SameLine();
    if (ImGui::Button("Load Profile", ImVec2(140, 0)))
    {
        settings->LoadSettings();
        ImGui::OpenPopup("LoadedPopup");
    }
    ImGui::SameLine();
    if (ImGui::Button("Reset to Default", ImVec2(140, 0)))
    {
        settings->ResetToDefaults();
    }
    ImGui::EndGroup();

    // Popup messages
    if (ImGui::BeginPopup("SavedPopup"))
    {
        ImGui::Text("Settings saved successfully!");
        ImGui::EndPopup();
    }
    if (ImGui::BeginPopup("LoadedPopup"))
    {
        ImGui::Text("Settings loaded successfully!");
        ImGui::EndPopup();
    }

    ImGui::Spacing();
    ImGui::Separator();
    
    // System Information
    ImGui::Text("System Information");
    ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
    ImGui::Text("Frame Time: %.3f ms", 1000.0f / ImGui::GetIO().Framerate);

    ImGui::End();
}

void UIManager::ApplyTheme(bool isDark)
{
    ImGuiStyle& style = ImGui::GetStyle();
    
    if (isDark)
    {
        // Dark theme
        ImGui::StyleColorsDark();
        
        style.WindowRounding = 5.0f;
        style.FrameRounding = 3.0f;
        style.ScrollbarRounding = 3.0f;
        style.GrabRounding = 3.0f;
        
        ImVec4* colors = style.Colors;
        colors[ImGuiCol_WindowBg] = ImVec4(0.10f, 0.10f, 0.10f, 0.95f);
        colors[ImGuiCol_Header] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
        colors[ImGuiCol_HeaderHovered] = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);
        colors[ImGuiCol_HeaderActive] = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);
        colors[ImGuiCol_Button] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
        colors[ImGuiCol_ButtonHovered] = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);
        colors[ImGuiCol_ButtonActive] = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);
    }
    else
    {
        // Light theme
        ImGui::StyleColorsLight();
        
        style.WindowRounding = 5.0f;
        style.FrameRounding = 3.0f;
        style.ScrollbarRounding = 3.0f;
        style.GrabRounding = 3.0f;
        
        ImVec4* colors = style.Colors;
        colors[ImGuiCol_WindowBg] = ImVec4(0.94f, 0.94f, 0.94f, 0.95f);
        colors[ImGuiCol_Header] = ImVec4(0.80f, 0.80f, 0.80f, 1.00f);
        colors[ImGuiCol_HeaderHovered] = ImVec4(0.70f, 0.70f, 0.70f, 1.00f);
        colors[ImGuiCol_HeaderActive] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
    }
    
    m_isDarkTheme = isDark;
}
