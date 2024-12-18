#pragma once

#include "Vector2D.h"
#include "Renderer.h"
#include <array>

class Rectangle
{
public:
	Rectangle(const Vector2D& position);

	void Update(double deltaTime);
	void Draw(const Renderer& renderer) const;

	static bool CheckCollisionSAT(const Rectangle& rectA, const Rectangle& rectB);

	/**
	 * Applies a force to the rectangle at a given point.
	 * A linear force is applied as well as a torque contribution.
	 *
	 * @param force The force vector
	 * @param point The point in space at which the force is applied
	 */
	void ApplyForce(const Vector2D& force, const Vector2D& point);

	const Vector2D& GetPosition() const;
	void SetPosition(const Vector2D& position);

	const Vector2D& GetVelocity() const;
	void SetVelocity(const Vector2D& velocity);

	const Vector2D& GetPreviousVelocity() const;

	float GetWidth() const;
	void SetWidth(float width);

	float GetHeight() const;
	void SetHeight(float height);

	const Vector2D* GetVertices() const;
	std::array<Vector2D, 4> GetTransformedVertices() const;

	float GetAngle() const;
	void SetAngle(float angle);

	float GetAngularVelocity() const;
	void SetAngularVelocity(float angularVelocity);

	float GetMass() const;
	void SetMass(float mass);

	float GetInverseMass() const;

	const Vector2D& GetCurrentForce() const;

	const SDL_Color& GetColor() const;
	void SetColor(const SDL_Color& color);

private:
	Vector2D m_Position;
	Vector2D m_Velocity;
	Vector2D m_PreviousVelocity;

	float m_Width = 200;
	float m_Height = 100;

	Vector2D m_Vertices[4];

	float m_Angle = 0;
	float m_AngularVelocity = 0;

	float m_Mass = 10;
	float m_InverseMass = 1 / m_Mass;

	Vector2D m_CurrentForce;
	float m_CurrentTorque;

	SDL_Color m_Color = { 255, 255, 255, 255 };
};

