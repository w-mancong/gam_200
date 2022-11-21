/*!
file:	GamePlayInterface.cpp
author:	Tan Zhen Xiong
email:	t.zhenxiong@digipen.edu
brief:	This file contains the function definition for GamePlayInterface.cpp

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/

#include "pch.h"
#include <Engine/GamePlayInterface.h>

namespace ALEngine::Engine::GameplayInterface
{
	u32 getEntityCell(Room& currentRoom,u32 x, u32 y)
	{
		return currentRoom.roomCellsArray[y * currentRoom.width + x];
	}

	void ToggleCellToInaccessible(Room& currentRoom, u32 x, u32 y, b8 istrue) {
		Cell& cell = Coordinator::Instance()->GetComponent<Cell>(getEntityCell(currentRoom, x, y));
		cell.m_isAccesible = istrue;
	}

	bool CheckListContainsCell(std::list<ECS::Cell*> cellList, ECS::Cell& cellSearchNode)
	{
		for (auto it = cellList.begin(); it != cellList.end(); ++it) {
			if (*it == &cellSearchNode) {
				return true;
			}
		}
		return false;
	}

	bool IsCoordinateInsideRoom(Engine::GameplayInterface::Room& currentRoom, u32 gridX, u32 gridY)
	{
		return (gridX >= 0) && (gridX < currentRoom.width) && (gridY >= 0) && (gridY < currentRoom.height);
	}

	void InitializePatterns(std::vector<Pattern>& patternList) {
		patternList.clear();

		Pattern newPattern;
		
		//upside down T shape
		newPattern.coordinate_occupied.push_back({ 0, 0 });
		newPattern.coordinate_occupied.push_back({ 1, 0 });
		newPattern.coordinate_occupied.push_back({ -1, 0 });
		newPattern.coordinate_occupied.push_back({ 0, 1 });
		patternList.push_back(newPattern);
		newPattern.coordinate_occupied.clear();


		//Straight line
		newPattern.coordinate_occupied.push_back({ 0, 0 });
		newPattern.coordinate_occupied.push_back({ 1, 0 });
		newPattern.coordinate_occupied.push_back({ 2, 0 });
		newPattern.coordinate_occupied.push_back({ 3, 0 });
		patternList.push_back(newPattern);
		newPattern.coordinate_occupied.clear();

		//L Shape
		newPattern.coordinate_occupied.push_back({ 0, 0 });
		newPattern.coordinate_occupied.push_back({ 1, 0 });
		newPattern.coordinate_occupied.push_back({ 0, 1 });
		newPattern.coordinate_occupied.push_back({ 0, 2 });
		patternList.push_back(newPattern);
		newPattern.coordinate_occupied.clear();
	}

	void DisplayFilterPlacementGrid(Room& room, Vector2Int coordinate, Pattern pattern, Color color) {
		std::cout << pattern.coordinate_occupied.size();
		//Shift through each grid that the pattern would be in relative to given coordinate
		for (int i = 0; i < pattern.coordinate_occupied.size(); ++i) {
			//If the coordinate is within the boundaries of the room
			if (IsCoordinateInsideRoom(room, coordinate.x + pattern.coordinate_occupied[i].x, coordinate.y + pattern.coordinate_occupied[i].y)) {
				//If inside room, set the cell color to yellow
				ECS::Entity cellEntity = getEntityCell(room, coordinate.x + pattern.coordinate_occupied[i].x, coordinate.y + pattern.coordinate_occupied[i].y);

				Cell& cell = Coordinator::Instance()->GetComponent<Cell>(cellEntity);
				cell.m_Color_Tint = color;
				
			}
		}//End loop through pattern body check
	}

	void InitializePatternGUI(std::vector<ECS::Entity>& GUI_Pattern_Button_Entities) {
		GUI_Pattern_Button_Entities.clear();

		//There will be a fix of 4 buttons
		for (int i = 0; i < 4; ++i) {
			GUI_Pattern_Button_Entities.push_back(Coordinator::Instance()->CreateEntity());
		}

		u32 x_offset = 150;

		//First one will be the current
		Transform transform;
		transform.position = { 1000.f, 100.f, 0.f };
		transform.scale = { 100.f, 100.f };

		Coordinator::Instance()->AddComponent(GUI_Pattern_Button_Entities[0], transform);

		//The other 3 will be in queue
		transform.position = { 1000.f + x_offset, 100.f, 0.f };
		Coordinator::Instance()->AddComponent(GUI_Pattern_Button_Entities[1], transform);
		
		transform.position = { 1000.f + x_offset * 2.f, 100.f, 0.f };
		Coordinator::Instance()->AddComponent(GUI_Pattern_Button_Entities[2], transform);

		transform.position = { 1000.f + x_offset * 3.f, 100.f, 0.f };
		Coordinator::Instance()->AddComponent(GUI_Pattern_Button_Entities[3], transform);
	}
}
