#pragma once

#include <SDL.h>
#include <deque>

#include "Vector2D.h"
#include "Camera.h"
#include "PhysicsConstants.h"

class Body
{
public:
    int radius = 10;
    double mass = 10;
    bool active = true; // Tracks whether to update instance during simulation

    Vector2D position;
    Vector2D velocity;
    Vector2D acceleration;

    std::deque<Vector2D> previousPositions; // Holds previous positions up to a maximum amount
    int maximumPrevPos = 50;
    bool drawPrevious = false; // Enables drawing of previous positions trail
    int minimumPrevDist = 150; // The minimum distance required between last stored position and current position to allow storing of current position

    SDL_Color color = { 0, 0, 0, 255 };
    SDL_Color hoveredColor = { 100, 100, 100, 255 };

    bool isHovered = false;

    Body();
    Body(const Vector2D& pos, const Vector2D& vel);

    // Applies a force to a body instance based on Newton's 2nd law of motion
    inline void ApplyForce(const Vector2D& force);

    // Attract body instance using Newton's Law of Gravitation
    void AttractBody(Body& body);

    // Updates the previous positions of the body.
    void UpdatePreviousPositions();

    // Updates kinematics of body (position, velocity, acceleration)
    void UpdateKinematics(double deltaTime);

    // Updates logic and physics of body instance
    // Takes parameter [double]deltaTime for equations of motion
    void Update(double deltaTime);

    // Draws the body instance to a window using the window's assigned renderer
    // Takes parameter [SDL_Renderer*]renderer
    void Draw(SDL_Renderer* renderer, const Camera& camera, const Vector2D& screenSize) const;

    // Checks whether given mouse coordinates are inside body
    void CheckHover(const Vector2D& mousePos, const Camera& camera, const Vector2D& screenSize);
};

