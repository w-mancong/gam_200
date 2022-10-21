/*!
file:	Physics2D.h
author:	Tan Zhen Xiong
email:	t.zhenxiong@digipen.edu
brief:	This file contains the function declaration for Physics2D.h
		It contains declaration of structs for Ray2D, Raycasthit2D.
		It also contains functions needed for raycasting

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#ifndef	PHYSICS2D_H
#define PHYSICS2D_H
namespace ALEngine::Engine::Physics
{
	//Representation of ray or a line
	struct Ray2D {
		Math::Vector2 origin{};	//Origin of ray
		Math::Vector2 end{};	//End of ray
	};

	//To encapsulate output of data for raycasting
	struct RaycastHit2D {
		Math::Vector2 point{};	//Point of collision
		Math::Vector2 normal{};	//Normal of intersection
		bool isCollided{false};	//Holder for collision
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
}

#endif
