#ifndef PATH_FINDING_MANAGER_H
#define PATH_FINDING_MANAGER_H

/*!
file: PathFindingManager.h
author: Chan Jie Ming Stanley
email: c.jiemingstanley\@digipen.edu
brief: This file contains function declaration for PathFindingManager
All content :copyright: 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
namespace ALEngine::Engine::AI
{
	
	std::vector<ECS::Entity> FindPath(ECS::Entity *roomMap, uint32_t size[], ECS::Entity startCell, ECS::Entity endCell, bool defaultAstar = false);

	std::list<ECS::Cell> GetNeighbourList(ECS::Cell currentNode, bool defaultAstar);

	std::list<ECS::Cell> CalculatePath(ECS::Cell endNode);

    float CalculateDistanceCost(ECS::Cell a, ECS::Cell b);

	ECS::Cell GetLowestFCostNode(std::list<ECS::Cell> pathFindingCellNodeList);

    bool CellListContain(std::list<ECS::Cell> cellList, ECS::Cell cellSearchNode);

}

#endif // !PATH_FINDING_MANAGER_H
