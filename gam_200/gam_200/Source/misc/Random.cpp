#include "pch.h"

namespace ManCong
{
	namespace Random
	{
		s32 Range(s32 min, s32 max)
		{
			return (std::rand() % (max - min + 1)) + min;
		}
		f32 Range(f32 min, f32 max)
		{
			return ((max - min) * ((float)std::rand() / RAND_MAX)) + min;
		}
	}
}