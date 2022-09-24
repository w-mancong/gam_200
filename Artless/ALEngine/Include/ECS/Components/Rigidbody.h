/*!
file:	Rigidbody.h
author:	Tan Zhen Xiong
email:	t.zhenxiong@digipen.edu
brief:	This file contains the function declarations for Rigidbody.h

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/

#ifndef	RIGIDBODY_H
#define RIGIDBODY_H
namespace ALEngine
{
	namespace ECS
	{
		namespace Component
		{
			/*!*********************************************************************************
				\brief
					Data for the Rigidbody2D
					This component wil contain data needed for physics to be calculated for rigidbody
			***********************************************************************************/
			struct Rigidbody2D
			{
			public:
				//Keep track of gameobject's next position
				Math::Vector2 nextPosition{ 0.f, 0.f };

				//Drag, 0 for no friction, 1 for normal
				Math::Vector2 velocity{ 0.f, 0.f }, frameVelocity{ 0.f,0.f }, acceleration{ 0.f,0.f }, drag{ 1.f,1.f };
				
				f32 mass{ 1.f };			//Mass
				bool hasGravity{ true };	//Is Gravity Enabled
				bool isEnabled{ false };	//Is the rigidbody enabled
			};
		}
	}
}
#endif