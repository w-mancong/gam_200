#include "pch.h"


namespace ALEngine::ECS
{
    std::list<Cell> PathFindingManager::FindPath(Cell roomMap[], int roomMapSize  , Cell& startCell, Cell& endCell, bool defaultAstar)
    {
        Cell startNode{ startCell };
        Cell endNode{ endCell };

        //add start node to open list
        std::list<Cell> openList, closedList, pathList;

        openList.push_back(startNode);

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

            for (auto neighbourNode : GetNeighbourList(currentNode, defaultAstar))
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

        //out of nodes on open list or no path
        return pathList;
    }

    std::list<Cell> PathFindingManager::GetNeighbourList(Cell currentNode, bool defaultAstar)
    {
        std::list<Cell> neighbourList;
        //need to find out how to check grid or map

        ALEngine::Math::Vector2Int grid = currentNode.m_Grid;

        //if default astar pathfinding mode then do diagonal neighbour checks 
        if (defaultAstar)
        {
            //diagonal neighbour checks 
            if (ALEngine::ECS::Component::RoomBuilder::IsGridInside(grid + ALEngine::Math::Vector2Int(1, 1)))
            {

            }
            if (ALEngine::ECS::Component::RoomBuilder::IsGridInside(grid + ALEngine::Math::Vector2Int(1, -1)))
            {

            }

            if (ALEngine::ECS::Component::RoomBuilder::IsGridInside(grid + ALEngine::Math::Vector2Int(-1, 1)))
            {

            }
            if (ALEngine::ECS::Component::RoomBuilder::IsGridInside(grid + ALEngine::Math::Vector2Int(-1, -1)))
            {

            }
        }

        //astar without diagonal path, check for all neighbours except diagonal neighbours
        if (ALEngine::ECS::Component::RoomBuilder::IsGridInside(grid + ALEngine::Math::Vector2Int(1, 0))) //right
        {


        }
        if (ALEngine::ECS::Component::RoomBuilder::IsGridInside(grid + ALEngine::Math::Vector2Int(-1, 0))) //left
        {

        }
        if (ALEngine::ECS::Component::RoomBuilder::IsGridInside(grid + ALEngine::Math::Vector2Int(0, 1))) //up
        {

        }
        if (ALEngine::ECS::Component::RoomBuilder::IsGridInside(grid + ALEngine::Math::Vector2Int(0, -1))) //down
        {

        }

        return neighbourList;
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
        //ALEngine::Math::Vector3().Distance(a, b);
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

    bool PathFindingManager::CellListContain(std::list<Cell> cellList, Cell cellSearchNode)
    {
        return (std::find(cellList.begin(), cellList.end(), cellSearchNode) != cellList.end());
    }



}