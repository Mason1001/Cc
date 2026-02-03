#pragma once

#include <string>
#include <vector>
#include <memory>

namespace fov {

struct DetectionBox {
    float x, y, width, height;
    int class_id;
    float confidence;
    std::string label;
    bool is_friend;
};

class YOLODetector {
public:
    YOLODetector();
    ~YOLODetector();

    bool Initialize(const std::string& model_path, float conf_threshold = 0.5f, float iou_threshold = 0.45f);
    void Shutdown();
    
    std::vector<DetectionBox> Detect(const unsigned char* image_data, int width, int height, int channels);
    
    void SetConfidenceThreshold(float threshold) { conf_threshold_ = threshold; }
    void SetIOUThreshold(float threshold) { iou_threshold_ = threshold; }
    
    float GetConfidenceThreshold() const { return conf_threshold_; }
    float GetIOUThreshold() const { return iou_threshold_; }

private:
    bool LoadModel(const std::string& model_path);
    void Preprocess(const unsigned char* image_data, int width, int height, int channels);
    std::vector<DetectionBox> Postprocess();
    
    std::string model_path_;
    float conf_threshold_;
    float iou_threshold_;
    bool initialized_;
    
    // Model-specific data (placeholder for ONNX Runtime or PyTorch C++ API)
    void* model_handle_;
};

} // namespace fov
