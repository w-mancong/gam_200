#ifndef	MATH_H
#define MATH_H

#include "Math/inverse_sqrt.h"
#include "Math/Vector2.h"
#include "Math/Vector3.h"
#include "Math/Vector4.h"
#include "Math/Matrix3x3.h"
#include "Math/Matrix4x4.h"

namespace ManCong
{
	namespace Math
	{
		s32	Clamp(s32 value, s32 min, s32 max);
		f32	Clamp(f32 value, f32 min, f32 max);
		f64	Clamp(f64 value, f64 min, f64 max);
		f32	DegreeToRadian(f32 angle);
		f32	RadianToDegree(f32 angle);
	}
}

#endif