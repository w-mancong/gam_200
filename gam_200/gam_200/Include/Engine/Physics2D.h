#include "pch.h"

#ifndef	PHYSICS2D_H
#define PHYSICS2D_H
namespace ManCong
{
	namespace Engine
	{
		namespace Physics
		{
			//Representation of ray or a line
			class Ray2D {
			public:
				Math::Vector2 origin{};
				Math::Vector2 end{};
			};

			//To encapsulate output of data for raycasting
			class RaycastHit2D {
			public:
				Math::Vector2 point{};
				Math::Vector2 normal{};
				bool isCollided{false};
			};

			//Raycast functions
			RaycastHit2D Raycast_AABB(Ray2D const& ray, Collider2D const& collider, Transform const& parent_transform_collider);
			RaycastHit2D Raycast_Circle(Ray2D const& ray, Collider2D const& collider, Transform const& parent_transform_collider);

			//To encapsulate output of data for a collision as well as it's response
			class Collision2D {
			public:
				bool isCollided;
				bool isCollisionResponseEnabled = false;
			};
		}
	}
}
#endif
