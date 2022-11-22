/*!
file:	cell.h
author:	Tan Zhen Xiong
email:	t.zhenxiong@digipen.edu
brief:	This file contains the function declarations for cell.h

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#ifndef	CELL_H
#define CELL_H

namespace ALEngine::ECS::Component
{
	/*!*********************************************************************************
		\brief
		cell object for providing variables for pathfinding and functions for pathfinding
	***********************************************************************************/
	struct Cell {

		f32 m_HCost{ 0 }, m_GCost{ 10000 }, m_FCost{ 0 };
		b8 m_isAccesible{ true };

		//the came from cell or parent cell
		Cell* m_ParentCell {nullptr};

		Color m_Color_Tint;

		u32 unitEntity;
		Math::Vector2Int coordinate = { 0,0 };
		
		void CalculateFCost()
		{
			m_FCost = m_GCost + m_HCost;
		}

		bool operator==(const Cell& lhs) const
		{
			return ((lhs.coordinate.x == coordinate.x) && (lhs.coordinate.y == coordinate.y));
		}
	};
}
#endif