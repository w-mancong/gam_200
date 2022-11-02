#include "pch.h"

#include <Engine/PathFindingManager.h>

namespace  ALEngine::Engine::AI
{
 
    using Engine::GameplayInterface::Room;

    /*!*********************************************************************************
	\brief
	The Astar pathfinding main logic function
	***********************************************************************************/
    std::vector<ECS::Entity> FindPath(Room& currentRoom, ECS::Entity startCell, ECS::Entity endCell, bool defaultAstar)
    {
        Cell& startNode = Coordinator::Instance()->GetComponent<Cell>(startCell);
        Cell& endNode = Coordinator::Instance()->GetComponent<Cell>(endCell);

        //add start node to open list
        std::list<Cell*> openList, closedList;
        std::vector<u32> pathList;
        openList.push_back(&startNode);

        std::vector<Cell> cellRoomMap;// vector of room cells

        for (u32 i{ 0 }; i < currentRoom.width; ++i)
        {
            for (u32 j{ 0 }; j < currentRoom.height; ++j)
            {
                Cell& c = Coordinator::Instance()->GetComponent<Cell>(currentRoom.roomCellsArray[i + j]);

                c.m_GCost = 99999999;
                c.m_HCost = 0;
                c.CalculateFCost();
                c.m_ParentCell = NULL;

                c.coordinate[0] = i;
                c.coordinate[1] = j;

                cellRoomMap.push_back(c);
            }

        }

        //set start cell costs
        startNode.m_GCost = 0;
        startNode.m_HCost = CalculateDistanceCost(Engine::GameplayInterface::getEntityCell(currentRoom, startNode.coordinate[0], startNode.coordinate[1]), Engine::GameplayInterface::getEntityCell(currentRoom, endNode.coordinate[0], endNode.coordinate[1]));
        startNode.CalculateFCost();

        //well openlist not empty
        while (!openList.empty())
        {
            Cell& currentNode = GetLowestFCostNode(openList);

            if (currentNode == endNode)
            {
                pathList = std::move(CalculatePath(endNode, currentRoom));
                return pathList;
            }

             //remove from openlist
             openList.remove(&currentNode);
             //insert into closedlist
             closedList.push_back(&currentNode);

            for (auto neighbourNode : GetNeighbourList(currentNode, currentRoom, defaultAstar))
            {
                if (Engine::GameplayInterface::CheckListContainsCell(closedList, *neighbourNode))
                {
                    continue;
                }

                float tentativeGCost = currentNode.m_GCost + CalculateDistanceCost(Engine::GameplayInterface::getEntityCell(currentRoom ,currentNode.coordinate[0], currentNode.coordinate[1]) , Engine::GameplayInterface::getEntityCell(currentRoom, neighbourNode->coordinate[0], neighbourNode->coordinate[1]));
                if (tentativeGCost < neighbourNode->m_GCost)
                {
                    neighbourNode->m_ParentCell = &currentNode;
                    neighbourNode->m_GCost = tentativeGCost;
                    neighbourNode->m_HCost = CalculateDistanceCost(Engine::GameplayInterface::getEntityCell(currentRoom, neighbourNode->coordinate[0], neighbourNode->coordinate[1]), Engine::GameplayInterface::getEntityCell(currentRoom, endNode.coordinate[0], endNode.coordinate[1]));
                    neighbourNode->CalculateFCost();

                    if (!Engine::GameplayInterface::CheckListContainsCell(openList, *neighbourNode))
                    {
                        openList.push_back(neighbourNode);
                    }
                }
            }
        }

        //out of nodes on open list or no path
        return pathList;
    }

    /*!*********************************************************************************
    \brief
    The function to get neighbouring cells
    ***********************************************************************************/
    std::list<ECS::Cell*> GetNeighbourList(ECS::Cell& currentNode, Engine::GameplayInterface::Room& currentRoom, bool defaultAstar)
    {
        std::list<Cell*> neighbourList;
        //need to find out how to check grid or map

        u32 currentCoordinate[2]{ currentNode.coordinate[0],currentNode.coordinate[1]};

        //check cell in grid
        //astar without diagonal path, check for all neighbours except diagonal neighbour
        if (IsCoordinateInsideRoom(currentRoom, currentCoordinate[0] + 1, currentCoordinate[1] + 0)) //right 1,0
        {
           Cell& c = Coordinator::Instance()->GetComponent<Cell>(Engine::GameplayInterface::getEntityCell(currentRoom,currentCoordinate[0] + 1, currentCoordinate[1]));
           neighbourList.push_back(&c);
        }
        if (IsCoordinateInsideRoom(currentRoom, currentCoordinate[0] + -1, currentCoordinate[1] + 0)) //left -1,0
        {
            Cell& c = Coordinator::Instance()->GetComponent<Cell>(Engine::GameplayInterface::getEntityCell(currentRoom, currentCoordinate[0] -1, currentCoordinate[1]));
            neighbourList.push_back(&c);
        }
        if (IsCoordinateInsideRoom(currentRoom, currentCoordinate[0] + 0, currentCoordinate[1] + 1)) //up 0,1
        {
            Cell& c = Coordinator::Instance()->GetComponent<Cell>(Engine::GameplayInterface::getEntityCell(currentRoom, currentCoordinate[0], currentCoordinate[1] + 1));
            neighbourList.push_back(&c);
        }
        if (IsCoordinateInsideRoom(currentRoom, currentCoordinate[0] + 1, currentCoordinate[1] + -1)) //down 0,-1
        {
            Cell& c = Coordinator::Instance()->GetComponent<Cell>(Engine::GameplayInterface::getEntityCell(currentRoom, currentCoordinate[0], currentCoordinate[1] -1));
            neighbourList.push_back(&c);
        }

        return neighbourList;
    }

    /*!*********************************************************************************
    \brief
    The function to calculate and slot the number of cells
    ***********************************************************************************/
    std::vector<ECS::Entity> CalculatePath(Cell& endNode, Engine::GameplayInterface::Room& currentRoom)
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

        //std::reverse(pathlist.begin(), pathlist.end());

        for (auto it = pathlist.begin(); it != pathlist.end(); ++it)
        {
            Path.push_back(Engine::GameplayInterface::getEntityCell(currentRoom, it->coordinate[0], it->coordinate[1]));
        }

        return Path;
    }

    /*!*********************************************************************************
	\brief
	The function to calculate huriestic distance cost between two cells
	***********************************************************************************/
    f32 CalculateDistanceCost(ECS::Entity a, ECS::Entity b)
    {
        //need to find out how to calculate distance
        Transform & a_Tranform= Coordinator::Instance()->GetComponent<Transform>(a);
        Transform& b_Tranform = Coordinator::Instance()->GetComponent<Transform>(b);
        return Vector3::Distance(a_Tranform.position, b_Tranform.position);
    }

    /*!*********************************************************************************
    \brief
    The function to get the lowest fcost cells
    ***********************************************************************************/
    Cell& GetLowestFCostNode(std::list<Cell*> pathFindingCellNodeList)
    {
        Cell* lowestFCostNode = pathFindingCellNodeList.front();

        for (auto it = pathFindingCellNodeList.begin(); it != pathFindingCellNodeList.end(); ++it)
        {
            if (lowestFCostNode->m_FCost > (*it)->m_FCost)
            {
                lowestFCostNode = *it;
            }
        }

        return *lowestFCostNode;
    }
}

