///*!
//file:   GameplaySystem.h
//author:	Tan Zhen Xiong (30%)
//co-author:	Mohamed Zafir (20%)
//			Darrion Aw Wei Ting (20%)
//			Chan Jie Ming Stanley (20%)
//			Lucas Nguyen Thai Vinh (5%)
//			Wong Man Cong (5%)
//email:	t.zhenxiong@digipen.edu
//		m.zafir@digipen.edu
//		Weitingdarrion.aw@digipen.edu
//		c.jiemingstanley@digipen.edu
//		l.nguyen@digipen.edu
//		w.mancong@digipen.edu
//brief:	This file contains the function declarations for GameplaySystem.h
//
//		All content :copyright: 2022 DigiPen Institute of Technology Singapore. All rights reserved.
//*//*__________________________________________________________________________________*/

#ifndef	GAMEPLAY_SYSTEM_H
#define GAMEPLAY_SYSTEM_H

#include <ECS/Components/UniBehaviour.h>
#include <Engine/Gameplay_Interface.h>

namespace ALEngine::Script
{
	using GAMEPLAY_SYSTEM_INTERFACE_H::Room;
	static f32 constexpr const TILE_SIZE = 100.0f;
	
	/*!*********************************************************************************
		\brief Set the room_to_load for MapManager
	***********************************************************************************/
	void SetMap(u64 index);

	class GameplaySystem : public ECS::Component::UniBehaviour
	{
		/*!*********************************************************************************
			\brief Used to load in any resources, will only run once when new scene loads
		***********************************************************************************/
		void Load(ECS::Entity en);

		/*!*********************************************************************************
			\brief Used to initialise any values to it's default value
		***********************************************************************************/
		void Init(ECS::Entity en);

		/*!*********************************************************************************
			\brief Updates every frame
		***********************************************************************************/
		void Update(ECS::Entity en);

		/*!*********************************************************************************
			\brief This update will be used for physics related logic
		***********************************************************************************/
		void LateUpdate(ECS::Entity en);

		/*!*********************************************************************************
			\brief Whenever a scene ends, use this function to free any resources
		***********************************************************************************/
		void Free(ECS::Entity en);

		/*!*********************************************************************************
			\brief Use this function to unload any resourcecs before changing to the next level
		***********************************************************************************/
		void Unload(ECS::Entity en);

	public:
		/*!*********************************************************************************
			\brief
			Start GamePlay System to initialize variables for gameplay logic
		***********************************************************************************/
		void StartGameplaySystem();

		/*!*********************************************************************************
			\brief
			Update GamePlay System to update gameplay logic
		***********************************************************************************/
		void UpdateGameplaySystem();

		/*!*********************************************************************************
			\brief
			Exit GamePlay System to clean up
		***********************************************************************************/
		void ExitGameplaySystem();

		/*!*********************************************************************************
			\brief
			Draws the gameplay's grid and pathfinder
		***********************************************************************************/
		void DrawGameplaySystem();



		/*!*********************************************************************************
			\brief
			create player unit entity
		***********************************************************************************/
		void CreatePlayerUnit(ECS::Entity entity);

		/*!*********************************************************************************
		\brief
			Initializes the Room based on the .map file that is given
		\param [in]
			map_fp:		File Path to the .map file
		\return
			Returns true when filepath can be found,
			else returns false
		***********************************************************************************/
		bool InitializeRoom(std::string map_fp);

		/*!*********************************************************************************
		\brief
			End current turn
		***********************************************************************************/
		void EndTurn();

		/*!*********************************************************************************
		\brief
			EndTurn_Enemy, to fix the bug where the bug skips the setup phase
		***********************************************************************************/
		void EndTurn_Enemy();


		/*!*********************************************************************************
		\brief
			Clear the move order
		***********************************************************************************/
		void ClearMoveOrder();

		/*!*********************************************************************************
		\brief
			Get current cell the unit is moving to
		***********************************************************************************/
		ECS::Entity getCurrentEntityCell();
		void SetMoveOrder(std::vector<ECS::Entity> path);

		/*!*********************************************************************************
		\brief
			Select Pattern
		***********************************************************************************/
		void SelectPattern(Pattern pattern);

		/*!*********************************************************************************
		\brief
			Select Abilities
		***********************************************************************************/
		void SelectAbility(Abilities& ability);

		/*!*********************************************************************************
		\brief
			Move the step for move order (Take step forward in path)
		***********************************************************************************/
		bool StepUpModeOrderPath(MoveOrder& order);

