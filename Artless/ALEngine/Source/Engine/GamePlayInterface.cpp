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
		std::vector<Vector2Int> offset;
		
		//****** T SHAPE START *****//
		newPattern.file_path = "Assets\\Images\\T.png";

		//Upside down T shape (Ʇ)
		offset.push_back({ 0, 0 });
		offset.push_back({ 1, 0 });
		offset.push_back({ -1, 0 });
		offset.push_back({ 0, 1 });
		newPattern.offsetGroup.push_back(offset);
		offset.clear();

		//rightside T shape (|-)
		offset.push_back({ 0, 0 });
		offset.push_back({ 0, 1 });
		offset.push_back({ 0, -1 });
		offset.push_back({ 1, 0 });
		newPattern.offsetGroup.push_back(offset);
		offset.clear();

		//downside T shape (T)
		offset.push_back({ 0, 0 });
		offset.push_back({ -1, 0 });
		offset.push_back({ 1, 0 });
		offset.push_back({ 0, -1 });
		newPattern.offsetGroup.push_back(offset);
		offset.clear();

		//leftside T shape (-|)s
		offset.push_back({ 0, 0 });
		offset.push_back({ 0, 1 });
		offset.push_back({ 0, -1 });
		offset.push_back({ -1, 0 });
		newPattern.offsetGroup.push_back(offset);
		offset.clear();

		patternList.push_back(newPattern);
		//****** T SHAPE END *****//

		//****** LINE SHAPE START *****//
		newPattern = Pattern{};
		newPattern.file_path = "Assets\\Images\\I.png";

		//Horizontal Straight line(---)
		offset.push_back({ 0, 0 });
		offset.push_back({ 1, 0 });
		offset.push_back({ 2, 0 });
		offset.push_back({ 3, 0 });
		newPattern.offsetGroup.push_back(offset);
		offset.clear();

		//Vertical Straight line(|)
		offset.push_back({ 0, 0 });
		offset.push_back({ 0, 1 });
		offset.push_back({ 0, 2 });
		offset.push_back({ 0, 3 });
		newPattern.offsetGroup.push_back(offset);
		offset.clear();

		patternList.push_back(newPattern);
		//****** LINE SHAPE END *****//
		
		//****** L SHAPE START*****//
		newPattern = Pattern{};
		newPattern.file_path = "Assets\\Images\\L.png";

		//L Shape (L)
		offset.push_back({ 0, 0 });
		offset.push_back({ 1, 0 });
		offset.push_back({ 0, 1 });
		offset.push_back({ 0, 2 });
		newPattern.offsetGroup.push_back(offset);
		offset.clear();

		//90° clockwise rotation L Shape (Г)
		offset.push_back({ 0, 0 });
		offset.push_back({ 0, -1 });
		offset.push_back({ 1, 0 });
		offset.push_back({ 2, 0 });
		newPattern.offsetGroup.push_back(offset);
		offset.clear();

		//180° clockwise rotation L Shape (ꓶ)
		offset.push_back({ 0, 0 });
		offset.push_back({ -1, 0 });
		offset.push_back({ 0, -1 });
		offset.push_back({ 0, -2 });
		newPattern.offsetGroup.push_back(offset);
		offset.clear();
		
		//270° clockwise rotation L Shape (⅃)
		offset.push_back({ 0, 0 });
		offset.push_back({ -1, 0 });
		offset.push_back({ 0, -1 });
		offset.push_back({ 0, -2 });
		newPattern.offsetGroup.push_back(offset);
		offset.clear();
		

		patternList.push_back(newPattern);

		//****** L SHAPE END *****//

		//****** J SHAPE START*****//
		newPattern = Pattern{};
		newPattern.file_path = "Assets\\Images\\J.png";

		//J Shape (⅃)
		offset.push_back({ 0, 0 });
		offset.push_back({ -1, 0 });
		offset.push_back({ 0, 1 });
		offset.push_back({ 0, 2 });
		newPattern.offsetGroup.push_back(offset);
		offset.clear();

		//90° clockwise Shape (L)
		offset.push_back({ 0, 0 });
		offset.push_back({ 0, 1 });
		offset.push_back({ 1, 0 });
		offset.push_back({ 2, 0 });
		newPattern.offsetGroup.push_back(offset);
		offset.clear();

		//180° clockwise Shape (Г)
		offset.push_back({ 0, 0 });
		offset.push_back({ 1, 0 });
		offset.push_back({ 0, -1 });
		offset.push_back({ 0, -2});
		newPattern.offsetGroup.push_back(offset);
		offset.clear();

		//270° clockwise Shape (ꓶ)
		offset.push_back({ 0, 0 });
		offset.push_back({ 0, -1 });
		offset.push_back({ -1, 0 });
		offset.push_back({ -2, 0});
		newPattern.offsetGroup.push_back(offset);
		offset.clear();

		patternList.push_back(newPattern);
		//****** J SHAPE END *****//

		//****** BOX SHAPE START*****//
		newPattern = Pattern{};
		newPattern.file_path = "Assets\\Images\\O.png";

		//Box Shape (☐)
		offset.push_back({ 0, 0 });
		offset.push_back({ 1, 0 });
		offset.push_back({ 1, 1 });
		offset.push_back({ 0, 1 });
		newPattern.offsetGroup.push_back(offset);
		offset.clear();

		patternList.push_back(newPattern);
		//****** BOX SHAPE END *****//

		//****** S SHAPE START*****//
		newPattern = Pattern{};
		newPattern.file_path = "Assets\\Images\\S.png";

		//S Shape (S)
		offset.push_back({ 0, 0 });
		offset.push_back({ 1, 0 });
		offset.push_back({ 0, -1 });
		offset.push_back({ -1, -1 });
		newPattern.offsetGroup.push_back(offset);
		offset.clear();

		//S Shape ([
		//  	    ])
		offset.push_back({ 0, 0 });
		offset.push_back({ 0, -1 });
		offset.push_back({ -1, 0 });
		offset.push_back({ -1,	1 });
		newPattern.offsetGroup.push_back(offset);
		offset.clear();

		//S Shape (S) (0,0 point moved down)
		offset.push_back({ 0, 0 });
		offset.push_back({ 0, -1 });
		offset.push_back({ 1, 0 });
		offset.push_back({ 1, 1 });
		newPattern.offsetGroup.push_back(offset);
		offset.clear();

		//S Shape ([
		//  	    ]) (0,0 point moved left)
		offset.push_back({ 0, 0 });
		offset.push_back({ 1, 0 });
		offset.push_back({ 0, 1 });
		offset.push_back({ 1, -1 });
		newPattern.offsetGroup.push_back(offset);
		offset.clear();

		patternList.push_back(newPattern);
		//****** S SHAPE END *****//

		//****** Z SHAPE START*****//
		newPattern = Pattern{};
		newPattern.file_path = "Assets\\Images\\Z.png";
		
		//Z Shape (Z)
		offset.push_back({ 0, 0 });
		offset.push_back({ 0, -1 });
		offset.push_back({ -1, 0 });
		offset.push_back({ 1, -1 });
		newPattern.offsetGroup.push_back(offset);
		offset.clear();

		//Z Shape (N)
		offset.push_back({ 0, 0 });
		offset.push_back({ 1, 0 });
		offset.push_back({ 0, -1 });
		offset.push_back({ -1, -1 });
		newPattern.offsetGroup.push_back(offset);
		offset.clear();

		//Z Shape (Z) (0,0 point moved down)
		offset.push_back({ 0, 0 });
		offset.push_back({ 1, 0 });
		offset.push_back({ 0, 1 });
		offset.push_back({ -1, 1 });
		newPattern.offsetGroup.push_back(offset);
		offset.clear();

		//Z Shape (N) (0,0 point moved left)
		offset.push_back({ 0, 0 });
		offset.push_back({ 0, -1 });
		offset.push_back({ 1, 0 });
		offset.push_back({ 1, 1 });
		newPattern.offsetGroup.push_back(offset);
		offset.clear();

		patternList.push_back(newPattern);
		//****** Z SHAPE END *****//

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
		new_ability.max_Cooldown = 1;
		abilitiesList.push_back(new_ability);

		//Life steal
		new_ability.current_Ability_Name = ABILITY_NAME::LIFE_DRAIN;
		new_ability.current_Ability_Type = ABILITY_TYPE::DIRECT;
		new_ability.damage = 12;
		new_ability.max_Cooldown = 2;

		abilitiesList.push_back(new_ability);

		//Construct Wall
		new_ability.current_Ability_Name = ABILITY_NAME::CONSTRUCT_WALL;
		new_ability.current_Ability_Type = ABILITY_TYPE::EFFECT;
		new_ability.max_Cooldown = 4;
		//TRIGGER THE BUILD WALL FUNCTION HERE!!
		abilitiesList.push_back(new_ability);
	}

	void DisplayFilterPlacementGrid(Room& room, Vector2Int coordinate, Pattern pattern, Color color, int rotationIndex) {
		std::cout << " PATTERN OFFSET SIZE : " << pattern.offsetGroup.size() << std::endl;
		std::cout << "ROTATION INDEX : " << rotationIndex << std::endl;
		//Shift through each grid that the pattern would be in relative to given coordinate
		for (int i = 0; i < pattern.offsetGroup[rotationIndex].size(); ++i) {
			//If the coordinate is within the boundaries of the room
			if (IsCoordinateInsideRoom(room, coordinate.x + pattern.offsetGroup[rotationIndex][i].x, coordinate.y + pattern.offsetGroup[rotationIndex][i].y)) {
				//If inside room, set the cell color to yellow
				ECS::Entity cellEntity = getEntityCell(room, coordinate.x + pattern.offsetGroup[rotationIndex][i].x, coordinate.y + pattern.offsetGroup[rotationIndex][i].y);

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

	void PlacePatternOntoGrid(Room& room, Vector2Int coordinate, Pattern pattern, std::string sprite_fileName, int rotationIndex) {
		//Shift through each grid that the pattern would be in relative to given coordinate
		for (int i = 0; i < pattern.offsetGroup[rotationIndex].size(); ++i) {
			//If the coordinate is within the boundaries of the room
			if (IsCoordinateInsideRoom(room, coordinate.x + pattern.offsetGroup[rotationIndex][i].x, coordinate.y + pattern.offsetGroup[rotationIndex][i].y)) {
				//If inside room, set the cell color to yellow
				ECS::Entity cellEntity = getEntityCell(room, coordinate.x + pattern.offsetGroup[rotationIndex][i].x, coordinate.y + pattern.offsetGroup[rotationIndex][i].y);

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

	bool CheckIfPatternCanBePlacedForTile(Room& room, Vector2Int coordinate, Pattern pattern, int rotationIndex) {
		//Shift through each grid that the pattern would be in relative to given coordinate
		for (int i = 0; i < pattern.offsetGroup[rotationIndex].size(); ++i) {
			//If the coordinate is within the boundaries of the room
			if (IsCoordinateInsideRoom(room, coordinate.x + pattern.offsetGroup[rotationIndex][i].x, coordinate.y + pattern.offsetGroup[rotationIndex][i].y)) {
				//If inside room, set the cell color to yellow
				ECS::Entity cellEntity = getEntityCell(room, coordinate.x + pattern.offsetGroup[rotationIndex][i].x, coordinate.y + pattern.offsetGroup[rotationIndex][i].y);

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

	bool CheckIfAbilitiesCanBePlacedForTile(Room& room, Vector2Int coordinate, Pattern pattern, Abilities abilities, int rotationIndex) {
		//Shift through each grid that the pattern would be in relative to given coordinate
		for (int i = 0; i < pattern.offsetGroup[rotationIndex].size(); ++i) {
			//If the coordinate is within the boundaries of the room
			//must connect to player
			if (IsCoordinateInsideRoom(room, coordinate.x + pattern.offsetGroup[rotationIndex][i].x, coordinate.y + pattern.offsetGroup[rotationIndex][i].y)) {
				//If ability is direct type
				if (abilities.current_Ability_Type == ABILITY_TYPE::DIRECT) {
					//If inside room, set the cell color to yellow
					ECS::Entity cellEntity = getEntityCell(room, coordinate.x + pattern.offsetGroup[rotationIndex][i].x, coordinate.y + pattern.offsetGroup[rotationIndex][i].y);

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
					ECS::Entity cellEntity = getEntityCell(room, coordinate.x + pattern.offsetGroup[rotationIndex][i].x, coordinate.y + pattern.offsetGroup[rotationIndex][i].y);

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

	void RunAbilities_OnCells(Room& room, Vector2Int coordinate, Pattern pattern, Abilities &abilities, int rotationIndex) {
		AL_CORE_CRITICAL("USE ABILITY");

		//Shift through each grid that the pattern would be in relative to given coordinate
		for (int i = 0; i < pattern.offsetGroup[rotationIndex].size(); ++i) {
			//If the coordinate is within the boundaries of the room
			if (IsCoordinateInsideRoom(room, coordinate.x + pattern.offsetGroup[rotationIndex][i].x, coordinate.y + pattern.offsetGroup[rotationIndex][i].y)) {
				//If inside room, set the cell color to yellow
				ECS::Entity cellEntity = getEntityCell(room, coordinate.x + pattern.offsetGroup[rotationIndex][i].x, coordinate.y + pattern.offsetGroup[rotationIndex][i].y);
				Cell& cell = Coordinator::Instance()->GetComponent<Cell>(cellEntity);
				
				switch (abilities.current_Ability_Type)
				{
					//IF DIRECT ABILITY, AFFECT UNITS ONLY
					case ABILITY_TYPE::DIRECT:
						if (cell.hasUnit) {
							void reduceCooldown();

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
										abilities.current_Cooldown = abilities.max_Cooldown;
										
										AL_CORE_CRITICAL("HARD DROPPED USED. CURRENT CD :");
										AL_CORE_CRITICAL(abilities.current_Cooldown);

										break;
									case ABILITY_NAME::LIFE_DRAIN:
									{
										DoDamageToUnit(cell.unitEntity, abilities.damage);
										abilities.current_Cooldown = abilities.max_Cooldown;
										AL_CORE_CRITICAL("LIFE DRAIN USED. CURRENT CD :");
										AL_CORE_CRITICAL(abilities.current_Cooldown);

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
						constructWall(room, coordinate.x + pattern.offsetGroup[rotationIndex][i].x, coordinate.y + pattern.offsetGroup[rotationIndex][i].y, true);
						abilities.current_Cooldown = abilities.max_Cooldown;
						AL_CORE_CRITICAL("CONSTRUCT WALL USED.CURRENT CD : ");
						AL_CORE_CRITICAL(abilities.current_Cooldown);

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

	void reduceCooldown(std::vector<Abilities> &Abilities_List)
	{

		AL_CORE_CRITICAL("TURN ENDED, REDUCE COOLDOWN");
		std::cout << "Abilities_List SIZE : " << Abilities_List.size() << std::endl;

		for (int i = 0; i < Abilities_List.size(); i++) {

			if (Abilities_List[i].current_Cooldown > 0) {
				Abilities_List[i].current_Cooldown--;
			}

			//if (Abilities_List[i].current_Ability_Name == ABILITY_NAME::HARD_DROP) {
			//	std::cout << "HARD DROP ABILITY. I = " << i << std::endl;
			//	std::cout << "HARD DROP ORIGINAL COOLDOWN = " << Abilities_List[i].current_Cooldown << std::endl;
			//	Abilities_List[i].current_Cooldown--;
			//	std::cout << "HARD DROP CURRENT COOLDOWN = " << Abilities_List[i].current_Cooldown << std::endl;
			//}
			//else if (Abilities_List[i].current_Ability_Name == ABILITY_NAME::LIFE_DRAIN) {
			//	std::cout << "LIFE DRAIN ABILITY I = " << i << std::endl;
			//	std::cout << "LIFE DRAIN ORIGINAL COOLDOWN = " << Abilities_List[i].current_Cooldown << std::endl;
			//	Abilities_List[i].current_Cooldown--;
			//	std::cout << "LIFE DRAIN CURRENT COOLDOWN = " << Abilities_List[i].current_Cooldown << std::endl;
			//}
			//else if (Abilities_List[i].current_Ability_Name == ABILITY_NAME::CONSTRUCT_WALL) {
			//	std::cout << "CONSTRUCT ABILITY I = " << i << std::endl;
			//	std::cout << "CONSTRUCT WALL ORIGINAL COOLDOWN = " << Abilities_List[i].current_Cooldown << std::endl;
			//	Abilities_List[i].current_Cooldown--;
			//	std::cout << "CONSTRUCT WALL CURRENT COOLDOWN = " << Abilities_List[i].current_Cooldown << std::endl;
			//}

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
