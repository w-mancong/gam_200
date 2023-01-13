/*!
file:   Cell.h
author: Tan Zhen Xiong
email:  t.zhenxiong\@digipen.edu
brief:	This file contains the function declarations for Cell.h

		All content :copyright: 2022 DigiPen Institute of Technology Singapore. All rights reserved.
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
		//HCost - Heuristic Cost (Actual Distance)
		//GCost - Movement cost per cell to move
		//FCost - Total cost of node
		f32 m_HCost{ 0 }, m_GCost{ 10000 }, m_FCost{ 0 };

		//IsAccessible - whether or not pathfinding will block entirely
		//canWalk - accessible and if walkable by player unit
		b8 m_isAccessible{ true }, m_canWalk{ false };

		//A counter that resets the tile's walkability after 2 turns
		s32 m_resetCounter{ 0 };

		//Has unit
		b8 hasUnit { false};

		//the came from cell or parent cell
		Cell* m_ParentCell {nullptr};

		//tint of color for cell
		Color m_Color_Tint;

		//unit entity that is on the cell
		u32 unitEntity;

		//Coordinate of the cll
		Math::Vector2Int coordinate = { 0,0 };

		/*!*********************************************************************************
		\brief
			Check if the cells have same coordinates
		***********************************************************************************/
		bool operator==(const Cell& lhs) const
		{
			return ((lhs.coordinate.x == coordinate.x) && (lhs.coordinate.y == coordinate.y));
		}
	};
}
#endif