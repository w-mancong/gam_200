/*!
file:	Unit.h
author:	Tan Zhen Xiong
email:	t.zhenxiong@digipen.edu
brief:	This file contains the function declarations for Collider.h

		All content � 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#ifndef	UNIT_H
#define UNIT_H

namespace ALEngine::ECS::Component
{
	/*!*********************************************************************************
	\brief
		Enum used for differentiating the different type of Shape to be created
	***********************************************************************************/
	enum class UNIT_TYPE
	{
		PLAYER,
		ENEMY,
	};
	
	/*!*********************************************************************************
		\brief
			2D Circle Collider for collision detection
	***********************************************************************************/
	struct Unit
	{
		int32_t health;
		UNIT_TYPE unitType;
		uint32_t coordinate[2];
	};
}
#endif