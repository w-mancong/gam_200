/*!
file:	TutorialManager.h
author: Lucas Nguyen
email:	l.nguyen\@digipen.edu
brief:	Header file that contains declarations for functions needed for the Tutorial

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#ifndef TUTORIAL_MANAGER_H
#define TUTORIAL_MANAGER_H

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
				Get Tutorial Object
			\return
				Returns the Tutorial Object
		***********************************************************************************/
		b8 GetTileIsSelected(void);

		void SetTileIsSelected(b8 tileIsSelected);

	private:

		// Entities
		ALEngine::ECS::Entity m_TutorialObject{ ALEngine::ECS::MAX_ENTITIES };

		TutorialState m_CurrentState{ 0 };					// Current cutscene state

		// Booleans
		b8 m_TutorialIsPlaying{ true };						// Whether Tutorial is being played or not
		b8 m_TileIsSelected{ false };						// Whether the player is holding a tile right now

		// Required for Singleton to function
		friend class ALEngine::Templates::Singleton<TutorialManager>;
		friend class ALEngine::Memory::StaticMemory;
	};

	enum class TutorialState : u32
	{
		TUTORIAL_INTRO_CS = 0,						// Intro - Destroy All Enemies & Select Tile Cutscenes
		TUTORIAL_SELECT_TILE,						// Arrow pointing at tiles
		//TUTORIAL_PLACE_TILE,						// Tell player to place tile
		TUTORIAL_ACTION_PHASE_CS,					// Action Phase & AP Description Cutscenes
		//TUTORIAL_ACTION_PHASE_WALK,					// Action Phase Walking (No Skills Yet)
		TUTORIAL_MELEE_CS,							// Melee Enemy Cutscene
		//TUTORIAL_MELEE_END_TURN,					// Fight Melee Enemy
		TUTORIAL_HARD_DROP_CS,						// Setup Phase Again & Hard Drop Cutscenes
		//TUTORIAL_HARD_DROP,							// Hard Drop Picking


		TUTORIAL_END								// End Level
	};
}

#endif