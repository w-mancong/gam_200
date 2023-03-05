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
enum class ABILITY_NAME { HARD_DROP, LIFE_DRAIN, CONSTRUCT_WALL, OVERHANG, MATRIX_TRAP, VOLATILE };

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
	ALEngine::ECS::Entity* roomCellsArray{ nullptr };

	//Size of room
	ALEngine::ECS::Entity width{}, height{};
	ALEngine::ECS::Entity roomSize{}; //width * height

	// Player Position
	u32 playerX{ 0 }, playerY{ 0 };
};

/*!*********************************************************************************
\brief
	Container for Pattern
***********************************************************************************/
struct Pattern {
	// File Path
	std::string file_path{};

	//list of grid occupied relative to where it will be placed, 0,0 will be the center. 0,1 will be 1 grid right. 
	std::vector<std::vector<ALEngine::Math::Vector2Int>> offsetGroup;
};

/*!*********************************************************************************
\brief
	Container for Abilities
***********************************************************************************/
class Abilities {
public:
	//Stats
	ALEngine::ECS::Entity current_Cooldown = 0, max_Cooldown = 2;
	ALEngine::ECS::Entity damage = 15;

	ABILITY_TYPE current_Ability_Type = ABILITY_TYPE::DIRECT;

	//Keep track of ability type
	ABILITY_NAME current_Ability_Name = ABILITY_NAME::HARD_DROP;
};	

/*!*********************************************************************************
\brief
	Container for a move order
***********************************************************************************/
struct MoveOrder {
	ALEngine::ECS::Entity entity;				//unit
	std::vector<ALEngine::ECS::Entity> path;	//path, list of cells
	u32 path_step = 1;			//step into the path
};
//***************** Containers End ************************//

namespace ALEngine::Script
{
	void Set_GameplayInterface_Enemy(void* enemyManagerPtr);
	void Set_GameplayInterface_GUI(void* GUIManagerPtr);
	void Set_GameplayInterface_GameplayManager(void* GUIManagerPtr);

	//***************** Events  ************************//
		/*!*********************************************************************************
	\brief
		Restart the level
	***********************************************************************************/
	void Event_Button_Restart(ECS::Entity invoker);

	/*!*********************************************************************************
	\brief
		Select Ability 0
	***********************************************************************************/
	void Event_Button_Select_Abilities_0(ECS::Entity invoker);

	/*!*********************************************************************************
	\brief
		Select Ability 1
	***********************************************************************************/
	void Event_Button_Select_Abilities_1(ECS::Entity invoker);

	/*!*********************************************************************************
	\brief
		Select Ability 2
	***********************************************************************************/
	void Event_Button_Select_Abilities_2(ECS::Entity invoker);

	/*!*********************************************************************************
	\brief
		Select Ability 3
	***********************************************************************************/
	void Event_Button_Select_CurrentPattern(ECS::Entity invoker);

	/*!*********************************************************************************
	\brief
		Select Ability 4
	***********************************************************************************/
	void Event_Button_Select_Pattern_1(ECS::Entity invoker);

	/*!*********************************************************************************
	\brief
		Select Ability 5
	***********************************************************************************/
	void Event_Button_Select_Pattern_2(ECS::Entity invoker);

	/*!*********************************************************************************
	\brief
		Select Ability 6
	***********************************************************************************/
	void Event_Button_Select_Pattern_3(ECS::Entity invoker);

	/*!*********************************************************************************
	\brief
		Event for GUI button end turn
	***********************************************************************************/
	void Event_Button_Select_EndTurn(ECS::Entity invoker);

	/*!*********************************************************************************
	\brief
		Event for when mouse click on cell
	***********************************************************************************/
	void Event_ClickCell(ECS::Entity invokerCell);

	/*!*********************************************************************************
	\brief
		Event for when mouse enter cell
	***********************************************************************************/
	void Event_MouseEnterCell(ECS::Entity invoker);


	/*!*********************************************************************************
	\brief
		Event for when  mouse exit cell
	***********************************************************************************/
	void Event_MouseExitCell(ECS::Entity invoker);


	/*!*********************************************************************************
	\brief
		Event for when mouse enter unit
	***********************************************************************************/
	void Event_MouseEnterUnit(ECS::Entity invoker);

	/*!*********************************************************************************
	\brief
		Event for when mouse exit unit
	***********************************************************************************/
	void Event_MouseExitUnit(ECS::Entity invoker);
	

	/*!*********************************************************************************
	\brief
		Event for when mouse exit unit
	***********************************************************************************/
	void Event_Unit_OnSelect(ECS::Entity invoker);

	/*!*********************************************************************************
	\brief
		Event for when mouse exit ability button
	***********************************************************************************/
	void Event_Button_Exit_Ability_GUI(ECS::Entity invoker);


	/*!*********************************************************************************
	\brief
		Event for when mouse enter ability button
	***********************************************************************************/
	void Event_Button_Enter_Ability_GUI(ECS::Entity invoker);

	//***************** Events End  ************************//
}
#endif