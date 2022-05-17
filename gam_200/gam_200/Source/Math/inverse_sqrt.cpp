#include "pch.h"

namespace ManCong
{
	namespace Math
	{
		f32 Q_rsqrt(f32 number)
		{
			f32 x2 = number * 0.5f, y = number;
			f32 const threeHalfs = 1.5f;

			long i = *(long*)&y;					// Reserving the bits of float and storing it into an intergal value so that can do bit manipulation
			i = 0x5f3759df - (i >> 1);				// 0x5f3759df is some number of Mantissa and Exponent with an offset, - (i >> 1) is to make the exponent be inverse
			y = *(f32*)&i;							// Converting back to floating number
			for (u64 j = 0; j < 5; ++j)
				y *= threeHalfs - x2 * y * y;		// Newton's method	(can decide over accuracy/speed) 1% off
			return y;
		}
	}
}