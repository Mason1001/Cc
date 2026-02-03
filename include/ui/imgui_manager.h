#pragma once

#include <d3d11.h>
#include <memory>

struct ImGuiContext;

namespace fov {

class ImGuiManager {
public:
    ImGuiManager();
    ~ImGuiManager();

    bool Initialize(HWND hwnd, ID3D11Device* device, ID3D11DeviceContext* context);
    void Shutdown();
    
    void BeginFrame();
    void EndFrame();
    
    void RenderUI();
    
    bool WantCaptureMouse() const;
    bool WantCaptureKeyboard() const;

private:
    ImGuiContext* context_;
    bool initialized_;
};

} // namespace fov
