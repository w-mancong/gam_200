/*!
file:   GamePlayInterface.h
author: Tan Zhen Xiong
email:  t.zhenxiong\@digipen.edu
brief:	This file contains the function definition for GamePlayInterface.h

		All content :copyright: 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/

#ifndef	GAMEPLAY_INTERFACE_H
#define GAMEPLAY_INTERFACE_H
namespace ALEngine::Engine::GameplayInterface
{
	#define base_Layer 1000.0f;		//Base layer

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
		Container for Pattern
	***********************************************************************************/
	struct Pattern {
		// File Path
		std::string file_path{};

		//list of grid occupied relative to where it will be placed, 0,0 will be the center. 0,1 will be 1 grid right. 
		std::vector<Vector2Int> coordinate_occupied{}
		;
	};

	//***************For now 2 abilities***************//
	enum class ABILITY_NAME { HARD_DROP, LIFE_DRAIN, CONSTRUCT_WALL};
	enum class ABILITY_TYPE { DIRECT, EFFECT };

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

	/*!*********************************************************************************
	\brief
		Global function to return the cell entity based on specified position 
    ***********************************************************************************/
	u32 getEntityCell(Room& currentRoom, u32 x, u32 y);

	void ToggleCellAccessibility(Room& currentRoom, u32 x, u32 y, b8 istrue);

	/*!*********************************************************************************
	\brief
		Global function to check if list contains cell 
    ***********************************************************************************/
	bool CheckListContainsCell(std::list<ECS::Cell*> cellList, ECS::Cell& cellSearchNode);

	/*!*********************************************************************************
	\brief
		Get if cell is inside room of cells
    ***********************************************************************************/
	bool IsCoordinateInsideRoom(Engine::GameplayInterface::Room& currentRoom, u32 gridX, u32 gridY);

	/*!*********************************************************************************
	\brief
		Get if the coordinate of cell is Accessible
	***********************************************************************************/
	bool IsCoordinateCellAccessible(Engine::GameplayInterface::Room& currentRoom, u32 gridX, u32 gridY);

	/*!*********************************************************************************
	\brief
		Initialize Patterns
	***********************************************************************************/
	void InitializePatterns(std::vector<Pattern>& patternList);

	/*!*********************************************************************************
	\brief
		Initialize Abilities
	***********************************************************************************/
	void InitializeAbilities(std::vector<Abilities>& abilitiesList);

	/*!*********************************************************************************
	\brief
		Filter Grids from Hovering Over cell during pattern select
	***********************************************************************************/
	void DisplayFilterPlacementGrid(Room& room, Vector2Int coordinate, Pattern pattern, Color color = { 1.f,1.f,1.f,1.f });

	/*!*********************************************************************************
	\brief
		Place pattern onto grid
	\param [in]
		room: gameplay room
	\param [in]
		coordinate: Coordinate to switch to walkable
	\param [in]
		pattern: Range, pattern
	\param [in]
		sprite_fileName: sprite to replace the cell of
	***********************************************************************************/
	void PlacePatternOntoGrid(Room& room, Vector2Int coordinate, Pattern pattern, std::string sprite_fileName);

	/*!*********************************************************************************
	\brief
		Disable pattern onto grid
	\param [in]
		room: gameplay room
	\param [in]
		coordinate: Coordinate to switch to walkable
	\param [in]
		pattern: Range, pattern
	\param [in]
		sprite_fileName: sprite to replace the cell of
	***********************************************************************************/
	void ToggleCellWalkability(Room& room, ECS::Entity cellEntity, b8 istrue);

	/*!*********************************************************************************
	\brief
		Place pattern onto grid, all affected grid will be changed to walkable
	\param [in]
		room: gameplay room
	\param [in]
		coordinate: Coordinate to switch to walkable
	\param [in]
		sprite_fileName: sprite to replace the cell of
	***********************************************************************************/
	void PlaceWalkableOnGrid(Room& room, Vector2Int coordinate, std::string sprite_fileName);

	/*!*********************************************************************************
	\brief
		check if grid is walkable
	\param [in]
		room: gameplay room
	\param [in]
		coordinate: Coordinate to check if walkable is placed
	***********************************************************************************/
	b8 CheckIfWalkableOnGrid(Room& room, u32 gridX, u32 gridY);

	/*!*********************************************************************************
	\brief
		Return if pattern can be placed on cell with given coordinate
	\param [in]
		room: gameplay room
	\param [in]
		coordinate: Coordinate to check if pattern can be placed on
	\param [in]
		pattern: Range, pattern
	\return
		if pattern can be placed on tile
***********************************************************************************/
	bool CheckIfPatternCanBePlacedForTile(Room& room, Vector2Int coordinate, Pattern pattern);

	/*!*********************************************************************************
	\brief
		Return if ability can be used on cell with given coordinate
	\param [in]
		room: gameplay room
	\param [in]
		coordinate: Coordinate to check the ability to run on
	\param [in]
		pattern: Range, pattern
	\return
		if ability can be used on tile
	***********************************************************************************/
	bool CheckIfAbilitiesCanBePlacedForTile(Room& room, Vector2Int coordinate, Pattern pattern, Abilities abilities);

	/*!*********************************************************************************
	\brief
		Check ability type and run abilities accordingly on cell
	\param [in]
		room: gameplay room
	\param [in]
		coordinate: Coordinate to run ability on
	\param [in]
		pattern: Range, pattern
	\param [in]
		abilities: ability
	***********************************************************************************/
	void RunAbilities_OnCells(Room& room, Vector2Int coordinate, Pattern pattern, Abilities abilities);

	/*!*********************************************************************************
	\brief
		Deal damage to a unit entity
	\param [in]
		unitEntity: unit entity
	\param [in]
		damage: damage amount
	***********************************************************************************/
	void DoDamageToUnit(ECS::Entity unitEntity, s32 damage);

	/*!*********************************************************************************
	\brief
		Runs process to check adjacent for player and attack if they are adjacent.
	\param [in] 
		room: room of gameplay
	\param [in] 
		room: enemy entity
	\return 
		if enemy attacked player and player is adjacent
	***********************************************************************************/
	bool RunEnemyAdjacentAttack(Room& room, Unit& enemy);


	void constructWall(Room& currentRoom, u32 x, u32 y, b8 isTrue);

	void destroyWall(Room& currentRoom, u32 x, u32 y, b8 isTrue);
	void CreateAudioEntityMasterSource(void);
}
#endif
