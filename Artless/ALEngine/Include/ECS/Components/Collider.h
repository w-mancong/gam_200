/*!
file:	Collider.h
author:	Tan Zhen Xiong
email:	t.zhenxiong\@digipen.edu
brief:	This file contains the function declarations for Collider.h

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#ifndef	COLLIDER_H
#define COLLIDER_H

namespace ALEngine::ECS::Component
{
	/*!*********************************************************************************
	\brief
		Enum used for differentiating the different type of Shape to be created
	***********************************************************************************/
	enum class ColliderType
	{
		Rectangle2D_AABB,
		Rectangle2D_OOBB,
		Circle2D,
		ColliderType_Final
	};

	/*!*********************************************************************************
	\brief
		Container for collisions data
	***********************************************************************************/
	struct CollisionPointData {
		Math::Vector2 point{};
		Math::Vector2 normal{};
	};

	/*!*********************************************************************************
		\brief
			2D Circle Collider for collision detection
	***********************************************************************************/
	struct Collider2D
	{
		//Collider Type, determines collision to use
		ColliderType colliderType { ColliderType::Rectangle2D_AABB };

		//Local Rotation
		f32 rotation = 0.f;
				
		//Local Scale
		f32 scale[2]{ 1.f, 1.f };

		bool isCollided{ false };	//If collider encountered collision
		bool isTrigger{ false };	//If collider is triggered (true will ignore collision response)
		bool isDebug{ false };		//If collider is debug
		bool isEnabled { true };

		//Keep track of all collision point during simulated frame
		std::vector<CollisionPointData> collisionPoints;

		std::vector<u32> collidedCollidersPtr;

		//Local position (For offsets)
		Math::Vector2 m_localPosition{ 0.f, 0.f };
				
		//Global axis, will be used for collisions that are not axis-aligned
		Math::Vector2 m_globalRight{ 1.f, 0.f }, m_globalUp{ 0.f, 1.f };
	};
}
#endif