		/*!*********************************************************************************
		\brief
			Get size of room
		***********************************************************************************/
		u32 getRoomSize();

		/*!*********************************************************************************
		\brief
			Run Game State (the update of the game states)
		***********************************************************************************/
		void RunGameState();

		/*!*********************************************************************************
		\brief
			Update unit sprite layer based on their y position
		***********************************************************************************/
		void UpdateUnitSpriteLayer();

		/*!*********************************************************************************
		\brief
			Set Move player entity to cell
		***********************************************************************************/
		void MovePlayerEntityToCell(ECS::Entity cellEntity);

		/*!*********************************************************************************
			\brief
				Run enemy decision and move the enemy accordingly
		***********************************************************************************/
		void MoveEnemy();

		/*!*********************************************************************************
		\brief
			Run update for moving units
		***********************************************************************************/
		void RunGameStateMoving();

		/*!*********************************************************************************
		\brief
			Place Player onto room
		***********************************************************************************/
		void PlaceNewPlayerInRoom(s32 x, s32 y);

		/*!*********************************************************************************
		\brief
			Initialize End turn button
		***********************************************************************************/
		void InitializeEndTurnButton();

		/*!*********************************************************************************
		\brief
			Update the unit information GUI when select the unit
		***********************************************************************************/

		// Pattern Stuff
		/*!*********************************************************************************
		\brief
			Randomizes the Pattern List
		***********************************************************************************/
		void RandomizePatternList(void);

		/*!*********************************************************************************
		\brief
			Global function to return the cell entity based on specified position
		***********************************************************************************/
		u32 getEntityCell(GAMEPLAY_SYSTEM_INTERFACE_H::Room& currentRoom, u32 x, u32 y);

		void ToggleCellAccessibility(GAMEPLAY_SYSTEM_INTERFACE_H::Room& currentRoom, u32 x, u32 y, b8 istrue);

		/*!*********************************************************************************
		\brief
			Global function to check if list contains cell
		***********************************************************************************/
		bool CheckListContainsCell(std::list<ECS::Cell*> cellList, ECS::Cell& cellSearchNode);

		/*!*********************************************************************************
		\brief
			Get if cell is inside room of cells
		***********************************************************************************/
		bool IsCoordinateInsideRoom(Room& currentRoom, u32 gridX, u32 gridY);

		/*!*********************************************************************************
		\brief
			Get if the coordinate of cell is Accessible
		***********************************************************************************/
		bool IsCoordinateCellAccessible(GAMEPLAY_SYSTEM_INTERFACE_H::Room& currentRoom, u32 gridX, u32 gridY);

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
		void DisplayFilterPlacementGrid(GAMEPLAY_SYSTEM_INTERFACE_H::Room& room, Math::Vector2Int coordinate, Pattern pattern, Color color = { 1.f,1.f,1.f,1.f });

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
		void PlacePatternOntoGrid(GAMEPLAY_SYSTEM_INTERFACE_H::Room& room, Math::Vector2Int coordinate, Pattern pattern, std::string sprite_fileName);

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
		void ToggleCellWalkability(GAMEPLAY_SYSTEM_INTERFACE_H::Room& room, ECS::Entity cellEntity, b8 istrue);

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
		void PlaceWalkableOnGrid(GAMEPLAY_SYSTEM_INTERFACE_H::Room& room, Math::Vector2Int coordinate, std::string sprite_fileName);

		/*!*********************************************************************************
		\brief (stanley)
			check if grid is walkable
		\param [in]
			room: gameplay room
		\param [in]
			coordinate: Coordinate to check if walkable is placed
		***********************************************************************************/
		b8 CheckIfWalkableOnGrid(GAMEPLAY_SYSTEM_INTERFACE_H::Room& room, u32 gridX, u32 gridY);

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
		bool CheckIfPatternCanBePlacedForTile(GAMEPLAY_SYSTEM_INTERFACE_H::Room& room, Math::Vector2Int coordinate, Pattern pattern);

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
		bool CheckIfAbilitiesCanBePlacedForTile(GAMEPLAY_SYSTEM_INTERFACE_H::Room& room, Math::Vector2Int coordinate, Pattern pattern, Abilities abilities);

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
		void RunAbilities_OnCells(GAMEPLAY_SYSTEM_INTERFACE_H::Room& room, Math::Vector2Int coordinate, Pattern pattern, Abilities* abilities);

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
			Constructs a wall onto a cell, makes the cell unwalkable in pathfinding
		\param [in]
			currentRoom: room maintained by the gameplay system
		\param [in]
			x: x coordinate
		\param [in]
			y: y coordinate
		\param [in]
			isTrue : Whether to build the wall
		***********************************************************************************/
		void constructWall(GAMEPLAY_SYSTEM_INTERFACE_H::Room& currentRoom, u32 x, u32 y, b8 isTrue);

