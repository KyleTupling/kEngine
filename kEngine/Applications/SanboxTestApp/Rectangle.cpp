#include "Rectangle.h"
#include "math.h"
#include "Physics.h"

Rectangle::Rectangle(const Vector2D& position) : m_Position(position)
{
	m_Vertices[0] = Vector2D(-m_Width / 2, -m_Height / 2);
	m_Vertices[1] = Vector2D(m_Width / 2, -m_Height / 2);
	m_Vertices[2] = Vector2D(m_Width / 2, m_Height / 2);
	m_Vertices[3] = Vector2D(-m_Width / 2, m_Height);

	m_Velocity = Vector2D(0, 0);
	m_CurrentForce = Vector2D(0, 0);
	m_CurrentTorque = 0;
}

void Rectangle::Update(double deltaTime)
{
	m_Velocity = m_Velocity + (m_CurrentForce * m_InverseMass) * deltaTime;
	m_Position = m_Position + m_Velocity * deltaTime;

	float momentOfInertia = (1.0f / 12.0f) * m_Mass * (m_Width * m_Width + m_Height * m_Height);
	float angularAcceleration = m_CurrentTorque / momentOfInertia;

	m_AngularVelocity += angularAcceleration * deltaTime;
	m_Angle = m_Angle + m_AngularVelocity * deltaTime;

	// Resolve angle to stay between -2PI and 2PI
	if (m_Angle > Physics::TWO_PI)
	{
		m_Angle -= Physics::TWO_PI;
	}
	if (m_Angle < -Physics::TWO_PI)
	{
		m_Angle += Physics::TWO_PI;
	}

	m_CurrentForce = Vector2D(0, 0);
	m_CurrentTorque = 0;
}

void Rectangle::Draw(const Renderer& renderer) const
{
	float cosAngle = cos(m_Angle);
	float sinAngle = sin(m_Angle);

	Vector2D topLeftCornerPos(
		m_Position.x + (-(m_Width / 2) * cosAngle + (m_Height / 2) * sinAngle),
		m_Position.y + (-(m_Width / 2) * sinAngle - (m_Height / 2) * cosAngle)
	);

	// Dot implementation
	/*Vector2D topLeftCornerPos(
		m_Position.x + Vector2D::Dot(m_Vertices[0], Vector2D(cosAngle, -sinAngle)),
		m_Position.y + Vector2D::Dot(m_Vertices[0], Vector2D(sinAngle, cosAngle))
	);*/

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

bool Rectangle::CheckCollisionSAT(const Rectangle& rectA, const Rectangle& rectB)
{
	// Get vertices of both Rectangles
	std::array<Vector2D, 4> verticesA = rectA.GetTransformedVertices();
	std::array<Vector2D, 4> verticesB = rectB.GetTransformedVertices();

	// Array for all axes (4 from each edge of each Rectangle, 8 total)
	Vector2D axes[8];

	// Compute edge normals (rectA)
	for (size_t i = 0; i < 4; i++)
	{
		Vector2D edge = verticesA[(i + 1) % 4] - verticesA[i];
		axes[i] = Vector2D(-edge.y, edge.x).GetUnitVector();
	}

	// Compute edge normals (rectB)
	for (size_t i = 0; i < 4; i++)
	{
		Vector2D edge = verticesB[(i + 1) % 4] - verticesB[i];
		axes[i + 4] = Vector2D(-edge.y, edge.x).GetUnitVector();
	}

	// Loop through all axes to check separation
	for (size_t i = 0; i < 8; i++)
	{
		float minA = Vector2D::Dot(verticesA[0], axes[i]);
		float maxA = minA;
		float minB = Vector2D::Dot(verticesB[0], axes[i]);
		float maxB = minB;

		minA = maxA = Vector2D::Dot(verticesA[0], axes[i]);
		for (size_t j = 1; j < 4; j++)
		{
			float projection = Vector2D::Dot(verticesA[j], axes[i]);
			if (projection < minA) minA = projection;
			if (projection > maxA) maxA = projection;
		}

		minB = maxB = Vector2D::Dot(verticesB[0], axes[i]);
		for (size_t j = 1; j < 4; j++)
		{
			float projection = Vector2D::Dot(verticesB[j], axes[i]);
			if (projection < minB) minB = projection;
			if (projection > maxB) maxB = projection;
		}

		// Check for overlap between projections
		if (maxA < minB || maxB < minA)
		{
			// If there's no overlap on the axis, there's no collision
			return false;
		}
	}

	return true;
}

void Rectangle::ApplyForce(const Vector2D& force, const Vector2D& point)
{
	// Calculate vector from center of mass to point at which force was applied
	Vector2D r = point - m_Position;

	// Add linear force to force accumulator
	m_CurrentForce = m_CurrentForce + force;

	// Use 2D cross product to calculate torque
	float torqueContribution = r.x * force.y - r.y * force.x;

	m_CurrentTorque += torqueContribution;
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

const Vector2D* Rectangle::GetVertices() const
{
	return m_Vertices;
}

std::array<Vector2D, 4> Rectangle::GetTransformedVertices() const
{
	std::array<Vector2D, 4> transformedVertices;

	float cosAngle = cos(m_Angle);
	float sinAngle = sin(m_Angle);

	transformedVertices[0] = Vector2D(
		m_Position.x + (-(m_Width / 2) * cosAngle + (m_Height / 2) * sinAngle),
		m_Position.y + (-(m_Width / 2) * sinAngle - (m_Height / 2) * cosAngle)
	);

	transformedVertices[1] = Vector2D(
		m_Position.x + ((m_Width / 2) * cosAngle + (m_Height / 2) * sinAngle),
		m_Position.y + ((m_Width / 2) * sinAngle - (m_Height / 2) * cosAngle)
	);

	transformedVertices[2] = Vector2D(
		m_Position.x + (-(m_Width / 2) * cosAngle - (m_Height / 2) * sinAngle),
		m_Position.y + (-(m_Width / 2) * sinAngle + (m_Height / 2) * cosAngle)
	);

	transformedVertices[3] = Vector2D(
		m_Position.x + ((m_Width / 2) * cosAngle - (m_Height / 2) * sinAngle),
		m_Position.y + ((m_Width / 2) * sinAngle + (m_Height / 2) * cosAngle)
	);

	return transformedVertices;
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
	m_InverseMass = 1 / m_Mass;
}

float Rectangle::GetInverseMass() const
{
	return m_InverseMass;
}

const SDL_Color& Rectangle::GetColor() const
{
	return m_Color;
}

const Vector2D& Rectangle::GetCurrentForce() const
{
	return m_CurrentForce;
}

void Rectangle::SetColor(const SDL_Color& color)
{
	m_Color = color;
}