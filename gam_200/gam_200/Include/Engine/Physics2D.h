#include "pch.h"

#ifndef	PHYSICS2D_H
#define PHYSICS2D_H
namespace ALEngine
{
	namespace Engine
	{
		namespace Physics
		{
			//Representation of ray or a line
			struct Ray2D {
				Math::Vector2 origin{};
				Math::Vector2 end{};
			};

			//To encapsulate output of data for raycasting
			struct RaycastHit2D {
				Math::Vector2 point{};
				Math::Vector2 normal{};
				bool isCollided{false};
			};

			//Raycast functions
			/*!*********************************************************************************
				\brief
				Raycast line to 2D Axis Aligned Bounding Box
			***********************************************************************************/
			RaycastHit2D Raycast_AABB(Ray2D const& ray, Collider2D const& collider, Transform const& parent_transform_collider);

			/*!*********************************************************************************
				\brief
				Raycast line to 2D circle
			***********************************************************************************/
			RaycastHit2D Raycast_Circle(Ray2D const& ray, Collider2D const& collider, Transform const& parent_transform_collider);

			//To encapsulate output of data for a collision as well as it's response
			struct Collision2D {
				bool isCollided;
				bool isCollisionResponseEnabled = false;
			};
		}
	}
}
#endif
