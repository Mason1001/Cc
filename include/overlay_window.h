/*
 * Overlay Window Management
 */

#pragma once
#include <Windows.h>
#include <string>

class OverlayWindow
{
public:
    OverlayWindow(HINSTANCE hInstance, WNDPROC wndProc);
    ~OverlayWindow();

    bool Initialize(int width, int height, const std::string& title);
    void Cleanup();

    HWND GetHWND() const { return m_hwnd; }
    int GetWidth() const { return m_width; }
    int GetHeight() const { return m_height; }

private:
    HINSTANCE m_hInstance;
    HWND m_hwnd;
    WNDPROC m_wndProc;
    int m_width;
    int m_height;
    std::string m_title;
};
