#include "pch.h"

namespace ManCong
{
	namespace Utility
	{
		bool IsEqual(float a, float b)
		{
			const float abs_a = fabs(a), abs_b = fabs(b);
			return abs_a - abs_b <= (abs_a > abs_b ? abs_b : abs_a) * FLT_EPSILON;
		}
	}
}