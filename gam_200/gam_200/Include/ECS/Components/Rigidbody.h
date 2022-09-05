/*!
file:	Rigidbody.h
author:	Tan Zhen Xiong
email:	t.zhenxiong@digipen.edu
brief:	This file contains the function declarations for Rigidbody.h

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/

#ifndef	RIGIDBODY_H
#define RIGIDBODY_H
namespace ManCong
{
	namespace ECS
	{
		namespace Component
		{
			/*!*********************************************************************************
				\brief
					2D Circle Collider for collision detection
			***********************************************************************************/
			struct Rigidbody2D
			{
			public:
				Math::Vector2 velocity{ 0.f, 0.f };
				bool hasGravity{ true };
				bool isEnabled{ false };
			};
		}
	}
}
#endif