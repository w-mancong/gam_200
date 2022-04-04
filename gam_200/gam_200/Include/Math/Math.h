#ifndef	MATH_H
#define MATH_H

#include "Math/inverse_sqrt.h"
#include "Math/Vector2.h"
#include "Math/Vector3.h"
#include "Math/Matrix3x3.h"

namespace ManCong
{
	namespace Math
	{
		int		Clamp(int value, int min, int max);
		float	Clamp(float value, float min, float max);
		double	Clamp(double value, double min, double max);
	}
}

#endif