#include "Body.h"

Body::Body() : m_Position(0, 0), m_Velocity(0, 0), m_Acceleration(0, 0) {}

Body::Body(const Vector2D& pos, const Vector2D& vel) : m_Position(pos), m_Velocity(vel), m_Acceleration(0, 0) {}

// Using inline to optimise function calls every frame
inline void Body::ApplyForce(const Vector2D& force)
{
    m_AppliedForces.push_back(Physics::Force(force, "None"));
}

void Body::AttractBody(Body& body)
{
    Vector2D dir = m_Position - body.GetPosition();
    double dirMag = dir.GetMagnitude();
    if (dirMag != 0) // Prevent division by zero
    {
        Vector2D unitDir = dir / dirMag;
        double forceMag = (Physics::G * m_Mass * body.GetMass()) / (dirMag * dirMag);
        Vector2D force = unitDir.Multiply(forceMag);
        body.ApplyForce(force);
    }   
}

void Body::UpdatePositionHistory()
{
    // If previous position list is empty, add current position
    /*if (m_PositionHistory.size() == 0 || abs((m_Position - m_PositionHistory.back()).GetMagnitudeSqr()) > m_MinimumPreviousPosDist)
    {
        m_PositionHistory.push_back(m_Position);
    }*/

    // This change from above is currently needed for verlet
    m_PositionHistory.push_back(m_Position);

    // Ensure previous position list doesn't store more than maximum amount of positions
    if (m_PositionHistory.size() > m_MaximumPosHistorySize)
    {
        m_PositionHistory.pop_front();
    }
}

void Body::UpdateForces()
{
    Vector2D totalForce(0, 0);

    // Accumulate total force
    for (const auto& force : m_AppliedForces)
    {
        totalForce = totalForce + force.vector;
    }

    // Convert to acceleration
    m_PreviousAcceleration = m_Acceleration;
    m_Acceleration = totalForce / m_Mass;

    m_AppliedForces.clear();
}

void Body::UpdateKinematics(double deltaTime)
{
    m_Position = m_Position + m_Velocity * deltaTime + 0.5 * m_PreviousAcceleration * deltaTime * deltaTime;
    m_Velocity = m_Velocity + 0.5 * (m_PreviousAcceleration + m_Acceleration) * deltaTime;
}

void Body::Update(double deltaTime)
{
    UpdatePositionHistory();
    UpdateForces();
    UpdateKinematics(deltaTime);
}

void Body::Draw(const Renderer& renderer) const
{
    // Check whether to draw previous positions
    if (m_ShouldDrawPosHistory)
    {
        constexpr int PREVPOS_RECT_SIZE = 2;
        constexpr int PREVPOS_RECT_OFFSET = 1;
        for (const auto& prevPos : m_PositionHistory)
        {
            renderer.DrawRectInWorld(prevPos, PREVPOS_RECT_SIZE, PREVPOS_RECT_SIZE, { 70, 70, 70, 120 });
        }
    }

    SDL_Color drawColor = m_IsHovered ? m_HoveredColor : m_Color;

    renderer.DrawRectInWorld(m_Position, m_Radius * 2, m_Radius * 2, drawColor);
}

void Body::CheckHover(const Vector2D& mousePos, const Renderer& renderer)
{
    m_IsHovered = renderer.IsPointInWorldRect(mousePos, m_Position, m_Radius * 2, m_Radius * 2);
}

bool Body::GetActive() const
{
    return m_IsActive;
}

void Body::SetActive(bool isActive)
{
    m_IsActive = isActive;
}

int Body::GetRadius() const
{
    return m_Radius;
}

void Body::SetRadius(int radius)
{
    m_Radius = radius;
}

double Body::GetMass() const
{
    return m_Mass;
}

void Body::SetMass(double mass)
{
    m_Mass = mass;
}

const Vector2D& Body::GetPosition() const
{
    return m_Position;
}

