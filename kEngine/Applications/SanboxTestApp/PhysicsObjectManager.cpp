#include "PhysicsObjectManager.h"

PhysicsObjectManager::PhysicsObjectManager()
{
}

void PhysicsObjectManager::Update()
{
	for (auto obj : m_PhysicsObjectsToAdd)
	{
		m_PhysicsObjects.push_back(obj);
	}
	for (auto obj : m_PhysicsObjects)
	{
		if (obj->ShouldBeRemoved())
		{
			// Remove PhysicsObject from vector
		}
	}
	// Clear object-add buffer
	m_PhysicsObjectsToAdd.clear();
}

std::shared_ptr<PhysicsObject> PhysicsObjectManager::AddPhysicsObject(PhysicsType type, const Vector2D& position)
{
	auto object = std::make_shared<PhysicsObject>(type, position);
	m_PhysicsObjectsToAdd.push_back(object);
	return object;
}
