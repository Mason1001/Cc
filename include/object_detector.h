/*
 * Object Detector using YOLOv8
 */

#pragma once
#include <string>
#include <vector>
#include "detection_result.h"

class ObjectDetector
{
public:
    ObjectDetector();
    ~ObjectDetector();

    bool Initialize(const std::string& modelPath);
    void Cleanup();

    void Update();
    std::vector<DetectionResult> GetDetections() const { return m_detections; }
    
    bool IsInitialized() const { return m_initialized; }

private:
    bool LoadModel(const std::string& modelPath);
    void RunInference();
    void ProcessDetections();

    bool m_initialized;
    std::vector<DetectionResult> m_detections;
    std::string m_modelPath;
};
