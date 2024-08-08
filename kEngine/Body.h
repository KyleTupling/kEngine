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

    Uint8 colour[4] = { 0, 0, 0, 255 };

    bool isHovered = false;

    Body();
    Body(Vector2D pos, Vector2D vel);

    // Applies a force to a body instance based on Newton's 2nd law of motion
    void ApplyForce(Vector2D force);

    // Attract body instance using Newton's Law of Gravitation
    void AttractBody(Body& body);

    // Updates logic and physics of body instance
    // Takes parameter [double]deltaTime for equations of motion
    void Update(double deltaTime);

    // Draws the body instance to a window using the window's assigned renderer
    // Takes parameter [SDL_Renderer*]renderer
    void Draw(SDL_Renderer* renderer, const Camera& camera, const Vector2D& screenSize);

    // Checks whether given mouse coordinates are inside body
    void CheckHover(const Vector2D& mousePos, const Camera& camera, const Vector2D& screenSize);
};

