#include "detection/object_tracker.h"
#include <algorithm>
#include <cmath>

namespace fov {

ObjectTracker::ObjectTracker()
    : next_track_id_(0) {
}

ObjectTracker::~ObjectTracker() {
    Clear();
}

std::vector<TrackedObject> ObjectTracker::Update(const std::vector<DetectionBox>& detections) {
    auto current_time = std::chrono::steady_clock::now();
    
    // Match detections to existing tracked objects
    std::vector<TrackedObject> updated_objects;
    
    for (const auto& detection : detections) {
        bool matched = false;
        int best_match_id = -1;
        float best_iou = 0.3f; // Minimum IOU threshold for matching
        
        // Find best matching tracked object
        for (auto& [id, obj] : tracked_objects_) {
            float iou = CalculateIOU(detection, obj.box);
            if (iou > best_iou) {
                best_iou = iou;
                best_match_id = id;
                matched = true;
            }
        }
        
        if (matched && best_match_id != -1) {
            // Update existing track
            auto& obj = tracked_objects_[best_match_id];
            
            // Calculate velocity
            auto time_diff = std::chrono::duration<float>(current_time - obj.last_seen).count();
            if (time_diff > 0) {
                obj.velocity_x = (detection.x - obj.box.x) / time_diff;
                obj.velocity_y = (detection.y - obj.box.y) / time_diff;
            }
            
            obj.box = detection;
            obj.last_seen = current_time;
            updated_objects.push_back(obj);
        } else {
            // Create new track
            TrackedObject new_obj;
            new_obj.box = detection;
            new_obj.track_id = GetNextTrackId();
            new_obj.last_seen = current_time;
            new_obj.velocity_x = 0.0f;
            new_obj.velocity_y = 0.0f;
            
            // Determine if friend or enemy
            bool is_friend = std::find(friend_classes_.begin(), friend_classes_.end(), 
                                      detection.class_id) != friend_classes_.end();
            new_obj.box.is_friend = is_friend;
            
            tracked_objects_[new_obj.track_id] = new_obj;
            updated_objects.push_back(new_obj);
        }
    }
    
    // Remove stale tracks (not seen for more than 1 second)
    auto it = tracked_objects_.begin();
    while (it != tracked_objects_.end()) {
        auto time_diff = std::chrono::duration<float>(current_time - it->second.last_seen).count();
        if (time_diff > 1.0f) {
            it = tracked_objects_.erase(it);
        } else {
            ++it;
        }
    }
    
    return updated_objects;
}

void ObjectTracker::Clear() {
    tracked_objects_.clear();
    next_track_id_ = 0;
}

int ObjectTracker::GetNextTrackId() {
    return next_track_id_++;
}

float ObjectTracker::CalculateIOU(const DetectionBox& box1, const DetectionBox& box2) {
    float x1 = std::max(box1.x, box2.x);
    float y1 = std::max(box1.y, box2.y);
    float x2 = std::min(box1.x + box1.width, box2.x + box2.width);
    float y2 = std::min(box1.y + box1.height, box2.y + box2.height);
    
    if (x2 < x1 || y2 < y1) {
        return 0.0f;
    }
    
    float intersection = (x2 - x1) * (y2 - y1);
    float area1 = box1.width * box1.height;
    float area2 = box2.width * box2.height;
    float union_area = area1 + area2 - intersection;
    
    return intersection / union_area;
}

} // namespace fov
