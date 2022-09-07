/*!
file:	CharacterController.h
author:	Tan Zhen Xiong
email:	t.zhenxiong@digipen.edu
brief:	This file contains the function declarations for CharacterController.h

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/

#ifndef	CHARACTERCONTROLLER_H
#define CHARACTERCONTROLLER_H
namespace ALEngine
{
	namespace ECS
	{
		namespace Component
		{
			/*!*********************************************************************************
				\brief
					Data for the character controller
			***********************************************************************************/
			struct CharacterController
			{
			public:
				f32 speed;
				f32 jumpStrength;

				u64 leftKey, rightKey, jumpKey, rotateLeftKey, rotateRightKey;
			};
		}
	}
}
#endif