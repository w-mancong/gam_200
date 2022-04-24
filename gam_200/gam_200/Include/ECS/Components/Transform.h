#ifndef	TRANSFORM_H
#define TRANSFORM_H

namespace ManCong
{
	namespace ECS
	{
		namespace Component
		{
			struct Transform
			{
				Math::Vector2 position{ 0.0f, 0.0f };
				Math::Vector2 scale{ 1.0f, 1.0f };
				f32	rotation{ 0.0f };
			};
		}
	}
}

#endif