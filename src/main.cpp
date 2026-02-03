#include "overlay/overlay_manager.h"
#include "overlay/fov_renderer.h"
#include "detection/yolo_detector.h"
#include "detection/object_tracker.h"
#include "sound/sound_tracker.h"
#include "config/config_manager.h"
#include "ui/imgui_manager.h"

#include <d3d11.h>
#include <iostream>
#include <memory>
#include <chrono>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

using namespace fov;

class Application {
public:
    Application() 
        : d3d_device_(nullptr)
        , d3d_context_(nullptr)
        , swap_chain_(nullptr)
        , render_target_view_(nullptr)
        , show_menu_(true)
        , running_(true) {
    }
    
    ~Application() {
        Shutdown();
    }
    
    bool Initialize() {
        std::cout << "=== FOV Overlay Application ===" << std::endl;
        std::cout << "Initializing..." << std::endl;
        
        // Load configuration
        config_manager_ = std::make_unique<ConfigManager>();
        if (!config_manager_->LoadFromFile("settings.json")) {
            std::cerr << "Warning: Could not load settings.json, using defaults" << std::endl;
        }
        
        auto& overlay_config = config_manager_->GetOverlayConfig();
        
        // Initialize overlay window
        overlay_manager_ = std::make_unique<OverlayManager>();
        if (!overlay_manager_->Initialize(overlay_config.width, overlay_config.height, 
                                         overlay_config.transparent, overlay_config.topmost)) {
            std::cerr << "Failed to initialize overlay" << std::endl;
            return false;
        }
        
        // Initialize DirectX
        if (!InitializeDirectX()) {
            std::cerr << "Failed to initialize DirectX" << std::endl;
            return false;
        }
        
        // Initialize FOV renderer
        fov_renderer_ = std::make_unique<FOVRenderer>();
        if (!fov_renderer_->Initialize(d3d_device_, d3d_context_)) {
            std::cerr << "Failed to initialize FOV renderer" << std::endl;
            return false;
        }
        fov_renderer_->SetConfig(config_manager_->GetFOVConfig());
        
        // Initialize YOLO detector
        auto& detection_config = config_manager_->GetDetectionConfig();
        if (detection_config.enabled) {
            yolo_detector_ = std::make_unique<YOLODetector>();
            if (!yolo_detector_->Initialize(detection_config.model_path, 
                                           detection_config.confidence_threshold,
                                           detection_config.iou_threshold)) {
                std::cerr << "Warning: Failed to initialize YOLO detector" << std::endl;
            }
            
            // Initialize object tracker
            object_tracker_ = std::make_unique<ObjectTracker>();
            object_tracker_->SetFriendClasses(detection_config.friend_classes);
            object_tracker_->SetEnemyClasses(detection_config.enemy_classes);
        }
        
        // Initialize sound tracker
        auto& sound_config = config_manager_->GetSoundTrackingConfig();
        if (sound_config.enabled) {
            sound_tracker_ = std::make_unique<SoundTracker>();
            if (!sound_tracker_->Initialize()) {
                std::cerr << "Warning: Failed to initialize sound tracker" << std::endl;
            }
            sound_tracker_->SetConfig(sound_config);
        }
        
        // Initialize ImGui
        imgui_manager_ = std::make_unique<ImGuiManager>();
        if (!imgui_manager_->Initialize(overlay_manager_->GetWindow(), d3d_device_, d3d_context_)) {
            std::cerr << "Warning: Failed to initialize ImGui" << std::endl;
        }
        
        std::cout << "Initialization complete!" << std::endl;
        std::cout << "Press INSERT to toggle menu" << std::endl;
        std::cout << "Press ESC to exit" << std::endl;
        
        return true;
    }
    
