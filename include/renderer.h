/*
 * DirectX 11 Renderer
 */

#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include <wrl/client.h>
#include <vector>
#include "detection_result.h"

using namespace DirectX;
using Microsoft::WRL::ComPtr;

class Renderer
{
public:
    Renderer();
    ~Renderer();

    bool Initialize(HWND hwnd);
    void Cleanup();

    void BeginFrame();
    void EndFrame();
    void ResizeBuffers(UINT width, UINT height);

    void RenderFOVOverlay(float size, const float color[4], float transparency);
    void RenderDetections(const std::vector<DetectionResult>& detections);
    void RenderBox(const XMFLOAT2& topLeft, const XMFLOAT2& bottomRight, const float color[4], float thickness = 2.0f);
    void RenderCircle(const XMFLOAT2& center, float radius, const float color[4], int segments = 64);

    ID3D11Device* GetDevice() const { return m_device.Get(); }
    ID3D11DeviceContext* GetDeviceContext() const { return m_context.Get(); }

private:
    void CreateLineBuffer();
    void DrawLine(const XMFLOAT2& start, const XMFLOAT2& end, const float color[4]);

    ComPtr<ID3D11Device> m_device;
    ComPtr<ID3D11DeviceContext> m_context;
    ComPtr<IDXGISwapChain> m_swapChain;
    ComPtr<ID3D11RenderTargetView> m_renderTargetView;
    ComPtr<ID3D11VertexShader> m_vertexShader;
    ComPtr<ID3D11PixelShader> m_pixelShader;
    ComPtr<ID3D11InputLayout> m_inputLayout;
    ComPtr<ID3D11Buffer> m_vertexBuffer;
    ComPtr<ID3D11BlendState> m_blendState;

    UINT m_width;
    UINT m_height;
    bool m_initialized;
};
