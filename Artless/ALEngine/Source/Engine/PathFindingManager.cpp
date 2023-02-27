/*!
file: PathFindingManager.cpp
author:		Chan Jie Ming Stanley	(50%)
co-author:	Tan Zhen Xiong			(50%)
email: c.jiemingstanley\@digipen.edu
       t.zhenxiong\@digipen.edu
brief: This file contains function definition for PathFindingManager

All content :copyright: 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#include "pch.h"
#include <Engine/PathFindingManager.h>

namespace  ALEngine::Engine::AI
{
    /*!*********************************************************************************
	\brief
	The Astar pathfinding main logic function
	***********************************************************************************/
    b8 FindPath(std::shared_ptr<Script::GameplaySystem> gameplaySystem, GAMEPLAY_SYSTEM_INTERFACE_H::Room& currentRoom, ECS::Entity startCell, ECS::Entity endCell, std::vector<ECS::Entity>& pathReturn, bool canWalkOverUnwalkable)
    {
        Cell& startNode = Coordinator::Instance()->GetComponent<Cell>(startCell);
        Cell& endNode = Coordinator::Instance()->GetComponent<Cell>(endCell);

        if (!endNode.m_isAccessible || (!canWalkOverUnwalkable && !endNode.m_canWalk) || endNode.has_Wall) {
            return false;
        }      
        

        if (startNode.coordinate == endNode.coordinate) {
            return false;
        }

        //Open list contains all nodes to be checked
        //Closed list contains nodes that are already searched
        std::list<Cell*> openList, closedList;
        
        //add start node to open list
        openList.push_back(&startNode);

        std::vector<Cell> cellRoomMap;// vector of room cells

        for (u32 i{ 0 }; i < currentRoom.width; ++i)
        {
            for (u32 j{ 0 }; j < currentRoom.height; ++j)
            {
                //Get the cell of the current entity
                Cell& c = Coordinator::Instance()->GetComponent<Cell>(currentRoom.roomCellsArray[i * currentRoom.width + j]);

                //Set G to 0
                c.m_GCost = 0.0f;

                //Set H to Distance
                c.m_HCost = 0.0f;

                //Reset F
                c.m_FCost = c.m_GCost + c.m_HCost;

                //Set parent
                c.m_ParentCell = NULL;

                //Add to cell room
                cellRoomMap.push_back(c);
            }
        }

        //set start cell costs
        startNode.m_FCost = 0.0f;

        //when openlist not empty
        //When there are still more neighbour nodes to check
        while (!openList.empty())
        {
            //get the lowest F cost node from nodes to check (open list)
            Cell& currentNode = GetLowestFCostNode(openList);

            //remove from openlist
            openList.remove(&currentNode);

            //insert into closedlist
            closedList.push_back(&currentNode);

            //Check neighbour from current node
            for (auto neighbourNode : GetNeighbourList(gameplaySystem, currentNode, currentRoom))
            {
                //If the neighbour is the end
                if (neighbourNode == &endNode) {
                    neighbourNode->m_ParentCell = &currentNode;     //Update parent cell    

                    //Calculate the path and return it
                    pathReturn = std::move(CalculatePath(gameplaySystem, endNode, currentRoom));
                    return true;
                }
                //If it's not end
                //then check if the current neighbour node exist inside closed list or it is blocked
                else if (gameplaySystem->CheckListContainsCell(closedList, *neighbourNode) || neighbourNode->has_Wall || !neighbourNode->m_isAccessible || (!canWalkOverUnwalkable && !neighbourNode->m_canWalk) || neighbourNode->hasUnit)
                {
                    continue;
                }
                
                //Otherwise, it's a cell we can check
                neighbourNode->m_ParentCell = &currentNode;     //Update parent cell  
                neighbourNode->m_HCost = CalculateDistanceCost(
                    gameplaySystem->getEntityCell(currentRoom, neighbourNode->coordinate.x, neighbourNode->coordinate.y),
                    gameplaySystem->getEntityCell(currentRoom, endNode.coordinate.x, endNode.coordinate.y));
                neighbourNode->m_GCost = currentNode.m_GCost + 1.0f;
                neighbourNode->m_FCost = neighbourNode->m_GCost + neighbourNode->m_HCost;

                //If the iterated neighbourn node does not exist in the open list 
                //Add it to the open list
                if (!gameplaySystem->CheckListContainsCell(openList, *neighbourNode))
                {
                    openList.push_back(neighbourNode);
                }
            }//End check neigbour node loop
        }//End open list loop check

        //out of nodes on open list or no path
        return false;
    }

    /*!*********************************************************************************
    \brief
    The function to get neighbouring cells
    ***********************************************************************************/
    std::list<ECS::Cell*> GetNeighbourList(std::shared_ptr<Script::GameplaySystem> gameplaySystem, ECS::Cell& currentNode, GAMEPLAY_SYSTEM_INTERFACE_H::Room& currentRoom, [[maybe_unused]] bool defaultAstar)
    {
        std::list<Cell*> neighbourList;
        
        u32 currentCoordinate[2]{ static_cast<u32>(currentNode.coordinate.x), static_cast<u32>(currentNode.coordinate.y) };

        //check cell in grid
        //astar without diagonal path, check for all neighbours except diagonal neighbour
        if (gameplaySystem->IsCoordinateInsideRoom(currentRoom, currentCoordinate[0] + 1, currentCoordinate[1])) //right 1,0
        {
           Cell& c = Coordinator::Instance()->GetComponent<Cell>(gameplaySystem->getEntityCell(currentRoom,currentCoordinate[0] + 1, currentCoordinate[1]));
           neighbourList.push_back(&c);
        }
        if (gameplaySystem->IsCoordinateInsideRoom(currentRoom, currentCoordinate[0] - 1, currentCoordinate[1])) //left -1,0
        {
            Cell& c = Coordinator::Instance()->GetComponent<Cell>(gameplaySystem->getEntityCell(currentRoom, currentCoordinate[0] - 1, currentCoordinate[1]));
            neighbourList.push_back(&c);
        }
        if (gameplaySystem->IsCoordinateInsideRoom(currentRoom, currentCoordinate[0], currentCoordinate[1] + 1)) //up 0,1
        {
            Cell& c = Coordinator::Instance()->GetComponent<Cell>(gameplaySystem->getEntityCell(currentRoom, currentCoordinate[0], currentCoordinate[1] + 1));
            neighbourList.push_back(&c);
        }
        if (gameplaySystem->IsCoordinateInsideRoom(currentRoom, currentCoordinate[0], currentCoordinate[1] - 1)) //down 0,-1
        {
            Cell& c = Coordinator::Instance()->GetComponent<Cell>(gameplaySystem->getEntityCell(currentRoom, currentCoordinate[0], currentCoordinate[1] -1));
            neighbourList.push_back(&c);
        }

        return neighbourList;
    }

    /*!*********************************************************************************
    \brief
    The function to calculate and slot the number of cells
    It returns a path build from the end node back to the start by iterating through the parent node.
    ***********************************************************************************/
    std::vector<ECS::Entity> CalculatePath(std::shared_ptr<Script::GameplaySystem> gameplaySystem, Cell& endNode, GAMEPLAY_SYSTEM_INTERFACE_H::Room& currentRoom)
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

        for (auto it = pathlist.begin(); it != pathlist.end(); ++it)
        {
            Path.push_back(gameplaySystem->getEntityCell(currentRoom, it->coordinate.x, it->coordinate.y));
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

