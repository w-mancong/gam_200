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
}
