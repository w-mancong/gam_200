#include "pch.h"

namespace ManCong
{
	namespace Math
	{
		s32 Clamp(s32 value, s32 min, s32 max)
		{
			return std::max(min, std::min(value, max));
		}

		float Clamp(f32 value, f32 min, f32 max)
		{
			return std::max(min, std::min(value, max));
		}

		double Clamp(f64 value, f64 min, f64 max)
		{
			return std::max(min, std::min(value, max));
		}

		f32	DegreeToRadian(f32 angle)
		{
			return angle * static_cast<f32>(M_PI) / 180.0f;
		}

		f32 RadianToDegree(f32 angle)
		{
			return angle * 180.0f / static_cast<f32>(M_PI);
		}
	}
}