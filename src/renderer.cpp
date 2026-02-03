/*
 * DirectX 11 Renderer Implementation
 */

#include "renderer.h"
#include <iostream>
#include <d3dcompiler.h>

#pragma comment(lib, "d3dcompiler.lib")

// Vertex structure
struct Vertex
{
    XMFLOAT3 position;
    XMFLOAT4 color;
};

// Simple vertex shader
const char* g_vertexShaderSource = R"(
struct VS_INPUT {
    float3 pos : POSITION;
    float4 color : COLOR;
};

struct VS_OUTPUT {
    float4 pos : SV_POSITION;
    float4 color : COLOR;
};

VS_OUTPUT main(VS_INPUT input) {
    VS_OUTPUT output;
    output.pos = float4(input.pos, 1.0f);
    output.color = input.color;
    return output;
}
)";

// Simple pixel shader
const char* g_pixelShaderSource = R"(
struct PS_INPUT {
    float4 pos : SV_POSITION;
    float4 color : COLOR;
};

float4 main(PS_INPUT input) : SV_TARGET {
    return input.color;
}
)";

Renderer::Renderer()
    : m_width(0)
    , m_height(0)
    , m_initialized(false)
{
}

Renderer::~Renderer()
{
    Cleanup();
}

bool Renderer::Initialize(HWND hwnd)
{
    // Get window dimensions
    RECT rect;
    GetClientRect(hwnd, &rect);
    m_width = rect.right - rect.left;
    m_height = rect.bottom - rect.top;

    // Create swap chain description
    DXGI_SWAP_CHAIN_DESC sd = {};
    sd.BufferCount = 2;
    sd.BufferDesc.Width = m_width;
    sd.BufferDesc.Height = m_height;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 120;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hwnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

    // Create device and swap chain
    UINT createDeviceFlags = 0;
#ifdef _DEBUG
    createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    D3D_FEATURE_LEVEL featureLevel;
    const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0 };

    HRESULT hr = D3D11CreateDeviceAndSwapChain(
        NULL,
        D3D_DRIVER_TYPE_HARDWARE,
        NULL,
        createDeviceFlags,
        featureLevelArray,
        2,
        D3D11_SDK_VERSION,
        &sd,
        &m_swapChain,
        &m_device,
        &featureLevel,
        &m_context
    );

    if (FAILED(hr))
    {
        std::cerr << "Failed to create D3D11 device and swap chain!" << std::endl;
        return false;
    }

    // Create render target view
    ComPtr<ID3D11Texture2D> backBuffer;
    m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);
    m_device->CreateRenderTargetView(backBuffer.Get(), NULL, &m_renderTargetView);

    // Compile and create vertex shader
    ComPtr<ID3DBlob> vsBlob, psBlob;
    ComPtr<ID3DBlob> errorBlob;
    
    hr = D3DCompile(g_vertexShaderSource, strlen(g_vertexShaderSource), NULL, NULL, NULL, "main", "vs_5_0", 0, 0, &vsBlob, &errorBlob);
    if (FAILED(hr))
    {
        if (errorBlob)
            std::cerr << "VS Compile Error: " << (char*)errorBlob->GetBufferPointer() << std::endl;
        return false;
    }

    hr = m_device->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), NULL, &m_vertexShader);
    if (FAILED(hr))
    {
        std::cerr << "Failed to create vertex shader!" << std::endl;
        return false;
    }

    // Compile and create pixel shader
    hr = D3DCompile(g_pixelShaderSource, strlen(g_pixelShaderSource), NULL, NULL, NULL, "main", "ps_5_0", 0, 0, &psBlob, &errorBlob);
    if (FAILED(hr))
    {
        if (errorBlob)
            std::cerr << "PS Compile Error: " << (char*)errorBlob->GetBufferPointer() << std::endl;
        return false;
    }

    hr = m_device->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), NULL, &m_pixelShader);
    if (FAILED(hr))
    {
        std::cerr << "Failed to create pixel shader!" << std::endl;
        return false;
    }

    // Create input layout
    D3D11_INPUT_ELEMENT_DESC layout[] = {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };

    hr = m_device->CreateInputLayout(layout, 2, vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), &m_inputLayout);
    if (FAILED(hr))
    {
        std::cerr << "Failed to create input layout!" << std::endl;
        return false;
    }

    // Create blend state for transparency
    D3D11_BLEND_DESC blendDesc = {};
    blendDesc.RenderTarget[0].BlendEnable = TRUE;
    blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
    blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
    blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
    blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
    blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

    hr = m_device->CreateBlendState(&blendDesc, &m_blendState);
    if (FAILED(hr))
    {
        std::cerr << "Failed to create blend state!" << std::endl;
        return false;
    }

    CreateLineBuffer();

    m_initialized = true;
    std::cout << "Renderer initialized successfully!" << std::endl;

    return true;
}

void Renderer::Cleanup()
{
    m_vertexBuffer.Reset();
    m_blendState.Reset();
    m_inputLayout.Reset();
    m_pixelShader.Reset();
    m_vertexShader.Reset();
    m_renderTargetView.Reset();
    m_swapChain.Reset();
    m_context.Reset();
    m_device.Reset();
    m_initialized = false;
}

