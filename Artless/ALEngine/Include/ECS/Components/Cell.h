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

		float m_HCost{ 0 }, m_GCost{ 10000 }, m_FCost{ 0 };
		bool m_isAccesible{ true };
		Cell *m_ParentCell;

		u32 unitEntity;
		uint32_t coordinate[2];

		void CalculateFCost()
		{
			m_FCost = m_GCost + m_HCost;
		}
	};
}
#endif