		/*!*********************************************************************************
		\brief
			Constructs a trap, if enemy steps on it, take damage and get stunned for a few turns
		\param [in]
			currentRoom: room maintained by the gameplay system
		\param [in]
			x: x coordinate
		\param [in]
			y: y coordinate
		***********************************************************************************/
		void constructTrap(GAMEPLAY_SYSTEM_INTERFACE_H::Room& currentRoom, u32 x, u32 y);

		/*!*********************************************************************************
		\brief
			Constructs a bomb, will explode in the next action phase, damage all units in it's 8 adjacent cell as well as resetting the affected cells
		\param [in]
			currentRoom: room maintained by the gameplay system
		\param [in]
			x: x coordinate
		\param [in]
			y: y coordinate
		***********************************************************************************/
		void constructBomb(GAMEPLAY_SYSTEM_INTERFACE_H::Room& currentRoom, u32 x, u32 y);


		/*!*********************************************************************************
		\brief
			Resets a cell back default cell
		\param [in]
			currentRoom: room maintained by the gameplay system
		\param [in]
			x: x coordinate
		\param [in]
			y: y coordinate
		***********************************************************************************/
		void ResetCell(GAMEPLAY_SYSTEM_INTERFACE_H::Room& currentRoom, u32 x, u32 y);

		/*!*********************************************************************************
		\brief
			Destroys a wall on a cell
		\param [in]
			currentRoom: room maintained by the gameplay system
		\param [in]
			x: x coordinate
		\param [in]
			y: y coordinate
		\param [in]
			isTrue : Whether to destroy the wall
		***********************************************************************************/
		void destroyWall(GAMEPLAY_SYSTEM_INTERFACE_H::Room& currentRoom, u32 x, u32 y, b8 isTrue);

		/*!*********************************************************************************
		\brief
			Runs explode algorithm on a cell, will reset adjacent cell as well as damage unit on top of them
		\param [in]
			currentRoom: room maintained by the gameplay system
		\param [in]
			x: x coordinate
		\param [in]
			y: y coordinate
		***********************************************************************************/
		void Bomb_Explode(GAMEPLAY_SYSTEM_INTERFACE_H::Room& currentRoom, u32 x, u32 y);

		/*!*********************************************************************************
		\brief
			Set gameplay system to running or not
		***********************************************************************************/
		void Toggle_Gameplay_State(b8 istrue);


		/*!*********************************************************************************
		\brief
			Check the selected tile counters and to make
			amendments to them at the end of the turn
		***********************************************************************************/
		s32 checkTileCounters(ECS::Entity& selectedCell);

		/*!*********************************************************************************
		\brief
		 Function for setting EnemyManager object variables for references for
		 GameplayInterface_Management_Enemy functions
		***********************************************************************************/
		void EnemyManager_LoadData();

		/*!*********************************************************************************
		\brief
		 Rotates the current pattern to place
		***********************************************************************************/
		void RotatePattern(int patternRotationAmount);

		/*!*********************************************************************************
		\brief
		 Shift through each cell and decrease their counter, resets them if the counter is 0
		***********************************************************************************/
		void scanRoomCellArray();

		/*!*********************************************************************************
		\brief
		 Checks if the player is on cell, if yes then eliminate player
		***********************************************************************************/
		void checkPlayerPlacement();

		/*!*********************************************************************************
		\brief
		 Display path from player towards a cell
		***********************************************************************************/
		void DisplayPlayerEntityPathToCell(ECS::Entity cellEntity);

		/*!*********************************************************************************
		\brief
		 Hides the highlighted path.
		***********************************************************************************/
		void ClearHighlightPath();

		/*!*********************************************************************************
		\brief
		 Highlights walkable path within the player's cell
		***********************************************************************************/
		void HighlightWalkableCellsRange(Room& room, Math::Vector2Int coordinate, bool reachable, std::vector<ECS::Entity>& pathlist);


		/*!*********************************************************************************
		\brief
			Creates an audio manager
		***********************************************************************************/
		void CreateAudioEntityMasterSource(void);

