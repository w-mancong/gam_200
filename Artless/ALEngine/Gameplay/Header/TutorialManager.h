/*!
file:	TutorialManager.h
author: Lucas Nguyen
email:	l.nguyen\@digipen.edu
brief:	Header file that contains declarations for functions needed for the Tutorial

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#ifndef TUTORIAL_MANAGER_H
#define TUTORIAL_MANAGER_H

namespace ALEngine::Script
{
	class GameplaySystem;
}

namespace Gameplay
{
	// Forward Declaration
	enum class TutorialState : u32;


	class TutorialManager : public ALEngine::Templates::Singleton<TutorialManager>
	{
	public:
		// Variables
		std::vector<ALEngine::ECS::Entity> m_AbilityList{};

		/*!*********************************************************************************
			\brief
				Begings the Tutorial
		***********************************************************************************/
		void BeginTutorial(void);

		/*!*********************************************************************************
			\brief
				Set to the next tutorial state
		***********************************************************************************/
		void NextState(void);

		/*!*********************************************************************************
			\brief
				Gets whether the Tutorial is being played
			\return
				Returns true if Tutorial is playing, 
				Else returns false
		***********************************************************************************/
		b8 TutorialIsPlaying(void) const;

		void SetTutorialIsPlaying(b8 playing);

		// Getters and Setters
	public:
		/*!*********************************************************************************
			\brief
				Gets the current state of the tutorial as an enum value
			\return
				Returns the current state of the tutorial
		***********************************************************************************/
		TutorialState GetState(void);

		/*!*********************************************************************************
			\brief
				Get Tutorial Object
			\return
				Returns the Tutorial Object
		***********************************************************************************/
		ALEngine::ECS::Entity GetTutorialObject(void);

		/*!*********************************************************************************
			\brief
				Set Tutorial Object 
			\param [in] tut_obj
				The value for the Tutorial Object
		***********************************************************************************/
		void SetTutorialObject(ALEngine::ECS::Entity tut_obj);

		/*!*********************************************************************************
			\brief
				Get if a Tile is selected (Mainly for placement)
			\return
				Returns true if a Tile is selected
				Else returns false
		***********************************************************************************/
		b8 GetTileIsSelected(void);

		/*!*********************************************************************************
			\brief
				Set if a Tile is selected (Mainly for placement)
			\param [in] tileIsSelected
				Value for whether the Tile is selected
		***********************************************************************************/
		void SetTileIsSelected(b8 tileIsSelected);

		/*!*********************************************************************************
			\brief
				Get if a Tile is placed
			\return
				Returns true if tile was just placed,
				Else Returns False
		***********************************************************************************/
		b8 GetTileIsPlaced(void);

		/*!*********************************************************************************
			\brief
				Set if a Tile is placed
			\param [in] tileIsSelected
				Value for whether the Tile is placed
		***********************************************************************************/
		void SetTileIsPlaced(b8 tileIsPlaced);

		/*!*********************************************************************************
			\brief
				Get Player Entity 
			\return
				Return Player Entity
		***********************************************************************************/
		ALEngine::ECS::Entity GetPlayerEntity(void);

		/*!*********************************************************************************
			\brief
				Set player Entity
			\param [in] en
				Value to set to player entity
		***********************************************************************************/
		void SetPlayerEntity(ALEngine::ECS::Entity en);

		/*!*********************************************************************************
			\brief
				Gets the GameplaySystem
			\return
				Returns pointer to the GameplaySystem
		***********************************************************************************/
		std::shared_ptr<ALEngine::Script::GameplaySystem> GetGameplaySystem(void);

		/*!*********************************************************************************
			\brief
				Sets the GameplaySystem pointer
			\param [in] gs
				Pointer to the GameplaySystem, to be set to this
		***********************************************************************************/
		void SetGameplaySystem(std::shared_ptr<ALEngine::Script::GameplaySystem> gs);

		/*!*********************************************************************************
			\brief
				Gets whether the player has finished their move
			\return
				Returns whether player has finished their move
		***********************************************************************************/
		b8 GetPlayerMoveFinished(void);

		/*!*********************************************************************************
			\brief
				Gets whether the player has finished their move
			\param [in] moveOver
				Value to set player's move is over
		***********************************************************************************/
		void SetPlayerMoveFinished(b8 moveOver);

		/*!*********************************************************************************
			\brief
				Gets whether the player has clicked End Turn
			\return
				Returns whether player has clicked End Turn
		***********************************************************************************/
		b8 GetEndTurnPressed(void);

		/*!*********************************************************************************
			\brief
				Gets whether the player has clicked End Turn
			\param [in] isPressed
				Value to set whether player has clicked End Turn
		***********************************************************************************/
		void SetEndTurnPressed(b8 isPressed);
		
		/*!*********************************************************************************
			\brief
				Gets whether the player's turn has started
			\return
				Returns whether player's turn has started
		***********************************************************************************/
		b8 GetPlayerTurnStart(void);

		/*!*********************************************************************************
			\brief
				Gets whether the player's turn has started
			\param [in] hasStarted
				Value to set whether player's turn has started
		***********************************************************************************/
		void SetPlayerTurnStart(b8 hasStarted);

		/*!*********************************************************************************
			\brief
				Gets number of enemies player has killed
			\return
				Returns the number of enemies player has killed
		***********************************************************************************/
		u16 GetNumEnemiesKilled(void);

		/*!*********************************************************************************
			\brief
				Increments the number of enemies killed by 1
		***********************************************************************************/
		void IncrementNumberOfEnemiesKilled(void);

		/*!*********************************************************************************
			\brief
				Sets all the abilities to be off
		***********************************************************************************/
		void SetAllAbilitiesOff(void);
		
		/*!*********************************************************************************
			\brief
				Sets all the abilities but Hard Drop to be off
		***********************************************************************************/
		void SetAllAbilitiesButHardDropOff(void);

		/*!*********************************************************************************
			\brief
				Sets all the abilities but Construct Tile to be off
		***********************************************************************************/
		void SetAllAbilitiesButConstructTileOff(void);

	private:

		// Entities
		ALEngine::ECS::Entity m_TutorialObject{ ALEngine::ECS::MAX_ENTITIES };
		ALEngine::ECS::Entity m_PlayerObject{ ALEngine::ECS::MAX_ENTITIES };

		// Enums
		TutorialState m_CurrentState{ 0 };					// Current cutscene state

		// Booleans
		b8 m_TutorialIsPlaying{ true };						// Whether Tutorial is being played or not
		b8 m_TileIsSelected{ false };						// Whether the player is holding a tile right now
		b8 m_TileIsPlaced{ false };							// Whether player has placed tile
		b8 m_PlayerMoveFinished{ false };					// Whether player has finished moving
		b8 m_EndTurnPressed{ false };						// Whether player pressed End Turn
		b8 m_PlayerTurnStart{ false };						// Whether player's turn has started

		// Info
		u16 m_EnemiesKilled{};								// Number of enemies killed by player

		// Pointers
		std::shared_ptr<ALEngine::Script::GameplaySystem> m_GameplaySystem{ nullptr };

		// Required for Singleton to function
		friend class ALEngine::Templates::Singleton<TutorialManager>;
		friend class ALEngine::Memory::StaticMemory;
	};

	enum class TutorialState : u32
	{
		TUTORIAL_INTRO_CS = 0,						// Intro - Destroy All Enemies & Select Tile Cutscenes
		TUTORIAL_SELECT_TILE,						// Arrow pointing at tiles
		TUTORIAL_ACTION_PHASE_CS,					// Action Phase & AP Description Cutscenes
		TUTORIAL_ACTION_PHASE_WALK,					// Action Phase Walking (No Skills Yet)
		TUTORIAL_MELEE_CS,							// Melee Enemy Cutscene
		TUTORIAL_MELEE_END_TURN,					// Melee End Turn
		TUTORIAL_CRACKING_TILES_CS,					// Cracking Tiles Cutscene
		TUTORIAL_SETUP_2,							// 2nd Setup Phase
		TUTORIAL_HARD_DROP_CS,						// Setup Phase Again & Hard Drop Cutscenes
		TUTORIAL_HARD_DROP,							// Hard Drop Picking
		TUTORIAL_ABILITIES_CS,						// Cutscene explaining how the attack mechanics work 
		TUTORIAL_ATTACK,							// Player Execute Attack
		TUTORIAL_SHOW_HOVER_CS,						// Show the hover over enemy cutscene
		TUTORIAL_WAIT_GUARD_DEFEAT,					// Wait for the player to defeat the guard
		TUTORIAL_MOVE_CAMERA_1,						// Move Camera to the Tile Destroyer
		TUTORIAL_TILE_DESTROYER_CS,					// Cutscene Explaining Tile Destroyer & Using Construct Wall
		TUTORIAL_CONSTRUCT_TILE,					// Use Construct Wall
		TUTORIAL_UTILITIES_CS,						// Explanation of Utility Skills
		TUTORIAL_PLACE_CONSTRUCT_TILE,				// Placing of the Construct Wall
		TUTORIAL_DEFEAT_TILE_DESTROYER_CS,			// Cutscene to tell player to defeat tile destroyer
		TUTORIAL_WAIT_TILE_DESTROYER_DEFEAT,		// Wait for Tile Destroyer to be defeated
		TUTORIAL_MOVE_CAMERA_2,						// Move camera to the Summoner
		TUTORIAL_SUMMONER_INTRO_CS,					// Cutscene Explaining the Summoner
		TUTORIAL_FINAL_FIGHT,						// Final Fight
		TUTORIAL_FINAL_CS,							// Final cutscene



		TUTORIAL_END								// End Level
	};
}

#endif