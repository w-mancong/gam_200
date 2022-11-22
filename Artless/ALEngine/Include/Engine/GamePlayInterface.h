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

	struct Pattern {
		//grid occupied relative to where it will be placed, 0,0 will be the center. 0,1 will be 1 grid right. 
		std::vector<Vector2Int> coordinate_occupied;
	};


	/*!*********************************************************************************
	\brief
	Global function to return the cell entity based on specified position 
    ***********************************************************************************/
	u32 getEntityCell(Room& currentRoom, u32 x, u32 y);

	void ToggleCellToInaccessible(Room& currentRoom, u32 x, u32 y, b8 istrue);

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

	//Initialize Patterns
	void InitializePatterns(std::vector<Pattern>& patternList);

	//Initialize Pattern GUI
	void InitializePatternGUI(std::vector<ECS::Entity>& GUI_Pattern_Button_Entities);

	//Initialize Pattern GUI
	void InitializeAbilitiesGUI(std::vector<ECS::Entity>& GUI_Abilities_Button_Entities);

	//Filter Grids from Hovering Over cell during pattern select
	//void DisplayFilterPlacementGrid(Room& room, Vector2Int coordinate, Pattern pattern);
	void DisplayFilterPlacementGrid(Room& room, Vector2Int coordinate, Pattern pattern, Color color = { 1.f,1.f,1.f,1.f });
}
#endif
