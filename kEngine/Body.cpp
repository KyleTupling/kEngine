#include "Body.h"

Body::Body() : position(0, 0), velocity(0, 0), acceleration(0, 0) {}

Body::Body(const Vector2D& pos, const Vector2D& vel) : position(pos), velocity(vel), acceleration(0, 0) {}

// Using inline to optimise function calls every frame
inline void Body::ApplyForce(const Vector2D& force)
{
	// Acceleration = Force / Mass
	Vector2D acc = force.Divide(mass);
	acceleration = acceleration + acc;
}

void Body::AttractBody(Body& body)
{
    Vector2D dir = Vector2D::Subtract(this->position, body.position);
    double dirMag = dir.GetMagnitude();
    if (dirMag != 0) // Prevent division by zero
    {
        Vector2D unitDir = dir / dirMag;
        double forceMag = (PhysicsConstants::G * this->mass * body.mass) / (dirMag * dirMag);
        Vector2D force = unitDir.Multiply(forceMag);
        body.ApplyForce(force);
    }   
}

void Body::UpdatePreviousPositions()
{
    // If previous position list is empty, add current position
    if (this->previousPositions.size() == 0 || abs((this->position - this->previousPositions.back()).GetMagnitudeSqr()) > this->minimumPrevDist)
    {
        this->previousPositions.push_back(this->position);
    }

    // Ensure previous position list doesn't store more than maximum amount of positions
    if (this->previousPositions.size() > this->maximumPrevPos)
    {
        this->previousPositions.pop_front();
    }
}

void Body::UpdateKinematics(double deltaTime)
{
    this->velocity = this->velocity + this->acceleration * deltaTime;
    this->position = this->position + this->velocity * deltaTime;
    this->acceleration.Set(0, 0);
}

void Body::Update(double deltaTime)
{
    UpdatePreviousPositions();
    UpdateKinematics(deltaTime);
}

void Body::Draw(SDL_Renderer* renderer, const Camera& camera, const Vector2D& screenSize) const
{
    Vector2D screenPos = camera.ConvertWorldToScreen(position, screenSize);

    // Check whether to draw previous positions
    if (this->drawPrevious)
    {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 100);
        for (const auto& prevPos : this->previousPositions)
        {
            constexpr int PREVPOS_RECT_SIZE = 2;
            constexpr int PREVPOS_RECT_OFFSET = 1;
            Vector2D prevScreenPos = camera.ConvertWorldToScreen(prevPos, screenSize);
            SDL_Rect rect{ static_cast<int>(std::round(prevScreenPos.x)) - PREVPOS_RECT_OFFSET, static_cast<int>(std::round(prevScreenPos.y)) - PREVPOS_RECT_OFFSET, PREVPOS_RECT_SIZE, PREVPOS_RECT_SIZE }; // TODO: Fix double->int conversion
            SDL_RenderFillRect(renderer, &rect);
        }
    }

    if (isHovered)
        SDL_SetRenderDrawColor(renderer, hoveredColor.r, hoveredColor.g, hoveredColor.b, hoveredColor.a);
    else
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

    SDL_Rect bodyRect = {
        static_cast<int>(screenPos.x - radius * camera.zoom),
        static_cast<int>(screenPos.y - radius * camera.zoom),
        static_cast<int>(radius * 2 * camera.zoom),
        static_cast<int>(radius * 2 * camera.zoom)
    };

    SDL_RenderFillRect(renderer, &bodyRect);
}

void Body::CheckHover(const Vector2D& mousePos, const Camera& camera, const Vector2D& screenSize)
{
    Vector2D screenPos = camera.ConvertWorldToScreen(position, screenSize);

    double distSqr = (mousePos - screenPos).GetMagnitudeSqr();
    isHovered = distSqr <= (radius * radius * camera.zoom * camera.zoom);
}