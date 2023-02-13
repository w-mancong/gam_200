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
		ENEMY_TYPE03,   //2
		ENEMY_TYPE04    //3
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