void Body::SetPosition(const Vector2D& position)
{
    m_Position = position;
}

const Vector2D& Body::GetVelocity() const
{
    return m_Velocity;
}

void Body::SetVelocity(const Vector2D& velocity)
{
    m_Velocity = velocity;
}

const Vector2D& Body::GetAcceleration() const
{
    return m_Acceleration;
}

void Body::SetAcceleration(const Vector2D& acceleration)
{
    m_Acceleration = acceleration;
}

const std::vector<Vector2D> Body::ProjectPosition(int projectionCount, const std::vector<std::unique_ptr<Body>>& bodyList) const
{
    std::vector<Vector2D> projectedPositions;
    float dt = 0.006f;

    Vector2D currentPosition = m_Position;
    Vector2D currentVelocity = m_Velocity;
    Vector2D previousAcceleration = m_Acceleration;
    Vector2D currentAcceleration = m_Acceleration;
    Vector2D previousVelocity = currentVelocity;
    for (size_t i = 0; i < projectionCount; i++)
    {
        std::vector<Physics::Force> currentForces;

        for (auto& body : bodyList)
        {
            if (body.get() == this)
            {
                continue;
            }

            Vector2D force(0, 0);
            Vector2D dir = currentPosition - body->GetPosition();
            double dirMag = dir.GetMagnitude();
            if (dirMag != 0) // Prevent division by zero
            {
                Vector2D unitDir = dir / dirMag;
                double forceMag = (-Physics::G * m_Mass * body->GetMass()) / (dirMag * dirMag);
                force = unitDir.Multiply(forceMag);
            }
            currentForces.push_back(Physics::Force(force, "Gravitational Attraction"));  
        }

        Vector2D totalForce(0, 0);

        // Accumulate total force
        for (const auto& force : currentForces)
        {
            totalForce = totalForce + force.vector;
        }

        // Convert to acceleration
        previousAcceleration = currentAcceleration;
        currentAcceleration = totalForce / m_Mass;

        currentForces.clear();

        Vector2D newPosition = currentPosition + currentVelocity * dt + 0.5 * previousAcceleration * dt * dt;
        projectedPositions.push_back(newPosition);
        
        currentPosition = newPosition;
        currentVelocity = currentVelocity + 0.5 * (previousAcceleration + currentAcceleration) * dt;
    }

    return projectedPositions;
}

const std::deque<Vector2D>& Body::GetPositionHistory() const
{
    return m_PositionHistory;
}

size_t Body::GetMaximumPosHistorySize() const
{
    return m_MaximumPosHistorySize;
}

void Body::SetMaximumPosHistorySize(size_t size)
{
    m_MaximumPosHistorySize = size;
    // If the new size is smaller than previously, remove excess position history
    while (m_PositionHistory.size() > m_MaximumPosHistorySize)
    {
        m_PositionHistory.pop_front();
    }
}

bool Body::GetShouldDrawPosHistory() const
{
    return m_ShouldDrawPosHistory;
}

void Body::SetShouldDrawPosHistory(bool shouldDraw)
{
    m_ShouldDrawPosHistory = shouldDraw;
}

int Body::GetMinimumPrevPosDist() const
{
    return m_MinimumPreviousPosDist;
}

void Body::SetMinimumPrevPosDist(int minDist)
{
    if (minDist > 0)
    {
        m_MinimumPreviousPosDist = minDist;
    }
    else
    {
        m_MinimumPreviousPosDist = 1;
    }
}

SDL_Color Body::GetColor() const
{
    return m_Color;
}

void Body::SetColor(SDL_Color color)
{
    m_Color = color;
}

SDL_Color Body::GetHoveredColor() const
{
    return m_HoveredColor;
}

void Body::SetHoveredColor(SDL_Color color)
{
    m_HoveredColor = color;
}

bool Body::GetIsHovered() const
{
    return m_IsHovered;
}

void Body::SetIsHovered(bool isHovered)
{
    m_IsHovered = isHovered;
}