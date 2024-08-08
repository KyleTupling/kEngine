#pragma once

namespace MathFunctions
{
	template <typename T>
	T clamp(const T& num, const T& lower, const T& upper)
	{
		return std::max(lower, std::min(num, upper));
	}
}