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
		b8 TutorialIsPlaying(void);

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
		//TUTORIAL_MELEE_END_TURN,					// Fight Melee Enemy
		TUTORIAL_HARD_DROP_CS,						// Setup Phase Again & Hard Drop Cutscenes
		//TUTORIAL_HARD_DROP,							// Hard Drop Picking


		TUTORIAL_END								// End Level
	};
}

#endif