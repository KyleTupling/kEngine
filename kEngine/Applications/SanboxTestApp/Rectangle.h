#pragma once

#include "Vector2D.h"
#include "Renderer.h"

class Rectangle
{
public:
	Rectangle(const Vector2D& position);

	void Update(double deltaTime);
	void Draw(const Renderer& renderer) const;

	void ApplyForce(const Vector2D& force, const Vector2D& point);

	const Vector2D& GetPosition() const;
	void SetPosition(const Vector2D& position);

	const Vector2D& GetVelocity() const;
	void SetVelocity(const Vector2D& velocity);

	float GetWidth() const;
	void SetWidth(float width);

	float GetHeight() const;
	void SetHeight(float height);

	float GetAngle() const;
	void SetAngle(float angle);

	float GetAngularVelocity() const;
	void SetAngularVelocity(float angularVelocity);

	float GetMass() const;
	void SetMass(float mass);

	const Vector2D& GetCurrentForce() const;

	const SDL_Color& GetColor() const;
	void SetColor(const SDL_Color& color);

private:
	Vector2D m_Position;
	Vector2D m_Velocity;

	float m_Width = 200;
	float m_Height = 100;

	Vector2D m_Vertices[4];

	float m_Angle = 0;
	float m_AngularVelocity = 0;

	float m_Mass = 10;

	Vector2D m_CurrentForce;
	float m_CurrentTorque;

	SDL_Color m_Color = { 255, 255, 255, 255 };
};