    void Run() {
        auto last_frame_time = std::chrono::high_resolution_clock::now();
        int frame_count = 0;
        float fps_timer = 0.0f;
        
        while (overlay_manager_->IsRunning() && running_) {
            // Process window messages
            overlay_manager_->ProcessMessages();
            
            // Check for ESC key to exit
            if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
                running_ = false;
                break;
            }
            
            // Toggle menu with INSERT key
            static bool insert_pressed = false;
            if (GetAsyncKeyState(VK_INSERT) & 0x8000) {
                if (!insert_pressed) {
                    show_menu_ = !show_menu_;
                    insert_pressed = true;
                }
            } else {
                insert_pressed = false;
            }
            
            // Calculate delta time
            auto current_time = std::chrono::high_resolution_clock::now();
            float delta_time = std::chrono::duration<float>(current_time - last_frame_time).count();
            last_frame_time = current_time;
            
            // Update FPS counter
            frame_count++;
            fps_timer += delta_time;
            if (fps_timer >= 1.0f) {
                float fps = frame_count / fps_timer;
                std::cout << "FPS: " << fps << std::endl;
                frame_count = 0;
                fps_timer = 0.0f;
            }
            
            // Update sound tracker
            if (sound_tracker_) {
                sound_tracker_->Update(delta_time);
            }
            
            // Render frame
            Render();
            
            // Limit FPS to target
            auto target_fps = config_manager_->GetFOVConfig().target_fps;
            if (target_fps > 0) {
                float target_frame_time = 1.0f / target_fps;
                float elapsed = std::chrono::duration<float>(
                    std::chrono::high_resolution_clock::now() - current_time).count();
                if (elapsed < target_frame_time) {
                    std::this_thread::sleep_for(
                        std::chrono::milliseconds(static_cast<int>((target_frame_time - elapsed) * 1000)));
                }
            }
        }
    }
    
    void Shutdown() {
        std::cout << "Shutting down..." << std::endl;
        
        if (imgui_manager_) {
            imgui_manager_->Shutdown();
        }
        
        if (sound_tracker_) {
            sound_tracker_->Shutdown();
        }
        
        if (fov_renderer_) {
            fov_renderer_->Shutdown();
        }
        
        if (yolo_detector_) {
            yolo_detector_->Shutdown();
        }
        
        ShutdownDirectX();
        
        if (overlay_manager_) {
            overlay_manager_->Shutdown();
        }
        
        std::cout << "Shutdown complete" << std::endl;
    }

