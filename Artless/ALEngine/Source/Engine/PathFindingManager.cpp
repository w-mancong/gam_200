#include "pch.h"
#include <Engine/PathFindingManager.h>

namespace  ALEngine::Engine::AI
{
    std::vector<ECS::Entity> FindPath(ECS::Entity&roomMap  , ECS::Entity startCell, ECS::Entity endCell, bool defaultAstar)
    {
        Cell startNode;
        startNode.unitEntity = startCell;
        Cell endNode;
        endNode.unitEntity = endCell;

        //add start node to open list
        std::list<Cell> openList, closedList;
        std::vector<u32> pathList;
        openList.push_back(startNode);

        //std::vector<Cell> cellRoomMap;
        //for (int i{ 0 }; i < cellRoomMap.size(); ++i)
        //{
        //    cellRoomMap[i].m_GCost = 99;
        //    cellRoomMap[i].m_HCost = 0;
        //    cellRoomMap[i].CalculateFCost();
        //    cellRoomMap[i].m_ParentCell = NULL;
        //}

        //startNode.m_GCost = 0;
        //startNode.m_FCost;
        //startNode.CalculateFCost();

        //while (!openList.empty())
        //{
        //    Cell currentNode = GetLowestFCostNode(openList);

        //    if (&currentNode == &endNode)
        //    {
        //        //pathList = CalculatePath(endCell);
        //        return pathList;
        //    }
      
        //     openList.remove(currentNode);
        //     closedList.push_back(currentNode);

        //    for (auto neighbourNode : GetNeighbourList(currentNode, defaultAstar))
        //    {
        //        if (CellListContain(closedList, neighbourNode))
        //        {
        //            continue;
        //        }

        //        float tentativeGCost = currentNode.m_GCost + CalculateDistanceCost(currentNode, neighbourNode);
        //        if (tentativeGCost < neighbourNode.m_GCost)
        //        {
        //            neighbourNode.m_ParentCell = &currentNode;
        //            neighbourNode.m_GCost = tentativeGCost;
        //            neighbourNode.m_HCost = CalculateDistanceCost(neighbourNode, endNode);
        //            neighbourNode.CalculateFCost();

        //            if (!CellListContain(openList, neighbourNode))
        //            {
        //                openList.push_back(neighbourNode);
        //            }
        //        }
        //    }
        //}

        //out of nodes on open list or no path
        return pathList;
    }

    std::list<ECS::Cell> GetNeighbourList(ECS::Cell currentNode, bool defaultAstar)
    {
        std::list<Cell> neighbourList;
        //need to find out how to check grid or map

        ALEngine::Math::Vector2Int grid = currentNode.m_Grid;

        //if default astar pathfinding mode then do diagonal neighbour checks 
        if (defaultAstar)
        {
            //diagonal neighbour checks 

            //if (ALEngine::ECS::Component::RoomBuilder::IsGridInside(grid + ALEngine::Math::Vector2Int(1, 1)))
            //{
            //    
            //}
            //if (ALEngine::ECS::Component::RoomBuilder::IsGridInside(grid + ALEngine::Math::Vector2Int(1, -1)))
            //{

            //}

            //if (ALEngine::ECS::Component::RoomBuilder::IsGridInside(grid + ALEngine::Math::Vector2Int(-1, 1)))
            //{

            //}
            //if (ALEngine::ECS::Component::RoomBuilder::IsGridInside(grid + ALEngine::Math::Vector2Int(-1, -1)))
            //{

            //}
        }

        //astar without diagonal path, check for all neighbours except diagonal neighbours
        //if (ALEngine::ECS::Component::RoomBuilder::IsGridInside(grid + ALEngine::Math::Vector2Int(1, 0))) //right
        //{

        //}
        //if (ALEngine::ECS::Component::RoomBuilder::IsGridInside(grid + ALEngine::Math::Vector2Int(-1, 0))) //left
        //{

        //}
        //if (ALEngine::ECS::Component::RoomBuilder::IsGridInside(grid + ALEngine::Math::Vector2Int(0, 1))) //up
        //{

        //}
        //if (ALEngine::ECS::Component::RoomBuilder::IsGridInside(grid + ALEngine::Math::Vector2Int(0, -1))) //down
        //{

        //}

        return neighbourList;
    }

    std::list<Cell> CalculatePath(Cell endNode)
    {
        std::list<Cell> pathlist;
        std::vector<u32> Path;
        pathlist.push_back(endNode);

        Cell currentNode{ endNode };

        while (currentNode.m_ParentCell != NULL)
        {
            pathlist.push_back(*currentNode.m_ParentCell);
            currentNode = *currentNode.m_ParentCell;
        }

        std::reverse(pathlist.begin(), pathlist.end());

        return pathlist;
    }

    float CalculateDistanceCost(Cell a, Cell b)
    {
        //need to find out how to calculate distance
        //ALEngine::Math::Vector3().Distance(a, b);
        
        return 0.0f;
    }

    Cell GetLowestFCostNode(std::list<Cell> pathFindingCellNodeList)
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

    bool CellListContain(std::list<Cell> cellList, Cell cellSearchNode)
    {
        return (std::find(cellList.begin(), cellList.end(), cellSearchNode) != cellList.end());
    }



}

