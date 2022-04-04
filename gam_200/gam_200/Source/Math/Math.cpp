#include "pch.h"

namespace ManCong
{
	namespace Math
	{
		int Clamp(int value, int min, int max)
		{
			return std::max(min, std::min(value, max));
		}

		float Clamp(float value, float min, float max)
		{
			return std::max(min, std::min(value, max));
		}

		double Clamp(double value, double min, double max)
		{
			return std::max(min, std::min(value, max));
		}
	}
}