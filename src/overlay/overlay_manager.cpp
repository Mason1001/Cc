#include "overlay/overlay_manager.h"
#include <iostream>

namespace fov {

static OverlayManager* g_overlay_instance = nullptr;

OverlayManager::OverlayManager() 
    : hwnd_(nullptr)
    , running_(false)
    , delta_time_(0.0f)
    , fps_(0.0f) {
    g_overlay_instance = this;
    QueryPerformanceFrequency(&frequency_);
    QueryPerformanceCounter(&last_time_);
}

OverlayManager::~OverlayManager() {
    Shutdown();
    g_overlay_instance = nullptr;
}

bool OverlayManager::Initialize(int width, int height, bool transparent, bool topmost) {
    WNDCLASSEXW wc = {};
    wc.cbSize = sizeof(WNDCLASSEXW);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = GetModuleHandle(nullptr);
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.lpszClassName = L"FOVOverlayClass";
    
    if (!RegisterClassExW(&wc)) {
        std::cerr << "Failed to register window class" << std::endl;
        return false;
    }
    
    DWORD style = WS_POPUP;
    DWORD ex_style = WS_EX_TOPMOST | WS_EX_LAYERED | WS_EX_TRANSPARENT;
    
    if (transparent) {
        ex_style |= WS_EX_TRANSPARENT;
    }
    
    hwnd_ = CreateWindowExW(
        ex_style,
        L"FOVOverlayClass",
        L"FOV Overlay",
        style,
        0, 0, width, height,
        nullptr, nullptr,
        GetModuleHandle(nullptr),
        nullptr
    );
    
    if (!hwnd_) {
        std::cerr << "Failed to create window" << std::endl;
        return false;
    }
    
    if (transparent) {
        SetLayeredWindowAttributes(hwnd_, RGB(0, 0, 0), 0, LWA_COLORKEY);
    }
    
    ShowWindow(hwnd_, SW_SHOW);
    UpdateWindow(hwnd_);
    
    running_ = true;
    return true;
}

void OverlayManager::Shutdown() {
    if (hwnd_) {
        DestroyWindow(hwnd_);
        hwnd_ = nullptr;
    }
    running_ = false;
}

void OverlayManager::ProcessMessages() {
    MSG msg;
    while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
        if (msg.message == WM_QUIT) {
            running_ = false;
        }
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

void OverlayManager::BeginFrame() {
    LARGE_INTEGER current_time;
    QueryPerformanceCounter(&current_time);
    
    delta_time_ = static_cast<float>(current_time.QuadPart - last_time_.QuadPart) / frequency_.QuadPart;
    fps_ = 1.0f / delta_time_;
    
    last_time_ = current_time;
}

void OverlayManager::EndFrame() {
    // Frame timing handled in BeginFrame
}

LRESULT CALLBACK OverlayManager::WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        case WM_KEYDOWN:
            if (wParam == VK_INSERT) {
                // Toggle menu (handled in main loop)
            }
            break;
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

} // namespace fov
