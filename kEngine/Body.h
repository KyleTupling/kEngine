#pragma once

#include <SDL.h>
#include <deque>

#include "Renderer.h"
#include "Vector2D.h"
#include "Camera.h"
#include "PhysicsConstants.h"

class Body
{
public:
    Body();
    Body(const Vector2D& pos, const Vector2D& vel);

    /**
     * Applies a force to the body according to Newton's 2nd law of motion.
     *
     * @param force The force vector.
     */
    inline void ApplyForce(const Vector2D& force);

    /**
     * Attracts a target body to this body according to Newton's Law of Gravitation.
     *
     * @param body The body to attract.
     */
    void AttractBody(Body& body);

    /**
     Updates the position history of the body.
     */
    void UpdatePositionHistory();

    /**
     * Updates kinematics of body (position, velocity, acceleration).
     *
     * @param deltaTime Time passed since last frame.
     */
    void UpdateKinematics(double deltaTime);

    /**
     * Carries out all updates to body (logic, kinematic).
     *
     * @param deltaTime Time passed since last frame.
     */
    void Update(double deltaTime);

    /**
     * Draws the body to the screen.
     *
     * @param renderer The renderer to use to draw the body.
     */
    void Draw(const Renderer& renderer) const;

    /**
     * Checks whether given mouse coordinates are inside body.
     *
     * @param mousePos The position of the mouse.
     * @param camera The camera currently used for rendering.
     * @param screenSize The size of the screen.
     */
    void CheckHover(const Vector2D& mousePos, const Renderer& renderer);

    bool GetActive() const;
    void SetActive(bool isActive);

    int GetRadius() const;
    void SetRadius(int radius);

    double GetMass() const;
    void SetMass(double mass);

    const Vector2D& GetPosition() const;
    void SetPosition(const Vector2D& position);

    const Vector2D& GetVelocity() const;
    void SetVelocity(const Vector2D& velocity);

    const Vector2D& GetAcceleration() const;
    void SetAcceleration(const Vector2D& acceleration);

    const std::deque<Vector2D>& GetPositionHistory() const;

    size_t GetMaximumPosHistorySize() const;
    void SetMaximumPosHistorySize(size_t size);

    bool GetShouldDrawPosHistory() const;
    void SetShouldDrawPosHistory(bool shouldDraw);

    int GetMinimumPrevPosDist() const;
    void SetMinimumPrevPosDist(int minDist);

    SDL_Color GetColor() const;
    void SetColor(SDL_Color color);

    SDL_Color GetHoveredColor() const;
    void SetHoveredColor(SDL_Color color);

    bool GetIsHovered() const;
    void SetIsHovered(bool isHovered);

private:
    bool m_IsActive = true; // Tracks whether to update instance during simulation

    int m_Radius = 10;
    double m_Mass = 10;

    Vector2D m_Position;
    Vector2D m_Velocity;
    Vector2D m_Acceleration;

    // These names are long but I may add other history types later
    std::deque<Vector2D> m_PositionHistory; // Holds previous positions up to a maximum amount
    size_t m_MaximumPosHistorySize = 50;
    bool m_ShouldDrawPosHistory = false; // Enables drawing of previous positions trail
    int m_MinimumPreviousPosDist = 150; // The minimum distance required between last stored position and current position to allow storing of current position

    SDL_Color m_Color = { 0, 0, 0, 255 };
    SDL_Color m_HoveredColor = { 100, 100, 100, 255 };
    bool m_IsHovered = false;
};

