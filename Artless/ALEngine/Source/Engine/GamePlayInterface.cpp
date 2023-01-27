/*!
file:	GamePlayInterface.cpp
author:	Tan Zhen Xiong
email:	t.zhenxiong\@digipen.edu
brief:	This file contains the function definition for GamePlayInterface.cpp

		All content � 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/

#include "pch.h"
#include <Utility/AudioNames.h>
#include <Engine/GamePlayInterface.h>

namespace ALEngine::Engine::GameplayInterface
{
	u32 getEntityCell(Room& currentRoom,u32 x, u32 y)
	{
		//Get required cell's entity
		return currentRoom.roomCellsArray[y * currentRoom.width + x];
	}

	void ToggleCellAccessibility(Room& currentRoom, u32 x, u32 y, b8 istrue) {
		Cell& cell = Coordinator::Instance()->GetComponent<Cell>(getEntityCell(currentRoom, x, y));
		cell.m_isAccessible = istrue;	//set accessible

		Coordinator::Instance()->GetComponent<EntityData>(cell.child_overlay).active = istrue;

		//Toggle color accordingly
		if(!cell.m_isAccessible)
		Coordinator::Instance()->GetComponent<Sprite>(getEntityCell(currentRoom, x, y)).color = { 0.f,0.f,0.f,0.f };
		else
		Coordinator::Instance()->GetComponent<Sprite>(getEntityCell(currentRoom, x, y)).color = { 1.f,1.f,1.f,1.f };
	}

	bool CheckListContainsCell(std::list<ECS::Cell*> cellList, ECS::Cell& cellSearchNode)
	{
		//If contain, returns true
		for (auto it = cellList.begin(); it != cellList.end(); ++it) {
			if (*it == &cellSearchNode) {
				return true;
			}
		}

		//Else false
		return false;
	}

	bool IsCoordinateInsideRoom(Engine::GameplayInterface::Room& currentRoom, u32 gridX, u32 gridY)
	{
		//Return if coordinate is inside room
		return (gridX >= 0) && (gridX < currentRoom.width) && (gridY >= 0) && (gridY < currentRoom.height);
	}

	bool IsCoordinateCellAccessible(Engine::GameplayInterface::Room& currentRoom, u32 gridX, u32 gridY) {
		//Return if Coordinate is accessible/walkable
		if (IsCoordinateInsideRoom(currentRoom, gridX, gridY)) {
			return Coordinator::Instance()->GetComponent<Cell>(getEntityCell(currentRoom, gridX, gridY)).m_isAccessible;
		}
		return false;
	}

	void InitializePatterns(std::vector<Pattern>& patternList) {
		//Clear pattern
		patternList.clear();

		//Template for pattern
		Pattern newPattern;
		
		//upside down T shape
		newPattern.coordinate_occupied.push_back({ 0, 0 });
		newPattern.coordinate_occupied.push_back({ 1, 0 });
		newPattern.coordinate_occupied.push_back({ -1, 0 });
		newPattern.coordinate_occupied.push_back({ 0, 1 });
		newPattern.file_path = "Assets\\Images\\T.png";
		patternList.push_back(newPattern);
		newPattern.coordinate_occupied.clear();

		//Straight line
		newPattern.coordinate_occupied.push_back({ 0, 0 });
		newPattern.coordinate_occupied.push_back({ 1, 0 });
		newPattern.coordinate_occupied.push_back({ 2, 0 });
		newPattern.coordinate_occupied.push_back({ 3, 0 });
		newPattern.file_path = "Assets\\Images\\I.png";
		patternList.push_back(newPattern);
		newPattern.coordinate_occupied.clear();

		//L Shape
		newPattern.coordinate_occupied.push_back({ 0, 0 });
		newPattern.coordinate_occupied.push_back({ 1, 0 });
		newPattern.coordinate_occupied.push_back({ 0, 1 });
		newPattern.coordinate_occupied.push_back({ 0, 2 });
		newPattern.file_path = "Assets\\Images\\L.png";
		patternList.push_back(newPattern);
		newPattern.coordinate_occupied.clear();
		
		//J Shape
		newPattern.coordinate_occupied.push_back({ 0, 0 });
		newPattern.coordinate_occupied.push_back({ -1, 0 });
		newPattern.coordinate_occupied.push_back({ 0, 1 });
		newPattern.coordinate_occupied.push_back({ 0, 2 });
		newPattern.file_path = "Assets\\Images\\J.png";
		patternList.push_back(newPattern);
		newPattern.coordinate_occupied.clear();

		//Box Shape
		newPattern.coordinate_occupied.push_back({ 0, 0 });
		newPattern.coordinate_occupied.push_back({ 1, 0 });
		newPattern.coordinate_occupied.push_back({ 1, 1 });
		newPattern.coordinate_occupied.push_back({ 0, 1 });
		newPattern.file_path = "Assets\\Images\\O.png";
		patternList.push_back(newPattern);
		newPattern.coordinate_occupied.clear();

		//S Shape
		newPattern.coordinate_occupied.push_back({ 0, 0 });
		newPattern.coordinate_occupied.push_back({ -1, 0 });
		newPattern.coordinate_occupied.push_back({ 0, 1 });
		newPattern.coordinate_occupied.push_back({ 1, 1 });
		newPattern.file_path = "Assets\\Images\\S.png";
		patternList.push_back(newPattern);
		newPattern.coordinate_occupied.clear();
		
		//Z Shape
		newPattern.coordinate_occupied.push_back({ 0, 0 });
		newPattern.coordinate_occupied.push_back({ 0, 1 });
		newPattern.coordinate_occupied.push_back({ -1, 1 });
		newPattern.coordinate_occupied.push_back({ 1, 0 });
		newPattern.file_path = "Assets\\Images\\Z.png";
		patternList.push_back(newPattern);
		newPattern.coordinate_occupied.clear();
	}

	//Initialize Abilities
	void InitializeAbilities(std::vector<Abilities>& abilitiesList) {
		//Clear abilities
		abilitiesList.clear();

		//Template for abilities
		Abilities new_ability;

		//Fixed damage
		new_ability.current_Ability_Name = ABILITY_NAME::HARD_DROP;
		new_ability.current_Ability_Type = ABILITY_TYPE::DIRECT;
		new_ability.damage = 15;
		abilitiesList.push_back(new_ability);

		//Life steal
		new_ability.current_Ability_Name = ABILITY_NAME::LIFE_DRAIN;
		new_ability.current_Ability_Type = ABILITY_TYPE::DIRECT;
		new_ability.damage = 12;
		abilitiesList.push_back(new_ability);

		//Construct Wall
		new_ability.current_Ability_Name = ABILITY_NAME::CONSTRUCT_WALL;
		new_ability.current_Ability_Type = ABILITY_TYPE::EFFECT;
		//TRIGGER THE BUILD WALL FUNCTION HERE!!
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
				if (!cell.m_isAccessible) {
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
				if (!cell.m_isAccessible) {
					continue;
				}

				//Set can walk to true
				cell.m_canWalk = true;

				//Sets the tile counter to 2 turns before it collapses
				cell.m_resetCounter = 3;

				Sprite& sprite = Coordinator::Instance()->GetComponent<Sprite>(cellEntity);
				sprite.id = AssetManager::Instance()->GetGuid(sprite_fileName);
			}
		}//End loop through pattern body check
	}	
	
	void ToggleCellWalkability([[maybe_unused]] Room& room, ECS::Entity cellEntity, b8 istrue) {
		Cell& cell = Coordinator::Instance()->GetComponent<Cell>(cellEntity);
		
		//if is completely block, dont need set
		if (!cell.m_isAccessible) {
			return;
		}

		//Set to toggle
		cell.m_canWalk = istrue;

		Sprite& sprite = Coordinator::Instance()->GetComponent<Sprite>(cellEntity);

		//Set sprite accordingly
		if (istrue) {
			sprite.id = AssetManager::Instance()->GetGuid("Assets/Images/Walkable.png");
		}
		else {
			sprite.id = AssetManager::Instance()->GetGuid("Assets/Images/InitialTile_v04.png");
		}
	}

	void PlaceWalkableOnGrid(Room& room, Vector2Int coordinate, std::string sprite_fileName) {
		ECS::Entity cellEntity = getEntityCell(room, coordinate.x, coordinate.y);

		Cell& cell = Coordinator::Instance()->GetComponent<Cell>(cellEntity);

		//if is completely block, dont need set
		if (!cell.m_isAccessible) {
			return;
		}

		//Set to canwalk
		cell.m_canWalk = true;

		Sprite& sprite = Coordinator::Instance()->GetComponent<Sprite>(cellEntity);
		sprite.id = AssetManager::Instance()->GetGuid(sprite_fileName);
	}

	b8 ALEngine::Engine::GameplayInterface::CheckIfWalkableOnGrid(Room& room, u32 gridX, u32 gridY)
	{
		ECS::Entity cellEntity = getEntityCell(room, gridX, gridY);

		Cell& cell = Coordinator::Instance()->GetComponent<Cell>(cellEntity);

		//if is completely block, dont need set
		if (!cell.m_isAccessible) {
			return false;
		}

		if (cell.m_canWalk == true)
		{
			return true;
		}

		return false;
	}

	bool CheckIfPatternCanBePlacedForTile(Room& room, Vector2Int coordinate, Pattern pattern) {
		//Shift through each grid that the pattern would be in relative to given coordinate
		for (int i = 0; i < pattern.coordinate_occupied.size(); ++i) {
			//If the coordinate is within the boundaries of the room
			if (IsCoordinateInsideRoom(room, coordinate.x + pattern.coordinate_occupied[i].x, coordinate.y + pattern.coordinate_occupied[i].y)) {
				//If inside room, set the cell color to yellow
				ECS::Entity cellEntity = getEntityCell(room, coordinate.x + pattern.coordinate_occupied[i].x, coordinate.y + pattern.coordinate_occupied[i].y);

				Cell& cell = Coordinator::Instance()->GetComponent<Cell>(cellEntity);
				if (!cell.m_isAccessible) {
					continue;
				}

				if (cell.m_canWalk) {
					return false;
				}
			}
		}//End loop through pattern body check

		return true;
	}

	bool CheckIfAbilitiesCanBePlacedForTile(Room& room, Vector2Int coordinate, Pattern pattern, Abilities abilities) {
		//Shift through each grid that the pattern would be in relative to given coordinate
		for (int i = 0; i < pattern.coordinate_occupied.size(); ++i) {
			//If the coordinate is within the boundaries of the room
			//must connect to player
			if (IsCoordinateInsideRoom(room, coordinate.x + pattern.coordinate_occupied[i].x, coordinate.y + pattern.coordinate_occupied[i].y)) {
				//If ability is direct type
				if (abilities.current_Ability_Type == ABILITY_TYPE::DIRECT) {
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
				//If it's effect type
				//Must connect to cell that is walkable but no on cells with units
				else {
					//If inside room, set the cell color to yellow
					ECS::Entity cellEntity = getEntityCell(room, coordinate.x + pattern.coordinate_occupied[i].x, coordinate.y + pattern.coordinate_occupied[i].y);

					Cell& cell = Coordinator::Instance()->GetComponent<Cell>(cellEntity);

					if (cell.hasUnit || !cell.m_isAccessible) {
						return false;
					} 
				} 
			} 
		} //End loop through pattern body check
		
		//If reach here and direct, means not touching player, false
		if (abilities.current_Ability_Type == ABILITY_TYPE::DIRECT) {
			return false;
		}
		//If reach here and effect, means has not touch unit or cell, true
		else {
			return true;
		}
	}

	void RunAbilities_OnCells(Room& room, Vector2Int coordinate, Pattern pattern, Abilities abilities) {
		AL_CORE_CRITICAL("USE ABILITY");

		//Shift through each grid that the pattern would be in relative to given coordinate
		for (int i = 0; i < pattern.coordinate_occupied.size(); ++i) {
			//If the coordinate is within the boundaries of the room
			if (IsCoordinateInsideRoom(room, coordinate.x + pattern.coordinate_occupied[i].x, coordinate.y + pattern.coordinate_occupied[i].y)) {
				//If inside room, set the cell color to yellow
				ECS::Entity cellEntity = getEntityCell(room, coordinate.x + pattern.coordinate_occupied[i].x, coordinate.y + pattern.coordinate_occupied[i].y);
				Cell& cell = Coordinator::Instance()->GetComponent<Cell>(cellEntity);
				
				switch (abilities.current_Ability_Type)
				{
					//IF DIRECT ABILITY, AFFECT UNITS ONLY
					case ABILITY_TYPE::DIRECT:
						if (cell.hasUnit) {
							AL_CORE_CRITICAL("HAS UNIT");

							Unit& unit = Coordinator::Instance()->GetComponent<Unit>(cell.unitEntity);
							if (unit.health <= 0) {
								continue;
							}

							u32 initialHealth = unit.health;

							//If unit is enemy
							if (unit.unitType == UNIT_TYPE::ENEMY) {
								//Check for ability name and run ability accordingly
								switch (abilities.current_Ability_Name)
								{
									case ABILITY_NAME::HARD_DROP:
										DoDamageToUnit(cell.unitEntity, abilities.damage);
										break;
									case ABILITY_NAME::LIFE_DRAIN:
									{
										DoDamageToUnit(cell.unitEntity, abilities.damage);

										//Life steal 
										ECS::Entity playerEntity = Coordinator::Instance()->GetEntityByTag("Player");
										Unit& playerUnit = Coordinator::Instance()->GetComponent<Unit>(playerEntity);

										u32 healthDrained = unit.health < 0 ? initialHealth : abilities.damage;

										playerUnit.health += healthDrained;

										if (playerUnit.health > playerUnit.maxHealth) {
											playerUnit.health = playerUnit.maxHealth;
										}

										AL_CORE_CRITICAL("Heal : " + std::to_string(healthDrained) + " to player, health before " + std::to_string(playerUnit.health - healthDrained) + ", health now " + std::to_string(playerUnit.health));
										break;
									}
								}
							}//End check if unit
						}
					break;

				case ABILITY_TYPE::EFFECT:
					//Check for ability name and run ability accordingly
					switch (abilities.current_Ability_Name)
					{
					case ABILITY_NAME::CONSTRUCT_WALL:
						constructWall(room, coordinate.x + pattern.coordinate_occupied[i].x, coordinate.y + pattern.coordinate_occupied[i].y, true);
						break;

					default:
						break;
					}
					break;
				} //End switch check
			}//End check if it's inside room
		}//End loop through pattern body check
	}

	void DoDamageToUnit(ECS::Entity unitEntity, s32 damage) {
		[[maybe_unused]]EntityData& unitData = Coordinator::Instance()->GetComponent<EntityData>(unitEntity);
		Unit& unit = Coordinator::Instance()->GetComponent<Unit>(unitEntity);

		AL_CORE_CRITICAL("Damage " + std::to_string(damage) + " to " + unitData.tag + " which has " + std::to_string(unit.health) + " health");

		//Do damage
		unit.health -= damage;

		AL_CORE_CRITICAL(unitData.tag + " now has " + std::to_string(unit.health) + " health");

		//If no health
		if (unit.health <= 0) {
			//Determinte type
			if (unit.unitType == UNIT_TYPE::PLAYER) {
				AL_CORE_INFO("Unit Died");
			}
			else {
				AL_CORE_INFO("Enemy Died");
			}

			Coordinator::Instance()->GetComponent<EntityData>(unitEntity).active = false;
			Coordinator::Instance()->GetComponent<EntityData>(unit.unit_Sprite_Entity).active = false;
			unit.health = 0;	//Limit to 0

			Cell& cell = Coordinator::Instance()->GetComponent<Cell>(unit.m_CurrentCell_Entity);
			cell.hasUnit = false;	//Free it's cell
		}
	}

	bool RunEnemyAdjacentAttack(Room& room, Unit& enemy) {
		//Check 4 adjacent
		//If player is beside, do damage to it
		if (GameplayInterface::IsCoordinateInsideRoom(room, enemy.coordinate[0] + 1, enemy.coordinate[1])) {
			Cell& cell = Coordinator::Instance()->GetComponent<Cell>(getEntityCell(room, enemy.coordinate[0] + 1, enemy.coordinate[1]));

			if (cell.hasUnit) {
				if (Coordinator::Instance()->GetComponent<Unit>(cell.unitEntity).unitType == UNIT_TYPE::PLAYER) {
					DoDamageToUnit(cell.unitEntity, enemy.minDamage);
					return true;
				}
			}
		}
		if (GameplayInterface::IsCoordinateInsideRoom(room, enemy.coordinate[0] - 1, enemy.coordinate[1])) {
			Cell& cell = Coordinator::Instance()->GetComponent<Cell>(getEntityCell(room, enemy.coordinate[0] - 1, enemy.coordinate[1]));

			if (cell.hasUnit) {
				if (Coordinator::Instance()->GetComponent<Unit>(cell.unitEntity).unitType == UNIT_TYPE::PLAYER) {
					DoDamageToUnit(cell.unitEntity, enemy.minDamage);
					return true;
				}
			}
		}
		AL_CORE_INFO("Checking UP");
		if (GameplayInterface::IsCoordinateInsideRoom(room, enemy.coordinate[0], enemy.coordinate[1] + 1)) {
			Cell& cell = Coordinator::Instance()->GetComponent<Cell>(getEntityCell(room, enemy.coordinate[0], enemy.coordinate[1] + 1));

			if (cell.hasUnit) {
				if (Coordinator::Instance()->GetComponent<Unit>(cell.unitEntity).unitType == UNIT_TYPE::PLAYER) {
					DoDamageToUnit(cell.unitEntity, enemy.minDamage);
					return true;
				}
			}
		}
		if (GameplayInterface::IsCoordinateInsideRoom(room, enemy.coordinate[0], enemy.coordinate[1] - 1)) {
			Cell& cell = Coordinator::Instance()->GetComponent<Cell>(getEntityCell(room, enemy.coordinate[0], enemy.coordinate[1] - 1));

			if (cell.hasUnit) {
				if (Coordinator::Instance()->GetComponent<Unit>(cell.unitEntity).unitType == UNIT_TYPE::PLAYER) {
					DoDamageToUnit(cell.unitEntity, enemy.minDamage);
					return true;
				}
			}
		}
		return false;
	}

	void constructWall(Room& currentRoom, u32 x, u32 y, b8 isTrue) {
		ECS::Entity cellEntity = getEntityCell(currentRoom, x, y);

		Cell& cell = Coordinator::Instance()->GetComponent<Cell>(cellEntity);
		cell.has_Wall = isTrue;
		cell.m_canWalk = !isTrue;
		cell.m_resetCounter = 2;

		Sprite& sprite = Coordinator::Instance()->GetComponent<Sprite>(cell.child_overlay);
		sprite.layer = 1000 - Coordinator::Instance()->GetComponent<Transform>(cellEntity).position.y;
		sprite.id = AssetManager::Instance()->GetGuid("Assets/Images/Wall.png"); // TO REPLACE WHEN A NEW SPRITE IS ADDED. CURRENTLY ITS TEMPORARY SPRITE CHANGE
		Coordinator::Instance()->GetComponent<EntityData>(cell.child_overlay).active = true; //TOGGLING FOR OVERLAY VISIBILITY
	}

	void destroyWall(Room& currentRoom, u32 x, u32 y, b8 isTrue) {

		Cell& cell = Coordinator::Instance()->GetComponent<Cell>(getEntityCell(currentRoom, x, y));
		cell.has_Wall = isTrue;
		cell.m_canWalk = !isTrue;

		Coordinator::Instance()->GetComponent<EntityData>(cell.child_overlay).active = false; //TOGGLING FOR OVERLAY VISIBILITY
	}

	void CreateAudioEntityMasterSource(void)
	{
		using namespace ECS;
		Entity en	   = Coordinator::Instance()->CreateEntity();
		EntityData& ed = Coordinator::Instance()->GetComponent<EntityData>(en);
		ed.tag = "Master Audio Source";

		AudioSource as;
		as.id = 0;

		Audio ad = AssetManager::Instance()->GetAudio(AssetManager::Instance()->GetGuid(AUDIO_BGM_1));
		as.list[as.id++] = ad;

		ad = AssetManager::Instance()->GetAudio(AssetManager::Instance()->GetGuid(AUDIO_GAMEPLAY_LOOP));
		as.list[as.id++] = ad;

		ad = AssetManager::Instance()->GetAudio(AssetManager::Instance()->GetGuid(AUDIO_SELECT_SKILL));
		as.list[as.id++] = ad;

		ad = AssetManager::Instance()->GetAudio(AssetManager::Instance()->GetGuid(AUDIO_PLAYER_HURT));
		as.list[as.id++] = ad;

		ad = AssetManager::Instance()->GetAudio(AssetManager::Instance()->GetGuid(AUDIO_HIT));
		as.list[as.id++] = ad;

		ad = AssetManager::Instance()->GetAudio(AssetManager::Instance()->GetGuid(AUDIO_ENEMY_HURT_1));
		as.list[as.id++] = ad;

		ad = AssetManager::Instance()->GetAudio(AssetManager::Instance()->GetGuid(AUDIO_CLICK_1));
		as.list[as.id++] = ad;

		ad = AssetManager::Instance()->GetAudio(AssetManager::Instance()->GetGuid(AUDIO_PLAYER_WALK_1));
		as.list[as.id++] = ad;

		Coordinator::Instance()->AddComponent(en, as);
	}
}
