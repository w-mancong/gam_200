/*!
file:	Collider.h
author:	Tan Zhen Xiong
email:	t.zhenxiong@digipen.edu
brief:	This file contains the Collider component

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#ifndef	RIGIDBODY2D_H
#define RIGIDBODY2D_H

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
			struct Rigidbody2D
			{
				Entity m_Entity{};
				
				bool hasGravity{ false };
				bool isActive{ true };
			};
		}
	}
}
#endif