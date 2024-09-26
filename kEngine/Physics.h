#pragma once

#include "Vector2D.h"
#include <string>

namespace Physics
{
	constexpr double G = 6.67e-11; // Universal gravitational constant
	constexpr float EarthGravity = 9.81f;

	constexpr float PI = 3.1415926535f;
	constexpr float TWO_PI = 2 * PI;

	struct Force
	{
		Vector2D vector;
		std::string name;

		Force(const Vector2D& forceVector, const std::string& forceName = "")
			: vector(forceVector), name(forceName) {}
	};
}