/*
 * Overlay Window Implementation
 */

#include "overlay_window.h"
#include <iostream>

OverlayWindow::OverlayWindow(HINSTANCE hInstance, WNDPROC wndProc)
    : m_hInstance(hInstance)
    , m_hwnd(NULL)
    , m_wndProc(wndProc)
    , m_width(0)
    , m_height(0)
{
}

OverlayWindow::~OverlayWindow()
{
    Cleanup();
}

bool OverlayWindow::Initialize(int width, int height, const std::string& title)
{
    m_width = width;
    m_height = height;
    m_title = title;

    // Register window class
    WNDCLASSEX wc = {};
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_CLASSDC;
    wc.lpfnWndProc = m_wndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = m_hInstance;
    wc.hIcon = NULL;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = NULL;
    wc.lpszMenuName = NULL;
    wc.lpszClassName = "GamingOverlayClass";
    wc.hIconSm = NULL;

    if (!RegisterClassEx(&wc))
    {
        std::cerr << "Failed to register window class!" << std::endl;
        return false;
    }

    // Create window
    m_hwnd = CreateWindowEx(
        WS_EX_TOPMOST | WS_EX_LAYERED | WS_EX_TRANSPARENT,
        wc.lpszClassName,
        m_title.c_str(),
        WS_POPUP,
        0, 0, m_width, m_height,
        NULL,
        NULL,
        m_hInstance,
        NULL
    );

    if (!m_hwnd)
    {
        std::cerr << "Failed to create window!" << std::endl;
        return false;
    }

    // Make window semi-transparent overlay
    SetLayeredWindowAttributes(m_hwnd, RGB(0, 0, 0), 0, LWA_COLORKEY);

    // Show window
    ShowWindow(m_hwnd, SW_SHOWDEFAULT);
    UpdateWindow(m_hwnd);

    std::cout << "Window initialized: " << m_width << "x" << m_height << std::endl;

    return true;
}

void OverlayWindow::Cleanup()
{
    if (m_hwnd)
    {
        DestroyWindow(m_hwnd);
        m_hwnd = NULL;
    }

    UnregisterClass("GamingOverlayClass", m_hInstance);
}
