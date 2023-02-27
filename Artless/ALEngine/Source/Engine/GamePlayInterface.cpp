/*!
file:	GamePlayInterface.cpp
author:	Tan Zhen Xiong (30%)
co-author:	Mohamed Zafir (20%)
			Darrion Aw Wei Ting (20%)
			Chan Jie Ming Stanley (20%)
			Lucas Nguyen Thai Vinh (5%)
			Wong Man Cong (5%)
email:	t.zhenxiong@digipen.edu
		m.zafir@digipen.edu
		Weitingdarrion.aw@digipen.edu
		c.jiemingstanley@digipen.edu
		l.nguyen@digipen.edu
		w.mancong@digipen.edu
brief:	This file contains the function definition for GamePlayInterface.cpp

		All content � 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/

#include "pch.h"
#include <Utility/AudioNames.h>
#include <Engine/GamePlayInterface.h>
#include <Engine/GameplayInterface_Management_GUI.h>

namespace ALEngine::Engine::GameplayInterface
{
	u32 getEntityCell(Room& currentRoom,u32 x, u32 y)
	{
		//Get required cell's entity

		u32 testValue = currentRoom.roomCellsArray[y * currentRoom.width + x];

		std::cout << testValue << std::endl;
		return currentRoom.roomCellsArray[y * currentRoom.width + x];
	}

	void ToggleCellAccessibility(Room& currentRoom, u32 x, u32 y, b8 istrue) {
		//Get cell component
		Cell& cell = Coordinator::Instance()->GetComponent<Cell>(getEntityCell(currentRoom, x, y));
		cell.m_isAccessible = istrue;	//set accessible

		//Set it's overlay
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
				if (!cell.m_isAccessible || cell.has_Wall) {
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
		//Get Cell Entity
		ECS::Entity cellEntity = getEntityCell(room, coordinate.x, coordinate.y);

		//Get Component
		Cell& cell = Coordinator::Instance()->GetComponent<Cell>(cellEntity);

		//if is completely block, dont need set
		if (!cell.m_isAccessible) {
			return;
		}

		//Set to canwalk
		cell.m_canWalk = true;
		cell.m_resetCounter = 2;

		//Change the cell sprite to filename sprite
		Sprite& sprite = Coordinator::Instance()->GetComponent<Sprite>(cellEntity);
		sprite.id = AssetManager::Instance()->GetGuid(sprite_fileName);
	}

	b8 ALEngine::Engine::GameplayInterface::CheckIfWalkableOnGrid(Room& room, u32 gridX, u32 gridY)
	{
		//Get Cell entity
		ECS::Entity cellEntity = getEntityCell(room, gridX, gridY);

		//Get Cell Component
		Cell& cell = Coordinator::Instance()->GetComponent<Cell>(cellEntity);

		//if is completely block, dont need set
		if (!cell.m_isAccessible) {
			return false;
		}

		//Return if it's walkable
		return cell.m_canWalk;
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

					if (cell.hasUnit) {
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

	// Scan the entire room array to check for the tile counters and to change the sprite to the correct state of the tile
	void GameplaySystem::scanRoomCellArray() {
		//Keep track of reset counter
		s32 resetCounter;
		//Scan through each cell in the roomCellArray for the individual cell in the roomArray
		for (s32 i = 0; i < static_cast<s32>(gameplaySystem->roomSize[0]); ++i) {
			for (s32 j = 0; j < static_cast<s32>(gameplaySystem->roomSize[1]); ++j) {
				//Get the cell index
				s32 cellIndex = i * gameplaySystem->roomSize[0] + j;
				Entity cellEntity = m_Room.roomCellsArray[cellIndex];

				//Get the cell component
				Cell& cell = Coordinator::Instance()->GetComponent<Cell>(cellEntity);
				resetCounter = checkTileCounters(cell);

				//If 1 then set to crack
				if (resetCounter == 1) {

					Sprite& sprite = Coordinator::Instance()->GetComponent<Sprite>(cellEntity);
					sprite.id = AssetManager::Instance()->GetGuid("Assets/Images/Cracked_Tile.png"); // TO REPLACE WHEN A NEW SPRITE IS ADDED. CURRENTLY ITS TEMPORARY SPRITE CHANGE

				}
				if (resetCounter == 0) {
					//If 0 then reset the cell sprite
					Sprite& sprite = Coordinator::Instance()->GetComponent<Sprite>(cellEntity);
					sprite.id = AssetManager::Instance()->GetGuid("Assets/Images/InitialTile_v04.png");

					//If it's wall then destroy the wall
					if (cell.has_Wall) {
						GameplayInterface::destroyWall(gameplaySystem->m_Room, cell.coordinate.x, cell.coordinate.y, false);
					}
				}
			}
		}
	}

	//Check the tile the player is currently on to check if the tile is supposed to be destroyed
	void GameplaySystem::checkPlayerPlacement() {
		//Get the components
		Unit& playerUnit = Coordinator::Instance()->GetComponent<Unit>(gameplaySystem->playerEntity);
		Entity cellEntity = playerUnit.m_CurrentCell_Entity;
		Cell& playerUnitCell = Coordinator::Instance()->GetComponent<Cell>(cellEntity);

		//If the cell is not walkable, do damage tot he player
		if (playerUnitCell.m_canWalk == false) {
			DoDamageToUnit(gameplaySystem->playerEntity, playerUnit.maxHealth + 1);
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

										Transform playerTrans = Coordinator::Instance()->GetComponent<Transform>(playerEntity);
										ECS::ParticleSystem::GetParticleSystem().UnitHealParticles(playerTrans.position);

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
		//Get the unit entity data
		[[maybe_unused]]EntityData& unitData = Coordinator::Instance()->GetComponent<EntityData>(unitEntity);
		
		//Get unit component
		Unit& unit = Coordinator::Instance()->GetComponent<Unit>(unitEntity);

		//Get Transform
		Transform& unitTrans = Coordinator::Instance()->GetComponent<Transform>(unitEntity);
		
		//Get Particle System
		ECS::ParticleSystem::GetParticleSystem().UnitDmgParticles(unitTrans.position);

		AL_CORE_CRITICAL("Damage " + std::to_string(damage) + " to " + unitData.tag + " which has " + std::to_string(unit.health) + " health");

		//Do damage
		unit.health -= damage;

		AL_CORE_CRITICAL(unitData.tag + " now has " + std::to_string(unit.health) + " health");
	
		//Get the master audio source
		ECS::Entity masterAudioSource = Coordinator::Instance()->GetEntityByTag("Master Audio Source");
		AudioSource& as = Coordinator::Instance()->GetComponent<AudioSource>(masterAudioSource);

		//Play hit sound accordingly
		if (unit.unitType == UNIT_TYPE::PLAYER) {
			Audio& ad = as.GetAudio(AUDIO_HIT);
			ad.m_Channel = Channel::SFX;
			ad.Play();
		}
		else {
			Audio& ad = as.GetAudio(AUDIO_ENEMY_HURT_1);
			ad.m_Channel = Channel::SFX;
			ad.Play();
		}

		//If no health
		if (unit.health <= 0) {
			//Determinte type
			if (unit.unitType == UNIT_TYPE::PLAYER) {
				AL_CORE_INFO("Unit Died");
				ECS::Entity LoseTextEntity = Coordinator::Instance()->GetEntityByTag("Win_Clear_Text");
				Coordinator::Instance()->GetComponent<Text>(LoseTextEntity).textString = "Player lost all health, press to try again";

				ECS::SetActive(true, GameplayInterface_Management_GUI::getGuiManager().Win_Clear);

				unitData.active = false;
				Coordinator::Instance()->GetComponent<EntityData>(unit.unit_Sprite_Entity).active = false;
			}
			else {
				//If enemy unit
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
			//RIGHT
			Cell& cell = Coordinator::Instance()->GetComponent<Cell>(getEntityCell(room, enemy.coordinate[0] + 1, enemy.coordinate[1]));

			//Cell has unit
			//Do damage to player
			if (cell.hasUnit) {
				if (Coordinator::Instance()->GetComponent<Unit>(cell.unitEntity).unitType == UNIT_TYPE::PLAYER) {
					DoDamageToUnit(cell.unitEntity, enemy.minDamage);
					return true;
				}
			}
		}
		if (GameplayInterface::IsCoordinateInsideRoom(room, enemy.coordinate[0] - 1, enemy.coordinate[1])) {
			//LEFT
			Cell& cell = Coordinator::Instance()->GetComponent<Cell>(getEntityCell(room, enemy.coordinate[0] - 1, enemy.coordinate[1]));

			//Cell has unit
			//Do damage to player
			if (cell.hasUnit) {
				if (Coordinator::Instance()->GetComponent<Unit>(cell.unitEntity).unitType == UNIT_TYPE::PLAYER) {
					DoDamageToUnit(cell.unitEntity, enemy.minDamage);
					return true;
				}
			}
		}
		if (GameplayInterface::IsCoordinateInsideRoom(room, enemy.coordinate[0], enemy.coordinate[1] + 1)) {
			//UP
			Cell& cell = Coordinator::Instance()->GetComponent<Cell>(getEntityCell(room, enemy.coordinate[0], enemy.coordinate[1] + 1));

			//Cell has unit
			//Do damage to player
			if (cell.hasUnit) {
				if (Coordinator::Instance()->GetComponent<Unit>(cell.unitEntity).unitType == UNIT_TYPE::PLAYER) {
					DoDamageToUnit(cell.unitEntity, enemy.minDamage);
					return true;
				}
			}
		}
		if (GameplayInterface::IsCoordinateInsideRoom(room, enemy.coordinate[0], enemy.coordinate[1] - 1)) {
			//DOWN
			Cell& cell = Coordinator::Instance()->GetComponent<Cell>(getEntityCell(room, enemy.coordinate[0], enemy.coordinate[1] - 1));

			//Cell has unit
			//Do damage to player
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
		//Get the cell entity
		ECS::Entity cellEntity = getEntityCell(currentRoom, x, y);

		//Get Cell Component
		Cell& cell = Coordinator::Instance()->GetComponent<Cell>(cellEntity);

		//IF it already has a wall, don't do anything
		if (cell.has_Wall || !cell.m_isAccessible) {
			return;
		}

		//Set stats
		cell.has_Wall = isTrue;
		cell.m_canWalk = !isTrue;
		cell.m_resetCounter = 2;

		//Change it's sprite overlay
		Sprite& sprite = Coordinator::Instance()->GetComponent<Sprite>(cell.child_overlay);
		sprite.layer = 1000 - static_cast<u32>(Coordinator::Instance()->GetComponent<Transform>(cellEntity).position.y);
		sprite.id = AssetManager::Instance()->GetGuid("Assets/Images/ConstructTile_TileSprite.png"); // TO REPLACE WHEN A NEW SPRITE IS ADDED. CURRENTLY ITS TEMPORARY SPRITE CHANGE
		Coordinator::Instance()->GetComponent<EntityData>(cell.child_overlay).active = true; //TOGGLING FOR OVERLAY VISIBILITY
	}

	void destroyWall(Room& currentRoom, u32 x, u32 y, b8 isTrue) {
		//Get cell component 
		Cell& cell = Coordinator::Instance()->GetComponent<Cell>(getEntityCell(currentRoom, x, y));
		
		//Set Stats
		cell.has_Wall = isTrue;
		cell.m_canWalk = false;

		//Set the overlay sprite to false
		Coordinator::Instance()->GetComponent<EntityData>(cell.child_overlay).active = false; //TOGGLING FOR OVERLAY VISIBILITY
	}

	void CreateAudioEntityMasterSource(void)
	{
		using namespace ECS;
		//Create Entity
		Entity en	   = Coordinator::Instance()->CreateEntity();
		
		//Get it's data
		EntityData& ed = Coordinator::Instance()->GetComponent<EntityData>(en);
		
		//Change name
		ed.tag = "Master Audio Source";

		//Prepare audiosource component
		AudioSource as;
		as.id = 0;

		//Add BGM
		Audio ad = AssetManager::Instance()->GetAudio(AssetManager::Instance()->GetGuid(AUDIO_BGM_1));
		as.list[as.id++] = ad;

		//Add Gameplay Loop
		ad = AssetManager::Instance()->GetAudio(AssetManager::Instance()->GetGuid(AUDIO_GAMEPLAY_LOOP));
		as.list[as.id++] = ad;

		//Add Select Skill
		ad = AssetManager::Instance()->GetAudio(AssetManager::Instance()->GetGuid(AUDIO_SELECT_SKILL));
		as.list[as.id++] = ad;

		//Add Player hurt
		ad = AssetManager::Instance()->GetAudio(AssetManager::Instance()->GetGuid(AUDIO_PLAYER_HURT));
		as.list[as.id++] = ad;

		//Add Enemy Hit
		ad = AssetManager::Instance()->GetAudio(AssetManager::Instance()->GetGuid(AUDIO_HIT));
		as.list[as.id++] = ad;

		//Add Enemy Hit 2
		ad = AssetManager::Instance()->GetAudio(AssetManager::Instance()->GetGuid(AUDIO_ENEMY_HURT_1));
		as.list[as.id++] = ad;

		//Add Audio Click (Button)
		ad = AssetManager::Instance()->GetAudio(AssetManager::Instance()->GetGuid(AUDIO_CLICK_1));
		as.list[as.id++] = ad;

		//Add Player Walk
		ad = AssetManager::Instance()->GetAudio(AssetManager::Instance()->GetGuid(AUDIO_PLAYER_WALK_1));
		as.list[as.id++] = ad;

		//Add the audiosource component to the entity
		Coordinator::Instance()->AddComponent(en, as);
	}
}
