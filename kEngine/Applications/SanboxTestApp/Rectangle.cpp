#include "Rectangle.h"
#include "math.h"

Rectangle::Rectangle(const Vector2D& position) : m_Position(position) {}

void Rectangle::Update(double deltaTime)
{
	m_Position = m_Position + m_Velocity * deltaTime;

	m_Angle = m_Angle + m_AngularVelocity * deltaTime;
}

void Rectangle::Draw(const Renderer& renderer) const
{
	float cosAngle = cos(m_Angle);
	float sinAngle = sin(m_Angle);

	Vector2D topLeftCornerPos(
		m_Position.x + (-(m_Width / 2) * cosAngle + (m_Height / 2) * sinAngle),
		m_Position.y + (-(m_Width / 2) * sinAngle - (m_Height / 2) * cosAngle)
	);

	Vector2D topRightCornerPos(
		m_Position.x + ((m_Width / 2) * cosAngle + (m_Height / 2) * sinAngle),
		m_Position.y + ((m_Width / 2) * sinAngle - (m_Height / 2) * cosAngle)
	);

	Vector2D bottomLeftCornerPos(
		m_Position.x + (-(m_Width / 2) * cosAngle - (m_Height / 2) * sinAngle),
		m_Position.y + (-(m_Width / 2) * sinAngle + (m_Height / 2) * cosAngle)
	);

	Vector2D bottomRightCornerPos(
		m_Position.x + ((m_Width / 2) * cosAngle - (m_Height / 2) * sinAngle),
		m_Position.y + ((m_Width / 2) * sinAngle + (m_Height / 2) * cosAngle)
	);

	/*renderer.DrawLineOnScreen(topLeftCornerPos, topRightCornerPos, m_Color);
	renderer.DrawLineOnScreen(topLeftCornerPos, bottomLeftCornerPos, m_Color);
	renderer.DrawLineOnScreen(topRightCornerPos, bottomRightCornerPos, m_Color);
	renderer.DrawLineOnScreen(bottomLeftCornerPos, bottomRightCornerPos, m_Color);*/
	renderer.DrawLineInWorld(topLeftCornerPos, topRightCornerPos, m_Color);
	renderer.DrawLineInWorld(topLeftCornerPos, bottomLeftCornerPos, m_Color);
	renderer.DrawLineInWorld(topRightCornerPos, bottomRightCornerPos, m_Color);
	renderer.DrawLineInWorld(bottomLeftCornerPos, bottomRightCornerPos, m_Color);
}

const Vector2D& Rectangle::GetPosition() const
{
	return m_Position;
}

void Rectangle::SetPosition(const Vector2D& position)
{
	m_Position = position;
}

const Vector2D& Rectangle::GetVelocity() const
{
	return m_Velocity;
}

void Rectangle::SetVelocity(const Vector2D& velocity)
{
	m_Velocity = velocity;
}

float Rectangle::GetWidth() const
{
	return m_Width;
}

void Rectangle::SetWidth(float width)
{
	m_Width = width;
}

float Rectangle::GetHeight() const
{
	return m_Height;
}

void Rectangle::SetHeight(float height)
{
	m_Height = height;
}

float Rectangle::GetAngle() const
{
	return m_Angle;
}

void Rectangle::SetAngle(float angle)
{
	m_Angle = angle;
}

float Rectangle::GetAngularVelocity() const
{
	return m_AngularVelocity;
}

void Rectangle::SetAngularVelocity(float angularVelocity)
{
	m_AngularVelocity = angularVelocity;
}

float Rectangle::GetMass() const
{
	return m_Mass;
}

void Rectangle::SetMass(float mass)
{
	m_Mass = mass;
}

const SDL_Color& Rectangle::GetColor() const
{
	return m_Color;
}

void Rectangle::SetColor(const SDL_Color& color)
{
	m_Color = color;
}