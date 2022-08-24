#include "pch.h"

#ifndef	PHYSICS2D_H
#define PHYSICS2D_H
namespace ManCong
{
	namespace Engine
	{
		namespace Physics
		{
			class Ray2D {
			public:
				Math::Vector2 origin{};
				Math::Vector2 end{};
			};

			class RaycastHit2D {
			public:
				Math::Vector2 point{};
				Math::Vector2 normal{};
				bool isCollided{};
			};

			RaycastHit2D Raycast_AABB(Ray2D const& ray, Collider2D const& collider, Transform const& parent_transform_collider);
			RaycastHit2D Raycast_Circle(Ray2D const& ray, Collider2D const& collider, Transform const& parent_transform_collider);
		}
	}
}
#endif
