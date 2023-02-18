#ifndef	GAMEPLAY_SYSTEM_INTERFACE_H
#define GAMEPLAY_SYSTEM_INTERFACE_H
#include <pch.h>

//***************** Enums ************************//
/*!*********************************************************************************
\brief
	Status of game, STOP if game is not playing anymore
					RUNNING if game is still running
***********************************************************************************/
enum class GAMEPLAY_STATUS
{
	STOP,
	RUNNING
};

/*!*********************************************************************************
\brief
	State of phase of game
***********************************************************************************/
enum class PHASE_STATUS
{
	PHASE_SETUP,
	PHASE_ACTION,
	PHASE_ENEMY,
};

/*!*********************************************************************************
\brief
	State of controlling/movement of unit
***********************************************************************************/
enum class UNITS_CONTROL_STATUS {
	NOTHING,
	UNIT_MOVING,
	UNIT_ATTACKING
};

/*!*********************************************************************************
\brief
	State of placing pattern onto room
***********************************************************************************/
enum class PATTERN_PLACEMENT_STATUS {
	NOTHING,
	PLACING_FOR_TILE,
	PLACING_FOR_ABILITIES
};

/*!*********************************************************************************
\brief
	Ability names
***********************************************************************************/
enum class ABILITY_NAME { HARD_DROP, LIFE_DRAIN, CONSTRUCT_WALL };

/*!*********************************************************************************
\brief
	Distiguish ability by type for who they can affect and how they check for usability
***********************************************************************************/
enum class ABILITY_TYPE { DIRECT, EFFECT };
//***************** Enums End ************************//


//***************** Containers ************************//
/*!*********************************************************************************
\brief
	Container for Room
***********************************************************************************/
struct Room
{
	//Will contain array to the room's cell's entity
	u32* roomCellsArray{ nullptr };

	//Size of room
	u32 width{}, height{};
	u32 roomSize{}; //width * height
};

/*!*********************************************************************************
\brief
	Container for Pattern
***********************************************************************************/
struct Pattern {
	// File Path
	std::string file_path{};

	//list of grid occupied relative to where it will be placed, 0,0 will be the center. 0,1 will be 1 grid right. 
	std::vector<ALEngine::Math::Vector2Int> coordinate_occupied{}
	;
};

/*!*********************************************************************************
\brief
	Container for Abilities
***********************************************************************************/
class Abilities {
public:
	//Stats
	u32 current_Cooldown = 0, max_Cooldown = 2;
	s32 damage = 15;

	ABILITY_TYPE current_Ability_Type = ABILITY_TYPE::DIRECT;

	//Keep track of ability type
	ABILITY_NAME current_Ability_Name = ABILITY_NAME::HARD_DROP;
};
//***************** Containers End ************************//

#endif