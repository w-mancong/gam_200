#ifndef PATH_FINDING_MANAGER_H
#define PATH_FINDING_MANAGER_H

/*!
file: PathFindingManager.h
author: Chan Jie Ming Stanley
email: c.jiemingstanley\@digipen.edu
brief: This file contains function declaration for PathFindingManager
All content :copyright: 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
namespace ALEngine::ECS
{
	class PathFindingManager: public System
	{
	public:
		static std::vector<Entity> FindPath(Entity &roomMap, Entity startCell, Entity endCell, bool defaultAstar);
	private:
		static std::list<Cell> GetNeighbourList(Cell currentNode, bool defaultAstar);

		static std::list<Cell> CalculatePath(Cell endNode);

		static float CalculateDistanceCost(Cell a, Cell b);

		static Cell GetLowestFCostNode(std::list<Cell> pathFindingCellNodeList);

		static bool CellListContain(std::list<Cell> cellList, Cell cellSearchNode);
	};
}

#endif // !PATH_FINDING_MANAGER_H
