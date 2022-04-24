#ifndef	TRANSFORM_H
#define TRANSFORM_H

#include "Math/Vector2.h"
namespace ManCong
{
	namespace ECS
	{
		namespace Component
		{
			using namespace Math;
			struct Transform
			{
				Vector2 position;
				Vector2 scale;
				f32	rotation;
			};
		}
	}
}

#endif