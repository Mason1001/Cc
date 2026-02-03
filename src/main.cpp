/*
 * Gaming Overlay with Object Detection and Tracking
 * Main Application Entry Point
 */

#include <Windows.h>
#include <d3d11.h>
#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>
#include <iostream>
#include <memory>
#include "overlay_window.h"
#include "renderer.h"
#include "ui_manager.h"
#include "settings_manager.h"
#include "object_detector.h"
#include "tracking_system.h"

#pragma comment(lib, "d3d11.lib")

// Forward declarations
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Global objects
std::unique_ptr<OverlayWindow> g_window;
std::unique_ptr<Renderer> g_renderer;
std::unique_ptr<UIManager> g_uiManager;
std::unique_ptr<SettingsManager> g_settingsManager;
std::unique_ptr<ObjectDetector> g_objectDetector;
std::unique_ptr<TrackingSystem> g_trackingSystem;

// Window procedure
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_SIZE:
        if (g_renderer && wParam != SIZE_MINIMIZED)
        {
            g_renderer->ResizeBuffers((UINT)LOWORD(lParam), (UINT)HIWORD(lParam));
        }
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU)
            return 0;
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hWnd, msg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    // Initialize console for debugging
    AllocConsole();
    FILE* pFile;
    freopen_s(&pFile, "CONOUT$", "w", stdout);
    freopen_s(&pFile, "CONOUT$", "w", stderr);

    std::cout << "Gaming Overlay - Starting..." << std::endl;

    try
    {
        // Initialize settings manager
        g_settingsManager = std::make_unique<SettingsManager>("config/settings.json");
        g_settingsManager->LoadSettings();

        // Create window
        g_window = std::make_unique<OverlayWindow>(hInstance, WndProc);
        if (!g_window->Initialize(1920, 1080, "Gaming Overlay"))
        {
            std::cerr << "Failed to initialize window!" << std::endl;
            return -1;
        }

        // Initialize renderer
        g_renderer = std::make_unique<Renderer>();
        if (!g_renderer->Initialize(g_window->GetHWND()))
        {
            std::cerr << "Failed to initialize renderer!" << std::endl;
            return -1;
        }

        // Initialize UI manager
        g_uiManager = std::make_unique<UIManager>();
        if (!g_uiManager->Initialize(g_window->GetHWND(), g_renderer->GetDevice(), g_renderer->GetDeviceContext()))
        {
            std::cerr << "Failed to initialize UI!" << std::endl;
            return -1;
        }

        // Initialize object detector
        g_objectDetector = std::make_unique<ObjectDetector>();
        if (!g_objectDetector->Initialize("assets/models/yolov8n.onnx"))
        {
            std::cerr << "Warning: Failed to initialize object detector. Detection features disabled." << std::endl;
        }

        // Initialize tracking system
        g_trackingSystem = std::make_unique<TrackingSystem>();

        std::cout << "All systems initialized successfully!" << std::endl;

        // Main loop
        MSG msg = {};
        auto lastTime = std::chrono::high_resolution_clock::now();
        float targetFrameTime = 1.0f / 120.0f; // 120 FPS

        while (msg.message != WM_QUIT)
        {
            if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
                continue;
            }

            // Calculate delta time
            auto currentTime = std::chrono::high_resolution_clock::now();
            float deltaTime = std::chrono::duration<float>(currentTime - lastTime).count();

            if (deltaTime >= targetFrameTime)
            {
                lastTime = currentTime;

                // Update systems
                if (g_settingsManager->GetSettings().trackingEnabled)
                {
                    g_trackingSystem->Update(deltaTime);
                }

                // Render frame
                g_renderer->BeginFrame();

                // Render FOV overlay if enabled
                if (g_settingsManager->GetSettings().fovOverlayEnabled)
                {
                    g_renderer->RenderFOVOverlay(
                        g_settingsManager->GetSettings().fovSize,
                        g_settingsManager->GetSettings().fovColor,
                        g_settingsManager->GetSettings().fovTransparency
                    );
                }

                // Render detected objects if enabled
                if (g_settingsManager->GetSettings().detectionEnabled && g_objectDetector->IsInitialized())
                {
                    auto detections = g_objectDetector->GetDetections();
                    g_renderer->RenderDetections(detections);
                }

                // Render UI
                g_uiManager->Render(g_settingsManager.get(), g_objectDetector.get(), g_trackingSystem.get());

                g_renderer->EndFrame();
            }
        }

        // Cleanup
        std::cout << "Shutting down..." << std::endl;
        g_settingsManager->SaveSettings();
        
        g_trackingSystem.reset();
        g_objectDetector.reset();
        g_uiManager.reset();
        g_renderer.reset();
        g_window.reset();
        g_settingsManager.reset();
    }
    catch (const std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << std::endl;
        MessageBoxA(NULL, e.what(), "Error", MB_OK | MB_ICONERROR);
        return -1;
    }

    return 0;
}
