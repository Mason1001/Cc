#include "overlay/fov_renderer.h"
#include <iostream>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace fov {

FOVRenderer::FOVRenderer()
    : device_(nullptr)
    , context_(nullptr)
    , vertex_buffer_(nullptr)
    , constant_buffer_(nullptr)
    , vertex_shader_(nullptr)
    , pixel_shader_(nullptr)
    , input_layout_(nullptr)
    , blend_state_(nullptr) {
    
    config_.enabled = true;
    config_.radius = 100.0f;
    config_.color[0] = 1.0f;
    config_.color[1] = 0.0f;
    config_.color[2] = 0.0f;
    config_.color[3] = 0.8f;
    config_.thickness = 2.0f;
    config_.filled = false;
    config_.target_fps = 120;
}

FOVRenderer::~FOVRenderer() {
    Shutdown();
}

bool FOVRenderer::Initialize(ID3D11Device* device, ID3D11DeviceContext* context) {
    device_ = device;
    context_ = context;
    
    if (!device_ || !context_) {
        return false;
    }
    
    // Create vertex buffer
    D3D11_BUFFER_DESC buffer_desc = {};
    buffer_desc.Usage = D3D11_USAGE_DYNAMIC;
    buffer_desc.ByteWidth = sizeof(DirectX::XMFLOAT2) * 1024; // Max vertices
    buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    
    HRESULT hr = device_->CreateBuffer(&buffer_desc, nullptr, &vertex_buffer_);
    if (FAILED(hr)) {
        std::cerr << "Failed to create vertex buffer" << std::endl;
        return false;
    }
    
    // Create blend state for transparency
    D3D11_BLEND_DESC blend_desc = {};
    blend_desc.RenderTarget[0].BlendEnable = TRUE;
    blend_desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
    blend_desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
    blend_desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    blend_desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
    blend_desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
    blend_desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    blend_desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
    
    hr = device_->CreateBlendState(&blend_desc, &blend_state_);
    if (FAILED(hr)) {
        std::cerr << "Failed to create blend state" << std::endl;
        return false;
    }
    
    return true;
}

void FOVRenderer::Shutdown() {
    if (vertex_buffer_) {
        vertex_buffer_->Release();
        vertex_buffer_ = nullptr;
    }
    if (constant_buffer_) {
        constant_buffer_->Release();
        constant_buffer_ = nullptr;
    }
    if (vertex_shader_) {
        vertex_shader_->Release();
        vertex_shader_ = nullptr;
    }
    if (pixel_shader_) {
        pixel_shader_->Release();
        pixel_shader_ = nullptr;
    }
    if (input_layout_) {
        input_layout_->Release();
        input_layout_ = nullptr;
    }
    if (blend_state_) {
        blend_state_->Release();
        blend_state_ = nullptr;
    }
}

void FOVRenderer::SetConfig(const FOVConfig& config) {
    config_ = config;
}

void FOVRenderer::RenderFOV(float center_x, float center_y) {
    if (!config_.enabled) {
        return;
    }
    
    RenderCircle(center_x, center_y, config_.radius, config_.color, config_.thickness, config_.filled);
}

void FOVRenderer::RenderCircle(float x, float y, float radius, const float color[4], float thickness, bool filled) {
    if (!context_) {
        return;
    }
    
    std::vector<DirectX::XMFLOAT2> vertices;
    CreateCircleVertices(x, y, radius, 64, vertices);
    
    // Set blend state
    if (blend_state_) {
        float blend_factor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
        context_->OMSetBlendState(blend_state_, blend_factor, 0xffffffff);
    }
    
    // In a full implementation, would render the vertices using shaders
    // For now, this is a placeholder showing the structure
}

void FOVRenderer::RenderLine(float x1, float y1, float x2, float y2, const float color[4], float thickness) {
    if (!context_) {
        return;
    }
    
    // Line rendering implementation placeholder
}

void FOVRenderer::RenderBox(float x, float y, float width, float height, const float color[4], float thickness) {
    if (!context_) {
        return;
    }
    
    RenderLine(x, y, x + width, y, color, thickness);
    RenderLine(x + width, y, x + width, y + height, color, thickness);
    RenderLine(x + width, y + height, x, y + height, color, thickness);
    RenderLine(x, y + height, x, y, color, thickness);
}

void FOVRenderer::CreateCircleVertices(float x, float y, float radius, int segments, std::vector<DirectX::XMFLOAT2>& vertices) {
    vertices.clear();
    vertices.reserve(segments + 1);
    
    for (int i = 0; i <= segments; i++) {
        float angle = 2.0f * M_PI * i / segments;
        float px = x + radius * cosf(angle);
        float py = y + radius * sinf(angle);
        vertices.push_back(DirectX::XMFLOAT2(px, py));
    }
}

} // namespace fov
