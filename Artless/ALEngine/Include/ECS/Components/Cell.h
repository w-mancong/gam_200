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

		float m_HCost{ 0 }, m_GCost{ 10000 }, m_FCost{ 0 };
		bool m_isAccesible{ true };
		//the came from cell or parent cell
		Cell* m_ParentCell {nullptr};

		u32 unitEntity;
		u32 coordinate[2];

		ALEngine::Math::Vector2Int m_Grid;

		void CalculateFCost()
		{
			m_FCost = m_GCost + m_HCost;
		}

		bool operator==(const Cell& lhs) const
		{
			return ((lhs.coordinate[0] == coordinate[0]) && (lhs.coordinate[1] == coordinate[1]));
		}
	};
}
#endif