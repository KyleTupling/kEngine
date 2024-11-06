#pragma once

#include "PhysicsObject.h"
#include "Vector2D.h"

#include <memory>
#include <vector>

typedef std::vector<std::shared_ptr<PhysicsObject>> PhysicsObjectVector;

class PhysicsObjectManager
{
public:
	PhysicsObjectManager();

	/**
	 * Updates the vector of PhysicsObjects with objects in deferred vector.
	 */
	void Update();

	std::shared_ptr<PhysicsObject> AddPhysicsObject(PhysicsType type, const Vector2D& position);
private:
	PhysicsObjectVector m_PhysicsObjects;
	PhysicsObjectVector m_PhysicsObjectsToAdd;
	size_t m_TotalPhysicsObjects;
};

