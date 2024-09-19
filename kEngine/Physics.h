#pragma once

#include "Vector2D.h"
#include <string>

namespace Physics
{
	const double G = 6.67e-11; // Universal gravitational constant
	const float EarthGravity = 9.81f;

	struct Force
	{
		Vector2D vector;
		std::string name;

		Force(const Vector2D& forceVector, const std::string& forceName = "")
			: vector(forceVector), name(forceName) {}
	};
}