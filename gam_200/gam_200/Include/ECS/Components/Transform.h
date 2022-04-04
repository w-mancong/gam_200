#ifndef	TRANSFORM_H
#define TRANSFORM_H

#include "Math/Vector2.h"
namespace ManCong
{
	namespace ECS
	{
		using namespace Math;
		struct Transform
		{
			Vector2 position;
			Vector2 scale;
			float	rotation;
		};
	}
}

#endif