#ifndef PATH_FINDING_MANAGER_H
#define PATH_FINDING_MANAGER_H

/*!
file: PathFindingManager.h
author: Chan Jie Ming Stanley
email: c.jiemingstanley\@digipen.edu
brief: This file contains function declaration for PathFindingManager
All content :copyright: 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#include "Engine/GamePlayInterface.h"


namespace ALEngine::Engine::AI
{
	std::vector<ECS::Entity> FindPath(Engine::GameplayInterface::Room& currentRoom, ECS::Entity startCell, ECS::Entity endCell, bool defaultAstar = false);

	std::list<ECS::Cell*> GetNeighbourList(ECS::Cell& currentNode, Engine::GameplayInterface::Room& currentRoom, bool defaultAstar = false);

	std::vector<ECS::Entity> CalculatePath(ECS::Cell& endNode, Engine::GameplayInterface::Room& currentRoom);

    f32 CalculateDistanceCost(ECS::Entity a, ECS::Entity b);

	ECS::Cell& GetLowestFCostNode(std::list<ECS::Cell*> pathFindingCellNodeList);
}

#endif // !PATH_FINDING_MANAGER_H
