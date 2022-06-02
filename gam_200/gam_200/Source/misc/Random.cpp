#include "pch.h"

namespace ManCong
{
	namespace Random
	{
		s32 Range(s32 min, s32 max)
		{
			std::random_device rd; std::mt19937 mt(rd()); std::uniform_int_distribution<s32> dis(min, max);
			return dis(mt);
		}
		f32 Range(f32 min, f32 max)
		{
			std::random_device rd; std::mt19937 mt(rd()); std::uniform_real_distribution<f32> dist(min, max);
			return dist(mt);
		}
	}
}