private:
    bool InitializeDirectX() {
        DXGI_SWAP_CHAIN_DESC swap_chain_desc = {};
        swap_chain_desc.BufferCount = 2;
        swap_chain_desc.BufferDesc.Width = config_manager_->GetOverlayConfig().width;
        swap_chain_desc.BufferDesc.Height = config_manager_->GetOverlayConfig().height;
        swap_chain_desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        swap_chain_desc.BufferDesc.RefreshRate.Numerator = 0;
        swap_chain_desc.BufferDesc.RefreshRate.Denominator = 1;
        swap_chain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        swap_chain_desc.OutputWindow = overlay_manager_->GetWindow();
        swap_chain_desc.SampleDesc.Count = 1;
        swap_chain_desc.SampleDesc.Quality = 0;
        swap_chain_desc.Windowed = TRUE;
        swap_chain_desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
        swap_chain_desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
        
        D3D_FEATURE_LEVEL feature_level;
        UINT create_flags = 0;
#ifdef _DEBUG
        create_flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
        
        HRESULT hr = D3D11CreateDeviceAndSwapChain(
            nullptr,
            D3D_DRIVER_TYPE_HARDWARE,
            nullptr,
            create_flags,
            nullptr,
            0,
            D3D11_SDK_VERSION,
            &swap_chain_desc,
            &swap_chain_,
            &d3d_device_,
            &feature_level,
            &d3d_context_
        );
        
        if (FAILED(hr)) {
            std::cerr << "Failed to create D3D11 device and swap chain" << std::endl;
            return false;
        }
        
        // Create render target view
        ID3D11Texture2D* back_buffer = nullptr;
        hr = swap_chain_->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&back_buffer);
        if (FAILED(hr)) {
            std::cerr << "Failed to get back buffer" << std::endl;
            return false;
        }
        
        hr = d3d_device_->CreateRenderTargetView(back_buffer, nullptr, &render_target_view_);
        back_buffer->Release();
        
        if (FAILED(hr)) {
            std::cerr << "Failed to create render target view" << std::endl;
            return false;
        }
        
        d3d_context_->OMSetRenderTargets(1, &render_target_view_, nullptr);
        
        // Setup viewport
        D3D11_VIEWPORT viewport = {};
        viewport.Width = static_cast<float>(config_manager_->GetOverlayConfig().width);
        viewport.Height = static_cast<float>(config_manager_->GetOverlayConfig().height);
        viewport.MinDepth = 0.0f;
        viewport.MaxDepth = 1.0f;
        viewport.TopLeftX = 0.0f;
        viewport.TopLeftY = 0.0f;
        
        d3d_context_->RSSetViewports(1, &viewport);
        
        return true;
    }
    
    void ShutdownDirectX() {
        if (render_target_view_) {
            render_target_view_->Release();
            render_target_view_ = nullptr;
        }
        if (swap_chain_) {
            swap_chain_->Release();
            swap_chain_ = nullptr;
        }
        if (d3d_context_) {
            d3d_context_->Release();
            d3d_context_ = nullptr;
        }
        if (d3d_device_) {
            d3d_device_->Release();
            d3d_device_ = nullptr;
        }
    }
    
    void Render() {
        // Clear render target
        float clear_color[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
        d3d_context_->ClearRenderTargetView(render_target_view_, clear_color);
        
        // Get screen center
        float center_x = config_manager_->GetOverlayConfig().width / 2.0f;
        float center_y = config_manager_->GetOverlayConfig().height / 2.0f;
        
        // Render FOV
        if (fov_renderer_ && config_manager_->GetFOVConfig().enabled) {
            fov_renderer_->RenderFOV(center_x, center_y);
        }
        
        // Render detected objects
        if (yolo_detector_ && object_tracker_) {
            // In real implementation, would capture screen and run detection
            // For demonstration, using placeholder
            auto& detection_config = config_manager_->GetDetectionConfig();
            
            // Render detection boxes (simulated)
            // Would normally call: auto detections = yolo_detector_->Detect(...)
            // Then: auto tracked = object_tracker_->Update(detections)
        }
        
        // Render sound indicators
        if (sound_tracker_) {
            auto sources = sound_tracker_->GetActiveSources();
            auto& sound_config = config_manager_->GetSoundTrackingConfig();
            
            for (const auto& source : sources) {
                // Convert azimuth/elevation to screen coordinates
                float x = center_x + cosf(source.azimuth * 3.14159f / 180.0f) * 200.0f;
                float y = center_y + sinf(source.elevation * 3.14159f / 180.0f) * 200.0f;
                
                if (fov_renderer_) {
                    fov_renderer_->RenderCircle(x, y, sound_config.indicator_size, 
                                               sound_config.indicator_color, 2.0f, true);
                }
            }
        }
        
        // Render UI
        if (imgui_manager_ && show_menu_) {
            imgui_manager_->BeginFrame();
            imgui_manager_->RenderUI();
            imgui_manager_->EndFrame();
        }
        
        // Present
        swap_chain_->Present(config_manager_->GetOverlayConfig().vsync ? 1 : 0, 0);
    }
    
    std::unique_ptr<OverlayManager> overlay_manager_;
    std::unique_ptr<FOVRenderer> fov_renderer_;
    std::unique_ptr<YOLODetector> yolo_detector_;
    std::unique_ptr<ObjectTracker> object_tracker_;
    std::unique_ptr<SoundTracker> sound_tracker_;
    std::unique_ptr<ConfigManager> config_manager_;
    std::unique_ptr<ImGuiManager> imgui_manager_;
    
    ID3D11Device* d3d_device_;
    ID3D11DeviceContext* d3d_context_;
    IDXGISwapChain* swap_chain_;
    ID3D11RenderTargetView* render_target_view_;
    
    bool show_menu_;
    bool running_;
};

int main(int argc, char* argv[]) {
    try {
        Application app;
        
        if (!app.Initialize()) {
            std::cerr << "Failed to initialize application" << std::endl;
            return 1;
        }
        
        app.Run();
        app.Shutdown();
        
        return 0;
    }
    catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return 1;
    }
}
