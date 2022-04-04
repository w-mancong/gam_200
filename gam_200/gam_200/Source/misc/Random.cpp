#include "pch.h"

namespace ManCong
{
	namespace Random
	{
		int Range(int min, int max)
		{
			return (std::rand() % (max - min + 1)) + min;
		}
		float Range(float min, float max)
		{
			return ((max - min) * ((float)std::rand() / RAND_MAX)) + min;
		}
	}
}