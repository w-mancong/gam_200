#include "pch.h"


namespace ALEngine::ECS
{
    std::list<Cell> PathFindingManager::FindPath(Cell roomMap[], int roomMapSize  , Cell& startCell, Cell& endCell)
    {
        Cell startNode{ startCell };
        Cell endNode{ endCell };

        //add start node to open list
        std::list<Cell> openList{startNode}, closedList, pathList;

        for (int i{ 0 }; i < roomMapSize; ++i)
        {
            roomMap[i].m_GCost = 99;
            roomMap[i].m_HCost = 0;
            roomMap[i].CalculateFCost();
            roomMap[i].m_ParentCell = NULL;
        }

        startNode.m_GCost = 0;
        startNode.m_FCost;
        startNode.CalculateFCost();

        while (!openList.empty())
        {
            Cell currentNode = GetLowestFCostNode(openList);

            if (&currentNode == &endNode)
            {
                pathList = CalculatePath(endCell);
                return pathList;
            }

            openList.remove(currentNode);
            closedList.push_back(currentNode);

            for (auto neighbourNode : GetNeighbourList(currentNode))
            {
                if (CellListContain(closedList, neighbourNode))
                {
                    continue;
                }

                float tentativeGCost = currentNode.m_GCost + CalculateDistanceCost(currentNode, neighbourNode);
                if (tentativeGCost < neighbourNode.m_GCost)
                {
                    neighbourNode.m_ParentCell = &currentNode;
                    neighbourNode.m_GCost = tentativeGCost;
                    neighbourNode.m_HCost = CalculateDistanceCost(neighbourNode, endNode);
                    neighbourNode.CalculateFCost();

                    if (!CellListContain(openList, neighbourNode))
                    {
                        openList.push_back(neighbourNode);
                    }
                }
            }
        }

        return pathList;
    }

    std::list<Cell> PathFindingManager::CalculatePath(Cell endNode)
    {
        std::list<Cell> path;

        path.push_back(endNode);

        Cell currentNode{ endNode };

        while (currentNode.m_ParentCell != NULL)
        {
            path.push_back(*currentNode.m_ParentCell);
            currentNode = *currentNode.m_ParentCell;
        }

        std::reverse(path.begin(), path.end());

        return path;
    }

    float PathFindingManager::CalculateDistanceCost(Cell a, Cell b)
    {
        //need to find out how to calculate distance
       
        return 0.0f;
    }

    Cell PathFindingManager::GetLowestFCostNode(std::list<Cell> pathFindingCellNodeList)
    {
        Cell lowestFCostNode = pathFindingCellNodeList.front();

        for (auto it = pathFindingCellNodeList.begin(); it != pathFindingCellNodeList.end(); ++it)
        {
            if (lowestFCostNode.m_FCost < it->m_FCost)
            {
                lowestFCostNode = *it;
            }
        }

        return lowestFCostNode;
    }

    std::list<Cell> PathFindingManager::GetNeighbourList(Cell currentNode)
    {
        std::list<Cell> neighbourList;
        //need to find out how to check grid or map

        return neighbourList;
    }

    bool PathFindingManager::CellListContain(std::list<Cell> cellList, Cell cellSearchNode)
    {
        return (std::find(cellList.begin(), cellList.end(), cellSearchNode) != cellList.end());
    }



}