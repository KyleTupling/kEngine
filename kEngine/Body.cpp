#include "Body.h"

Body::Body()
{
	this->position.Set(0, 0);
	this->velocity.Set(0, 0);
}

//TODO: Refactor to take by reference
Body::Body(Vector2D pos, Vector2D vel)
{
	this->position.Set(pos.x, pos.y);
	this->velocity.Set(vel.x, vel.y);
}

void Body::ApplyForce(Vector2D force)
{
	// Acceleration = Force / Mass
	Vector2D acc = force.Divide(this->mass);
	this->acceleration = this->acceleration + acc;
}

void Body::AttractBody(Body& body)
{
    Vector2D dir = Vector2D::Subtract(this->position, body.position);
    Vector2D unitDir = dir.GetUnitVector();
    double dirMag = dir.GetMagnitude();
    double forceMag = (PhysicsConstants::G * this->mass * body.mass) / (dirMag * dirMag);
    Vector2D force = unitDir.Multiply(forceMag);
    body.ApplyForce(force);
}

void Body::Update(double deltaTime)
{
    if (this->previousPositions.size() == 0) // If previous position list is empty, add current position
    {
        this->previousPositions.push_back(this->position);
    }
    else // If previous position list isn't empty, check that enough distance has been made between last position to add current
    {
        if (abs((this->position - this->previousPositions.back()).GetMagnitudeSqr()) > this->minimumPrevDist)
        {
            this->previousPositions.push_back(this->position);
        }
    }

    // Ensure previous position list doesn't store more than maximum amount of positions
    if (this->previousPositions.size() > this->maximumPrevPos)
    {
        this->previousPositions.pop_front();
    }

    this->velocity = this->velocity + this->acceleration * deltaTime;
    this->position = this->position + this->velocity * deltaTime;
    this->acceleration.Set(0, 0);
}

void Body::Draw(SDL_Renderer* renderer, const Camera& camera, const Vector2D& screenSize)
{
    Vector2D screenPos = camera.ConvertWorldToScreen(position, screenSize);

    // Check whether to draw previous positions
    if (this->drawPrevious)
    {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 100);
        for (int i = 0; i < this->previousPositions.size(); i++)
        {
            Vector2D prevScreenPos = camera.ConvertWorldToScreen(this->previousPositions[i], screenSize);
            SDL_Rect rect{ static_cast<int>(prevScreenPos.x) - 1, static_cast<int>(prevScreenPos.y) - 1, 2, 2 }; // TODO: Fix double->int conversion
            SDL_RenderFillRect(renderer, &rect);
        }
    }
    SDL_SetRenderDrawColor(renderer, this->colour[0], this->colour[1], this->colour[2], this->colour[3]);
    //SDL_Rect bodyRect{ (int)this->position.x - this->radius, (int)this->position.y - this->radius, this->radius * 2, this->radius * 2 };

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