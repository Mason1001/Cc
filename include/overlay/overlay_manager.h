#pragma once

#include <windows.h>
#include <string>
#include <memory>

namespace fov {

class OverlayManager {
public:
    OverlayManager();
    ~OverlayManager();

    bool Initialize(int width, int height, bool transparent = true, bool topmost = true);
    void Shutdown();
    
    bool IsRunning() const { return running_; }
    void ProcessMessages();
    
    HWND GetWindow() const { return hwnd_; }
    
    void BeginFrame();
    void EndFrame();
    
    float GetDeltaTime() const { return delta_time_; }
    float GetFPS() const { return fps_; }

private:
    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
    
    HWND hwnd_;
    bool running_;
    float delta_time_;
    float fps_;
    LARGE_INTEGER last_time_;
    LARGE_INTEGER frequency_;
};

} // namespace fov
