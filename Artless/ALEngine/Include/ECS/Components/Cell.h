/*!
file:	Unit.h
author:	Tan Zhen Xiong
email:	t.zhenxiong@digipen.edu
brief:	This file contains the function declarations for Collider.h

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#ifndef	CELL_H
#define CELL_H

namespace ALEngine::ECS::Component
{
	/*!*********************************************************************************
		\brief
			2D Circle Collider for collision detection
	***********************************************************************************/
	struct Cell {
		u32 unitEntity;
		uint32_t coordinate[2];
	};
}
#endif