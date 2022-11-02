#ifndef PATH_FINDING_MANAGER_H
#define PATH_FINDING_MANAGER_H

/*!
file: PathFindingManager.h
author: Chan Jie Ming Stanley
co-author:	Tan Zhen Xiong
email: c.jiemingstanley\@digipen.edu
email:	t.zhenxiong@digipen.edu
brief: This file contains function declaration for PathFindingManager
All content :copyright: 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#include "Engine/GamePlayInterface.h"


namespace ALEngine::Engine::AI
{
	/*!*********************************************************************************
	\brief
	The Astar pathfinding main logic function
	***********************************************************************************/
	std::vector<ECS::Entity> FindPath(Engine::GameplayInterface::Room& currentRoom, ECS::Entity startCell, ECS::Entity endCell, bool defaultAstar = false);

	/*!*********************************************************************************
	\brief
	The function to get neighbouring cells 
	***********************************************************************************/
	std::list<ECS::Cell*> GetNeighbourList(ECS::Cell& currentNode, Engine::GameplayInterface::Room& currentRoom, bool defaultAstar = false);

	/*!*********************************************************************************
	\brief
	The function to calculate and slot the number of cells
	***********************************************************************************/
	std::vector<ECS::Entity> CalculatePath(ECS::Cell& endNode, Engine::GameplayInterface::Room& currentRoom);

	/*!*********************************************************************************
	\brief
	The function to calculate huriestic distance cost between two cells
	***********************************************************************************/
    f32 CalculateDistanceCost(ECS::Entity a, ECS::Entity b);

	/*!*********************************************************************************
	\brief
	The function to get the lowest fcost cells
	***********************************************************************************/
	ECS::Cell& GetLowestFCostNode(std::list<ECS::Cell*> pathFindingCellNodeList);
}

#endif // !PATH_FINDING_MANAGER_H