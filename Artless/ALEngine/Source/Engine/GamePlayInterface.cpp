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
		cell.m_isBlocked = istrue;

		if(!cell.m_isBlocked)
		Coordinator::Instance()->GetComponent<Sprite>(getEntityCell(currentRoom, x, y)).color = { 0.f,0.f,0.f,0.f };
		else
		Coordinator::Instance()->GetComponent<Sprite>(getEntityCell(currentRoom, x, y)).color = { 1.f,1.f,1.f,1.f };
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

		//Box Shape
		newPattern.coordinate_occupied.push_back({ 0, 0 });
		newPattern.coordinate_occupied.push_back({ 1, 0 });
		newPattern.coordinate_occupied.push_back({ 1, 1 });
		newPattern.coordinate_occupied.push_back({ 0, 1 });
		patternList.push_back(newPattern);
		newPattern.coordinate_occupied.clear();
	}

	//Initialize Abilities
	void InitializeAbilities(std::vector<Abilities>& abilitiesList) {
		abilitiesList.clear();

		Abilities new_ability;

		new_ability.current_type = TYPE_ABILITIES::HARD_DROP;
		new_ability.damage = 15;
		abilitiesList.push_back(new_ability);

		new_ability.current_type = TYPE_ABILITIES::LIFE_DRAIN;
		new_ability.damage = 12;
		abilitiesList.push_back(new_ability);
	}

	void DisplayFilterPlacementGrid(Room& room, Vector2Int coordinate, Pattern pattern, Color color) {
		//Shift through each grid that the pattern would be in relative to given coordinate
		for (int i = 0; i < pattern.coordinate_occupied.size(); ++i) {
			//If the coordinate is within the boundaries of the room
			if (IsCoordinateInsideRoom(room, coordinate.x + pattern.coordinate_occupied[i].x, coordinate.y + pattern.coordinate_occupied[i].y)) {
				//If inside room, set the cell color to yellow
				ECS::Entity cellEntity = getEntityCell(room, coordinate.x + pattern.coordinate_occupied[i].x, coordinate.y + pattern.coordinate_occupied[i].y);

				Cell& cell = Coordinator::Instance()->GetComponent<Cell>(cellEntity);
				if (!cell.m_isBlocked) {
					continue;
				}

				cell.m_Color_Tint = color;

				Sprite& sprite = Coordinator::Instance()->GetComponent<Sprite>(cellEntity);
				sprite.color = color;
			}
		}//End loop through pattern body check
	}

	void PlacePatternOntoGrid(Room& room, Vector2Int coordinate, Pattern pattern, std::string sprite_fileName) {
		//Shift through each grid that the pattern would be in relative to given coordinate
		for (int i = 0; i < pattern.coordinate_occupied.size(); ++i) {
			//If the coordinate is within the boundaries of the room
			if (IsCoordinateInsideRoom(room, coordinate.x + pattern.coordinate_occupied[i].x, coordinate.y + pattern.coordinate_occupied[i].y)) {
				//If inside room, set the cell color to yellow
				ECS::Entity cellEntity = getEntityCell(room, coordinate.x + pattern.coordinate_occupied[i].x, coordinate.y + pattern.coordinate_occupied[i].y);

				Cell& cell = Coordinator::Instance()->GetComponent<Cell>(cellEntity);
				if (!cell.m_isBlocked) {
					continue;
				}

				cell.m_canWalk = true;

				Sprite& sprite = Coordinator::Instance()->GetComponent<Sprite>(cellEntity);
				sprite.id = AssetManager::Instance()->GetGuid(sprite_fileName);
			}
		}//End loop through pattern body check
	}

	void PlaceWalkableOnGrid(Room& room, Vector2Int coordinate, std::string sprite_fileName) {
		ECS::Entity cellEntity = getEntityCell(room, coordinate.x, coordinate.y);

		Cell& cell = Coordinator::Instance()->GetComponent<Cell>(cellEntity);
		if (!cell.m_isBlocked) {
			return;
		}

		cell.m_canWalk = true;

		Sprite& sprite = Coordinator::Instance()->GetComponent<Sprite>(cellEntity);
		sprite.id = AssetManager::Instance()->GetGuid(sprite_fileName);
	}

	void InitializePatternGUI(std::vector<ECS::Entity>& GUI_Pattern_Button_Entities) {
		GUI_Pattern_Button_Entities.clear();

		//There will be a fix of 4 buttons
		for (int i = 1; i <= 4; ++i) {
			GUI_Pattern_Button_Entities.push_back(Coordinator::Instance()->GetEntityByTag("next_tile_icon" + std::to_string(i)));
			//GUI_Pattern_Button_Entities.push_back(Coordinator::Instance()->CreateEntity());
		}

		u32 x_offset = 150;

		//First one will be the current
		Transform transform;
		transform.position = { 1000.f, 100.f, 0.f };
		transform.scale = { 100.f, 100.f };

		EventTrigger eventTrigger;

		Coordinator::Instance()->AddComponent(GUI_Pattern_Button_Entities[0], eventTrigger);

		//The other 3 will be in queue
		transform.position = { 1000.f + x_offset, 100.f, 0.f };
		Coordinator::Instance()->AddComponent(GUI_Pattern_Button_Entities[1], eventTrigger);
		
		transform.position = { 1000.f + x_offset * 2.f, 100.f, 0.f };
		Coordinator::Instance()->AddComponent(GUI_Pattern_Button_Entities[2], eventTrigger);

		transform.position = { 1000.f + x_offset * 3.f, 100.f, 0.f };
		Coordinator::Instance()->AddComponent(GUI_Pattern_Button_Entities[3], eventTrigger);
	}

	void InitializeAbilitiesGUI(std::vector<ECS::Entity>& GUI_Abilities_Button_Entities) {
		GUI_Abilities_Button_Entities.clear();

		//There will be a fix of 4 buttons
		for (int i = 1; i <= 6; ++i) {
			GUI_Abilities_Button_Entities.push_back(Coordinator::Instance()->GetEntityByTag("skill_icon" + std::to_string(i)));
			//GUI_Abilities_Button_Entities.push_back(Coordinator::Instance()->CreateEntity());
		}

		u32 x_offset = 75;

		Vector3 startPos = { 50.f, 100.f, 0.f };

		//First one will be the current
		Transform transform;
		transform.position = startPos;
		transform.scale = { 50.f, 50.f };

		EventTrigger eventTrigger;

		//Coordinator::Instance()->AddComponent(GUI_Abilities_Button_Entities[0], transform);
		Coordinator::Instance()->AddComponent(GUI_Abilities_Button_Entities[0], eventTrigger);

		//The other 3 will be in queue
		transform.position = { startPos.x + x_offset, 100.f, 0.f };
		//Coordinator::Instance()->AddComponent(GUI_Abilities_Button_Entities[1], transform);
		Coordinator::Instance()->AddComponent(GUI_Abilities_Button_Entities[1], eventTrigger);

		transform.position = { startPos.x + x_offset * 2.f, 100.f, 0.f };
		//Coordinator::Instance()->AddComponent(GUI_Abilities_Button_Entities[2], transform);
		Coordinator::Instance()->AddComponent(GUI_Abilities_Button_Entities[2], eventTrigger);

		transform.position = { startPos.x + x_offset * 3.f, 100.f, 0.f };
		//Coordinator::Instance()->AddComponent(GUI_Abilities_Button_Entities[3], transform);
		Coordinator::Instance()->AddComponent(GUI_Abilities_Button_Entities[3], eventTrigger);

		transform.position = { startPos.x + x_offset * 4.f, 100.f, 0.f };
		//Coordinator::Instance()->AddComponent(GUI_Abilities_Button_Entities[4], transform);
		Coordinator::Instance()->AddComponent(GUI_Abilities_Button_Entities[4], eventTrigger);

		transform.position = { startPos.x + x_offset * 5.f, 100.f, 0.f };
		//Coordinator::Instance()->AddComponent(GUI_Abilities_Button_Entities[5], transform);
		Coordinator::Instance()->AddComponent(GUI_Abilities_Button_Entities[5], eventTrigger);
	}

	bool CheckIfPatternCanBePlacedForTile(Room& room, Vector2Int coordinate, Pattern pattern) {
		//Shift through each grid that the pattern would be in relative to given coordinate
		for (int i = 0; i < pattern.coordinate_occupied.size(); ++i) {
			//If the coordinate is within the boundaries of the room
			if (IsCoordinateInsideRoom(room, coordinate.x + pattern.coordinate_occupied[i].x, coordinate.y + pattern.coordinate_occupied[i].y)) {
				//If inside room, set the cell color to yellow
				ECS::Entity cellEntity = getEntityCell(room, coordinate.x + pattern.coordinate_occupied[i].x, coordinate.y + pattern.coordinate_occupied[i].y);

				Cell& cell = Coordinator::Instance()->GetComponent<Cell>(cellEntity);
				if (!cell.m_isBlocked) {
					continue;
				}

				if (cell.m_canWalk) {
					return false;
				}

			}
		}//End loop through pattern body check

		return true;
	}

	bool CheckIfAbilitiesCanBePlacedForTile(Room& room, Vector2Int coordinate, Pattern pattern) {
		//Shift through each grid that the pattern would be in relative to given coordinate
		for (int i = 0; i < pattern.coordinate_occupied.size(); ++i) {
			//If the coordinate is within the boundaries of the room
			if (IsCoordinateInsideRoom(room, coordinate.x + pattern.coordinate_occupied[i].x, coordinate.y + pattern.coordinate_occupied[i].y)) {
				//If inside room, set the cell color to yellow
				ECS::Entity cellEntity = getEntityCell(room, coordinate.x + pattern.coordinate_occupied[i].x, coordinate.y + pattern.coordinate_occupied[i].y);

				Cell& cell = Coordinator::Instance()->GetComponent<Cell>(cellEntity);
				
				if (cell.hasUnit) {
					Unit& unit = Coordinator::Instance()->GetComponent<Unit>(cell.unitEntity);

					if (unit.unitType == UNIT_TYPE::PLAYER) {
						return true;
					}
				}
			}
		}//End loop through pattern body check

		return false;
	}

	void RunAbilities_OnCells(Room& room, Vector2Int coordinate, Pattern pattern, Abilities abilities) {
		//Shift through each grid that the pattern would be in relative to given coordinate
		for (int i = 0; i < pattern.coordinate_occupied.size(); ++i) {
			//If the coordinate is within the boundaries of the room
			if (IsCoordinateInsideRoom(room, coordinate.x + pattern.coordinate_occupied[i].x, coordinate.y + pattern.coordinate_occupied[i].y)) {
				//If inside room, set the cell color to yellow
				ECS::Entity cellEntity = getEntityCell(room, coordinate.x + pattern.coordinate_occupied[i].x, coordinate.y + pattern.coordinate_occupied[i].y);

				Cell& cell = Coordinator::Instance()->GetComponent<Cell>(cellEntity);

				if (cell.hasUnit) {
					Unit& unit = Coordinator::Instance()->GetComponent<Unit>(cell.unitEntity);
					if (unit.health <= 0) {
						continue;
					}

					u32 initialHealth = unit.health;

					if (unit.unitType == UNIT_TYPE::ENEMY) {
						switch (abilities.current_type)
						{
						case TYPE_ABILITIES::HARD_DROP:
							DoDamageToUnit(cell.unitEntity, abilities.damage);
							break;
						case TYPE_ABILITIES::LIFE_DRAIN:
							DoDamageToUnit(cell.unitEntity, abilities.damage);

							//Life steal 
							ECS::Entity playerEntity = Coordinator::Instance()->GetEntityByTag("Player");
							Unit& playerUnit = Coordinator::Instance()->GetComponent<Unit>(playerEntity);

							u32 healthDrained = unit.health < 0 ? initialHealth : abilities.damage;
							AL_CORE_INFO("Heal : " + std::to_string(healthDrained));

							playerUnit.health += healthDrained;

							if (playerUnit.health > playerUnit.maxHealth) {
								playerUnit.health = playerUnit.maxHealth;
							}
							break;
						}
					}
				}//End check if unit
			}//End check if it's inside room
		}//End loop through pattern body check
	}

	void DoDamageToUnit(ECS::Entity unitEntity, s32 damage) {
		Unit& unit = Coordinator::Instance()->GetComponent<Unit>(unitEntity);
		unit.health -= damage;

		AL_CORE_INFO(std::to_string(unit.health));

		if (unit.health <= 0) {
			AL_CORE_INFO("Enemy Died");
			Coordinator::Instance()->GetComponent<EntityData>(unitEntity).active = false;
			Coordinator::Instance()->GetComponent<EntityData>(unit.unit_Sprite_Entity).active = false;
		}
	}
}
