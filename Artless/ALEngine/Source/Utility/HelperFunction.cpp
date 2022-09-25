#include "pch.h"

namespace ALEngine::Utility
{
	bool IsEqual(f32 a, f32 b)
	{
		const f32 abs_a = fabs(a), abs_b = fabs(b);
		return abs_a - abs_b <= (abs_a > abs_b ? abs_b : abs_a) * FLT_EPSILON;
	}
}