#include "PhysicsObject.h"
#include "Physics.h"

PhysicsObject::PhysicsObject(PhysicsType type, const Vector2D& position)
	: m_Type(type), m_Position(position)
{
	if (m_Type == PhysicsType::Static)
	{
		m_InverseMass = 0;
	}
}

void PhysicsObject::Update(double deltaTime)
{
	m_Velocity = m_Velocity + (m_CurrentForce * m_InverseMass) * deltaTime;
	m_Position = m_Position + m_Velocity * deltaTime;

	float angularAcceleration = m_CurrentTorque / GetMomentOfInertia();

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

void PhysicsObject::ApplyForce(const Vector2D& force, const Vector2D& point)
{
	// Calculate vector from center of mass to point at which force was applied
	Vector2D r = point - m_Position;

	// Add linear force to force accumulator
	m_CurrentForce = m_CurrentForce + force;

	// Use 2D cross product to calculate torque
	float torqueContribution = r.x * force.y - r.y * force.x;

	m_CurrentTorque += torqueContribution;
}

const Vector2D& PhysicsObject::GetPosition() const
{
	return m_Position;
}

void PhysicsObject::SetPosition(const Vector2D& position)
{
	m_Position = position;
}

const Vector2D& PhysicsObject::GetVelocity() const
{
	return m_Velocity;
}

void PhysicsObject::SetVelocity(const Vector2D& velocity)
{
	m_Velocity = velocity;
}

float PhysicsObject::GetMass() const
{
	if (m_Type == PhysicsType::Static)
	{
		throw std::runtime_error("Static physics objects have infinite mass.");
	}
	return m_Mass;
}

void PhysicsObject::SetMass(float mass)
{
	if (m_Type == PhysicsType::Static)
	{
		throw std::runtime_error("Cannot set mass for a static physics object.");
	}
	m_Mass = mass;
	m_InverseMass = (mass != 0) ? 1.0f / mass : 0.0f;
}

float PhysicsObject::GetInverseMass() const
{
	return m_InverseMass;
}

PhysicsType PhysicsObject::GetType() const
{
	return m_Type;
}