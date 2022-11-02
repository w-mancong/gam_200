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
	class PathFindingManager
	{
	public:

		std::list<Cell> FindPath(Cell roomMap[], int roomMapSize, Cell& startCell, Cell& endCell, bool defaultAstar);
	private:
		std::list<Cell> GetNeighbourList(Cell currentNode, bool defaultAstar);

		std::list<Cell> CalculatePath(Cell endNode);

		float CalculateDistanceCost(Cell a, Cell b);

		Cell GetLowestFCostNode(std::list<Cell> pathFindingCellNodeList);

		bool CellListContain(std::list<Cell> cellList, Cell cellSearchNode);
	};
}

#endif // !PATH_FINDING_MANAGER_H
