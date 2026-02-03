#pragma once

#include "yolo_detector.h"
#include <map>
#include <chrono>

namespace fov {

struct TrackedObject {
    DetectionBox box;
    int track_id;
    std::chrono::steady_clock::time_point last_seen;
    float velocity_x;
    float velocity_y;
};

class ObjectTracker {
public:
    ObjectTracker();
    ~ObjectTracker();

    void SetFriendClasses(const std::vector<int>& classes) { friend_classes_ = classes; }
    void SetEnemyClasses(const std::vector<int>& classes) { enemy_classes_ = classes; }
    
    std::vector<TrackedObject> Update(const std::vector<DetectionBox>& detections);
    
    void Clear();

private:
    int GetNextTrackId();
    float CalculateIOU(const DetectionBox& box1, const DetectionBox& box2);
    
    std::vector<int> friend_classes_;
    std::vector<int> enemy_classes_;
    std::map<int, TrackedObject> tracked_objects_;
    int next_track_id_;
};

} // namespace fov
