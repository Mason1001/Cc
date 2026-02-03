#pragma once

#include <DirectXMath.h>
#include <memory>

namespace fov {

struct SoundSource {
    float azimuth;      // Horizontal angle (-180 to 180)
    float elevation;    // Vertical angle (-90 to 90)
    float distance;     // Distance estimate
    float intensity;    // Sound intensity/volume
};

struct SoundTrackingConfig {
    bool enabled;
    float y_axis_sensitivity;
    float horizontal_range;
    float vertical_range;
    float indicator_color[4];
    float indicator_size;
};

class SoundTracker {
public:
    SoundTracker();
    ~SoundTracker();

    bool Initialize();
    void Shutdown();
    
    void SetConfig(const SoundTrackingConfig& config);
    const SoundTrackingConfig& GetConfig() const { return config_; }
    
    void Update(float delta_time);
    std::vector<SoundSource> GetActiveSources() const;
    
    // Simulate sound detection (in real implementation, would interface with audio API)
    void AddSoundSource(float azimuth, float elevation, float intensity);
    void ClearSoundSources();

private:
    SoundTrackingConfig config_;
    std::vector<SoundSource> active_sources_;
    bool initialized_;
};

} // namespace fov
