
/*!
file:	Collider.h
author:	Tan Zhen Xiong
email:	t.zhenxiong@digipen.edu
brief:	This file contains the Collider component

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#ifndef	COLLIDER_H
#define COLLIDER_H

namespace ManCong
{
	namespace ECS
	{
		namespace Component
		{
			/*!*********************************************************************************
				\brief
					Basic Collider component will be used to handle collision
			***********************************************************************************/
			struct Collider
			{
				Entity m_Entity;
				Math::Vector2 position{ 0.0f, 0.0f };	//Collider position that is relative to it's entity
				bool isActive{ true };					//Is active
				bool isCollided{ false };				//Is Colliding
				f32	rotation{ 0.0f };					//Rotation of the collider relative to it's entity
			};

			/*!*********************************************************************************
				\brief
					2D Box Collider component will be used to handle collision
			***********************************************************************************/
			struct BoxCollder : Collider {
				f32 width;
				f32 height;
			};

			/*!*********************************************************************************
				\brief
					2D Circle Collider component will be used to handle collision
			***********************************************************************************/
			struct CircleCollider : Collider {
				f32 radius;
			};
		}
	}
}
#endif