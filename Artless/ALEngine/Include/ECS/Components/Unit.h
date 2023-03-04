/*!
file:   Unit.h
author: Tan Zhen Xiong
email:  t.zhenxiong\@digipen.edu
brief:  This file contains the function declarations for Collider.h

		All content :copyright: 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/

#ifndef	UNIT_H
#define UNIT_H

namespace ALEngine::ECS::Component
{
	/*!*********************************************************************************
	\brief
		Enum used for differentiating the different type of Shape to be created
	***********************************************************************************/
	enum class UNIT_TYPE
	{
		PLAYER,
		ENEMY,
	};
	
	enum class ENEMY_TYPE
	{
		ENEMY_MELEE,   //0 Enemy Melee
		ENEMY_CELL_DESTROYER,   //1 Cell Destroyer
		ENEMY_SUMMONER,   //2
		ENEMY_TYPE04    //3
	};

	enum class SUMMONER_ENEMY_STATE
	{
		//list of all spawnerenemystate
		SES_START = 0, //start and decision state
		SES_MOVE,      //move to player state once triggered
		SES_SUMMON,    //summon enemy state
		SES_RETREAT,   //retreat from player state
		SES_HEAL,      //heal itself state
		SES_DEAD       //enemy died state 
	};

	/*!*********************************************************************************
		\brief
			2D Circle Collider for collision detection
	***********************************************************************************/
	struct Unit
	{
		//Name of unit
		std::string unit_Name{};

		//File to profile of unit
		std::string unit_Profile_Sprite_File{};

		//Stats
		s32 health{}, maxHealth{};
		s32 defense{};
		s32 minDamage, maxDamage{};
		s32 movementPoints{}, maxMovementPoints{};
		s32 minRange{}, maxRange{};

		//spawner enemy state stuff
		//current state
		SUMMONER_ENEMY_STATE m_CurrentStateId{};
		//next state
		SUMMONER_ENEMY_STATE m_NextStateId{};
		//previous state
		SUMMONER_ENEMY_STATE m_PreviousStateId{};
		//turncounte to keep track
		s32 TurnCounter{};
		//keep track that turn ended
		bool TurnEnded{};

		//bool to trigger when player in specified range of enemy
		bool TriggeredByPlayer;

		//unit type
		UNIT_TYPE unitType{};
		ENEMY_TYPE enemyUnitType{};
		//int coords, 0 is x, 1 is y
		u32 coordinate[2]{};

		//Entity that acts as the unit's sprite
		ECS::Entity unit_Sprite_Entity{};
		
		//Current cell unit is on
		ECS::Entity m_CurrentCell_Entity{};

		void(*UpdateEnemyLogic)(struct EnemyManager&, std::vector<Entity>&, struct Room&) {};
	};
}
#endif