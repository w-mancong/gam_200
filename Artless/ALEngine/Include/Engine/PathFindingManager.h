/*!
file: PathFindingManager.h
author:		Chan Jie Ming Stanley	(50%)
co-author:	Tan Zhen Xiong			(50%)
email: c.jiemingstanley\@digipen.edu
	   t.zhenxiong\@digipen.edu
brief: This file contains function declaration for PathFindingManager

All content :copyright: 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/

//#include "Engine/GamePlayInterface.h"
#include <Engine/Gameplay_Interface.h>
#include <GameplaySystem.h>
#include <Engine/Gameplay_Interface.h>

#ifndef PATH_FINDING_MANAGER_H
#define PATH_FINDING_MANAGER_H
namespace ALEngine::Engine::AI
{
	/*!*********************************************************************************
	\brief
	The Astar pathfinding main logic function
	***********************************************************************************/
	b8 FindPath(std::shared_ptr<Script::GameplaySystem> gameplaySystem, GAMEPLAY_SYSTEM_INTERFACE_H::Room& currentRoom, ECS::Entity startCell, ECS::Entity endCell, std::vector<ECS::Entity>& pathReturn, bool canWalkOverUnwalkable);

	/*!*********************************************************************************
	\brief
	The function to get neighbouring cells 
	***********************************************************************************/
	std::list<ECS::Cell*> GetNeighbourList(std::shared_ptr<Script::GameplaySystem> gameplaySystem, ECS::Cell& currentNode, GAMEPLAY_SYSTEM_INTERFACE_H::Room& currentRoom, bool defaultAstar = false);

	/*!*********************************************************************************
	\brief
	The function to calculate and slot the number of cells
	***********************************************************************************/
	std::vector<ECS::Entity> CalculatePath(std::shared_ptr<Script::GameplaySystem> gameplaySystem, ECS::Cell& endNode, GAMEPLAY_SYSTEM_INTERFACE_H::Room& currentRoom);

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
