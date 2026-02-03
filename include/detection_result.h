/*
 * Detection Result Structure
 */

#pragma once
#include <DirectXMath.h>

using namespace DirectX;

struct DetectionResult
{
    XMFLOAT2 topLeft;
    XMFLOAT2 bottomRight;
    float confidence;
    int classId;
    bool isEnemy;
    
    DetectionResult()
        : topLeft(0.0f, 0.0f)
        , bottomRight(0.0f, 0.0f)
        , confidence(0.0f)
        , classId(0)
        , isEnemy(false)
    {
    }
};