void Renderer::BeginFrame()
{
    if (!m_initialized) return;

    // Clear render target with transparent black
    float clearColor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
    m_context->ClearRenderTargetView(m_renderTargetView.Get(), clearColor);
    m_context->OMSetRenderTargets(1, m_renderTargetView.GetAddressOf(), NULL);

    // Set viewport
    D3D11_VIEWPORT vp = {};
    vp.Width = (float)m_width;
    vp.Height = (float)m_height;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;
    m_context->RSSetViewports(1, &vp);

    // Set shaders and input layout
    m_context->VSSetShader(m_vertexShader.Get(), NULL, 0);
    m_context->PSSetShader(m_pixelShader.Get(), NULL, 0);
    m_context->IASetInputLayout(m_inputLayout.Get());
    m_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

    // Enable blending
    float blendFactor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
    m_context->OMSetBlendState(m_blendState.Get(), blendFactor, 0xffffffff);
}

void Renderer::EndFrame()
{
    if (!m_initialized) return;

    m_swapChain->Present(0, 0); // VSync off for 120 FPS
}

void Renderer::ResizeBuffers(UINT width, UINT height)
{
    if (!m_initialized) return;

    m_context->OMSetRenderTargets(0, NULL, NULL);
    m_renderTargetView.Reset();

    m_swapChain->ResizeBuffers(0, width, height, DXGI_FORMAT_UNKNOWN, 0);

    ComPtr<ID3D11Texture2D> backBuffer;
    m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);
    m_device->CreateRenderTargetView(backBuffer.Get(), NULL, &m_renderTargetView);

    m_width = width;
    m_height = height;
}

void Renderer::CreateLineBuffer()
{
    // Create a dynamic vertex buffer for line drawing
    D3D11_BUFFER_DESC bd = {};
    bd.Usage = D3D11_USAGE_DYNAMIC;
    bd.ByteWidth = sizeof(Vertex) * 10000; // Large enough for many lines
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    m_device->CreateBuffer(&bd, NULL, &m_vertexBuffer);
}

void Renderer::RenderFOVOverlay(float size, const float color[4], float transparency)
{
    if (!m_initialized) return;

    // Calculate center of screen
    XMFLOAT2 center(0.0f, 0.0f);
    
    // Apply transparency to color
    float adjustedColor[4] = { color[0], color[1], color[2], color[3] * transparency };
    
    // Render circle with specified radius
    RenderCircle(center, size, adjustedColor, 64);
}

void Renderer::RenderDetections(const std::vector<DetectionResult>& detections)
{
    if (!m_initialized) return;

    for (const auto& detection : detections)
    {
        // Set color based on detection type
        float color[4];
        if (detection.isEnemy)
        {
            color[0] = 1.0f; color[1] = 0.0f; color[2] = 0.0f; color[3] = 1.0f; // Red for enemies
        }
        else
        {
            color[0] = 0.0f; color[1] = 1.0f; color[2] = 0.0f; color[3] = 1.0f; // Green for friends
        }

        // Render bounding box
        RenderBox(detection.topLeft, detection.bottomRight, color, 2.0f);
    }
}

void Renderer::RenderBox(const XMFLOAT2& topLeft, const XMFLOAT2& bottomRight, const float color[4], float thickness)
{
    if (!m_initialized) return;

    XMFLOAT2 topRight(bottomRight.x, topLeft.y);
    XMFLOAT2 bottomLeft(topLeft.x, bottomRight.y);

    // Draw four lines to form a box
    DrawLine(topLeft, topRight, color);
    DrawLine(topRight, bottomRight, color);
    DrawLine(bottomRight, bottomLeft, color);
    DrawLine(bottomLeft, topLeft, color);
}

void Renderer::RenderCircle(const XMFLOAT2& center, float radius, const float color[4], int segments)
{
    if (!m_initialized) return;

    // Convert radius from pixels to NDC
    float radiusX = radius / (m_width / 2.0f);
    float radiusY = radius / (m_height / 2.0f);

    // Draw circle using line segments
    for (int i = 0; i < segments; i++)
    {
        float angle1 = (float)i / segments * 2.0f * XM_PI;
        float angle2 = (float)(i + 1) / segments * 2.0f * XM_PI;

        XMFLOAT2 p1(
            center.x + radiusX * cos(angle1),
            center.y + radiusY * sin(angle1)
        );

        XMFLOAT2 p2(
            center.x + radiusX * cos(angle2),
            center.y + radiusY * sin(angle2)
        );

        DrawLine(p1, p2, color);
    }
}

void Renderer::DrawLine(const XMFLOAT2& start, const XMFLOAT2& end, const float color[4])
{
    if (!m_initialized) return;

    Vertex vertices[2];
    vertices[0].position = XMFLOAT3(start.x, start.y, 0.0f);
    vertices[0].color = XMFLOAT4(color[0], color[1], color[2], color[3]);
    vertices[1].position = XMFLOAT3(end.x, end.y, 0.0f);
    vertices[1].color = XMFLOAT4(color[0], color[1], color[2], color[3]);

    // Update vertex buffer
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    m_context->Map(m_vertexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
    memcpy(mappedResource.pData, vertices, sizeof(vertices));
    m_context->Unmap(m_vertexBuffer.Get(), 0);

    // Set vertex buffer and draw
    UINT stride = sizeof(Vertex);
    UINT offset = 0;
    m_context->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOf(), &stride, &offset);
    m_context->Draw(2, 0);
}
