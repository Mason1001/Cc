#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include <memory>
#include <vector>

namespace fov {

struct FOVConfig {
    bool enabled;
    float radius;
    float color[4];  // RGBA
    float thickness;
    bool filled;
    int target_fps;
};

class FOVRenderer {
public:
    FOVRenderer();
    ~FOVRenderer();

    bool Initialize(ID3D11Device* device, ID3D11DeviceContext* context);
    void Shutdown();
    
    void SetConfig(const FOVConfig& config);
    const FOVConfig& GetConfig() const { return config_; }
    
    void RenderFOV(float center_x, float center_y);
    void RenderCircle(float x, float y, float radius, const float color[4], float thickness, bool filled = false);
    void RenderLine(float x1, float y1, float x2, float y2, const float color[4], float thickness);
    void RenderBox(float x, float y, float width, float height, const float color[4], float thickness);

private:
    void CreateCircleVertices(float x, float y, float radius, int segments, std::vector<DirectX::XMFLOAT2>& vertices);
    
    ID3D11Device* device_;
    ID3D11DeviceContext* context_;
    FOVConfig config_;
    
    ID3D11Buffer* vertex_buffer_;
    ID3D11Buffer* constant_buffer_;
    ID3D11VertexShader* vertex_shader_;
    ID3D11PixelShader* pixel_shader_;
    ID3D11InputLayout* input_layout_;
    ID3D11BlendState* blend_state_;
};

} // namespace fov
