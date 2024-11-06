#pragma once

#include "Vector2D.h"
#include "Renderer.h"

enum class PhysicsType
{
	Static,
	RigidBody
};

class PhysicsObject
{
public:
	PhysicsObject(PhysicsType type, const Vector2D& position);
	virtual ~PhysicsObject() = default;

	virtual void Update(double deltaTime);
	virtual void Draw(const Renderer& renderer) {};

	void ApplyForce(const Vector2D& force, const Vector2D& point);

	bool ShouldBeRemoved() const;

	const Vector2D& GetPosition() const;
	void SetPosition(const Vector2D& position);

	const Vector2D& GetVelocity() const;
	void SetVelocity(const Vector2D& velocity);

	float GetMass() const;
	void SetMass(float mass);

	float GetInverseMass() const;

	virtual float GetMomentOfInertia() { return 1.0f; };

	PhysicsType GetType() const;
protected:
	PhysicsType m_Type;

	// If set to true, PhysicsObject will be removed on next update
	bool m_ShouldBeRemoved = false;

	Vector2D m_Position;
	Vector2D m_Velocity;

	float m_Angle = 0;
	float m_AngularVelocity = 0.0f;

	float m_Mass = 1.0f;
	float m_InverseMass = 1.0f;

	Vector2D m_CurrentForce{ 0, 0 };
	float m_CurrentTorque = 0.0f;
};