		/*!*********************************************************************************
		\brief
		 Toggles god mode
		***********************************************************************************/
		void Cheat_ToggleGodMode();

		/*!*********************************************************************************
		\brief
		 Increase health by amount
		***********************************************************************************/
		void Cheat_IncreasePlayerHealth(s32 amount);

		/*!*********************************************************************************
		\brief
		 Increase Action Points by amount
		***********************************************************************************/
		void Cheat_IncreasePlayerActionPoint(s32 amount);

		/*!*********************************************************************************
		\brief
		 DOubles ability damage
		***********************************************************************************/
		void Cheat_ToggleDoubleAbilitiesDoubleDamage();
	
		/*!*********************************************************************************
		\brief
		 Decrease all enemy health to one
		***********************************************************************************/
		void Cheat_DecreaseEnemyHealthToOne();

		/*!*********************************************************************************
		\brief
		 Eliminate all enemy
		***********************************************************************************/
		void Cheat_EliminateAllEnemy();

		/*!*********************************************************************************
		\brief
		 Reset all alive enemy health to max
		***********************************************************************************/
		void Cheat_ResetAllEnemiesHealth();

		/*!*********************************************************************************
		\brief
		 Resets player health to max
		***********************************************************************************/
		void Cheat_ResetPlayerHealth();

		/*!*********************************************************************************
		\brief
		 Resets player action points to max
		***********************************************************************************/
		void Cheat_ResetPlayerActionPoints();

		/*!*********************************************************************************
		\brief
		 Clear all cell walkability
		***********************************************************************************/
		void Cheat_ClearFloorWalkability();

		/*!*********************************************************************************
		\brief
		 Play Audio
		***********************************************************************************/
		void PlayAudio(std::string audioName);

		/*!*********************************************************************************
			\brief
				Audio play player attack
		***********************************************************************************/
		void Audio_Play_PlayerAttack();

		/*!*********************************************************************************
			\brief
				Audio play player Death
		***********************************************************************************/
		void Audio_Play_PlayerDeath();


	public:
		GAMEPLAY_STATUS currentGameplayStatus = GAMEPLAY_STATUS::RUNNING;							//Keep track of gameplay status, running or stopped
		PHASE_STATUS currentPhaseStatus = PHASE_STATUS::PHASE_SETUP;								//Keep track of phase
		UNITS_CONTROL_STATUS currentUnitControlStatus = UNITS_CONTROL_STATUS::NOTHING;				//Keep track of status of unit control
		PATTERN_PLACEMENT_STATUS currentPatternPlacementStatus = PATTERN_PLACEMENT_STATUS::NOTHING;	//Keep track of what the pattern is being placed for
		static b8 selectedAbilities;
		static GAME_STATUS currentGameStatus;
		static u64 roomIndex;
		static constexpr const u64 maxRooms{ 3 };
	
		//******VARIABLES**********//
		u32 roomSize[2]{ 10, 10 };		//Size to initialize the room with
		GAMEPLAY_SYSTEM_INTERFACE_H::Room m_Room;					//Room COntainer

		//Keep track of what move order the unit has
		MoveOrder currentModeOrder;

		//Keep track of the parent of all the cells
		ECS::Entity m_Room_Parent_Entity;

		//Patterns
		std::vector<Pattern> pattern_Default, pattern_List;
		Pattern selected_Pattern{};
		u32 selected_Pattern_Index{ 0 };
		s32 selected_Pattern_Rotation{ 0 };

		//Abilities
		std::vector<Abilities> Abilities_List;
		Abilities* selected_Abilities;

		//Cell that the mouse is hovering over
		ECS::Entity current_Moused_Over_Cell;

		//Entities to keep track of
		ECS::Entity playerEntity, startCellEntity, targetCellEntity;
		ECS::Entity movingUnitEntity;

		//Enemy
		std::vector<ECS::Entity> enemyEntityList;

		//ECS::Entity masterAudioSource{ ECS::MAX_ENTITIES };

		//Tracks debug drawing for room
		b8 is_DebugDraw = false;

		//Engine::Audio const* buttonClickAudio{ nullptr };

		//Cheats
		b8 godMode = false, cheat_abilitiesDoubleDamage = false;
		
		// For RTTR
		void DeserializeComponent(ECS::Entity en)
		{
			ECS::AddLogicComponent<GameplaySystem>(en);
		};
		RTTR_ENABLE(ECS::Component::UniBehaviour)
	};
}
#endif