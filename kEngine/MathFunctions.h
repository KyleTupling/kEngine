#pragma once

namespace MathFunctions
{
	template <typename T>
	T clamp(const T& num, const T& lower, const T& upper)
	{
		return std::max(lower, std::min(num, upper));
	}

	template <typename T>
	T Lerp(const T& current, const T& target, const T& t)
	{
		return current * (1.0f - t) + target * t;
	}

	template <typename T>
	T ExpLerp(const T& current, const T& target, const T& t)
	{
		float decayFactor = 1.0f - std::exp(-t);
		return current + (target - current) * decayFactor;
	}
}