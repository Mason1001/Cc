/*
 * Aim Tracking System
 */

#pragma once
#include <DirectXMath.h>

using namespace DirectX;

class TrackingSystem
{
public:
    TrackingSystem();
    ~TrackingSystem();

    void Update(float deltaTime);
    void SetTarget(const XMFLOAT2& target);
    void ClearTarget();

    bool HasTarget() const { return m_hasTarget; }
    XMFLOAT2 GetCurrentPosition() const { return m_currentPosition; }
    XMFLOAT2 GetTargetPosition() const { return m_targetPosition; }

    void SetPrecision(float precision) { m_precision = precision; }
    void SetSmoothness(float smoothness) { m_smoothness = smoothness; }
    void SetSpeed(float speed) { m_speed = speed; }

private:
    bool m_hasTarget;
    XMFLOAT2 m_currentPosition;
    XMFLOAT2 m_targetPosition;
    XMFLOAT2 m_velocity;

    float m_precision;
    float m_smoothness;
    float m_speed;
};
