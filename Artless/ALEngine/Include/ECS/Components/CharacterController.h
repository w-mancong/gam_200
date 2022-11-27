/*!
file:	CharacterController.h
author:	Tan Zhen Xiong
email:	t.zhenxiong\@digipen.edu
brief:	This file contains the function declarations for CharacterController.h

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/

#ifndef	CHARACTERCONTROLLER_H
#define CHARACTERCONTROLLER_H
namespace ALEngine::ECS::Component
{
	/*!*********************************************************************************
		\brief
			Data for the character controller
			This component will act as a quick interface for implementing 2D platformer controls
	***********************************************************************************/
	struct CharacterController
	{
	public:
		f32 speed;			//Acceleration of object
		f32 jumpStrength;	//Velocity of jump

		//Interface for custom keys for control
		u64 leftKey, rightKey, jumpKey, rotateLeftKey, rotateRightKey;
	};
}
#endif