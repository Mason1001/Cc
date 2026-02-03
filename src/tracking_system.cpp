/*
 * Aim Tracking System Implementation
 */

#include "tracking_system.h"
#include <cmath>

TrackingSystem::TrackingSystem()
    : m_hasTarget(false)
    , m_currentPosition(0.0f, 0.0f)
    , m_targetPosition(0.0f, 0.0f)
    , m_velocity(0.0f, 0.0f)
    , m_precision(0.7f)
    , m_smoothness(0.5f)
    , m_speed(3.0f)
{
}

TrackingSystem::~TrackingSystem()
{
}

void TrackingSystem::Update(float deltaTime)
{
    if (!m_hasTarget) return;

    // Calculate direction to target
    XMFLOAT2 direction;
    direction.x = m_targetPosition.x - m_currentPosition.x;
    direction.y = m_targetPosition.y - m_currentPosition.y;

    // Calculate distance to target
    float distance = sqrt(direction.x * direction.x + direction.y * direction.y);

    // Check if we're close enough to the target based on precision
    float threshold = 0.01f * (1.0f - m_precision);
    if (distance < threshold)
    {
        m_currentPosition = m_targetPosition;
        m_velocity.x = 0.0f;
        m_velocity.y = 0.0f;
        return;
    }

    // Normalize direction
    if (distance > 0.0f)
    {
        direction.x /= distance;
        direction.y /= distance;
    }

    // Apply smoothness to acceleration
    float acceleration = m_speed * (1.0f - m_smoothness * 0.9f);
    
    // Update velocity with smoothing
    float smoothFactor = 1.0f - exp(-acceleration * deltaTime);
    m_velocity.x = m_velocity.x * (1.0f - smoothFactor) + direction.x * m_speed * smoothFactor;
    m_velocity.y = m_velocity.y * (1.0f - smoothFactor) + direction.y * m_speed * smoothFactor;

    // Apply velocity to position
    m_currentPosition.x += m_velocity.x * deltaTime;
    m_currentPosition.y += m_velocity.y * deltaTime;

    // Add natural variation (slight wobble for realism)
    float wobble = (1.0f - m_precision) * 0.002f;
    static float wobbleTime = 0.0f;
    wobbleTime += deltaTime * 10.0f;
    m_currentPosition.x += sin(wobbleTime) * wobble;
    m_currentPosition.y += cos(wobbleTime * 0.7f) * wobble;
}

void TrackingSystem::SetTarget(const XMFLOAT2& target)
{
    m_targetPosition = target;
    m_hasTarget = true;
}

void TrackingSystem::ClearTarget()
{
    m_hasTarget = false;
    m_velocity.x = 0.0f;
    m_velocity.y = 0.0f;
}
