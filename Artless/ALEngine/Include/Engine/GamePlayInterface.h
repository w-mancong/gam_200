#ifndef	GAMEPLAY_INTERFACE_H
#define GAMEPLAY_INTERFACE_H
/*!
file:	GamePlayInterface.h
author:	Tan Zhen Xiong
email:	t.zhenxiong@digipen.edu
brief:	This file contains the function definition for GamePlayInterface.h

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/


namespace ALEngine::Engine::GameplayInterface
{
	struct Room
	{
		u32* roomCellsArray;
		u32 width, height;
		u32 roomSize;
	};

	/*!*********************************************************************************
	\brief
	Global function to return the cell entity based on specified position 
    ***********************************************************************************/
	u32 getEntityCell(Room& currentRoom, u32 x, u32 y);

	/*!*********************************************************************************
	\brief
	Global function to check if list contains cell 
    ***********************************************************************************/
	bool CheckListContainsCell(std::list<ECS::Cell*> cellList, ECS::Cell& cellSearchNode);

	/*!*********************************************************************************
	\brief
	Global function to check cell is inside room of cells
    ***********************************************************************************/
	bool IsCoordinateInsideRoom(Engine::GameplayInterface::Room& currentRoom, u32 gridX, u32 gridY);
}
#endif
