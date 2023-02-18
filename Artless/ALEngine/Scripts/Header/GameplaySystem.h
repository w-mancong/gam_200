#ifndef	GAMEPLAY_SYSTEM_H
#define GAMEPLAY_SYSTEM_H

#include <ECS/Components/UniBehaviour.h>
#include <Engine/Gameplay_Interface.h>

namespace ALEngine::Script
{
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
	        create enemy unit entity
		***********************************************************************************/
		void CreateEnemyUnit(ECS::Entity entity); 

		/*!*********************************************************************************
		\brief
			End current turn
		***********************************************************************************/
		void EndTurn();

	public:
		GAMEPLAY_STATUS currentGameplayStatus = GAMEPLAY_STATUS::RUNNING;							//Keep track of gameplay status, running or stopped
		PHASE_STATUS currentPhaseStatus = PHASE_STATUS::PHASE_SETUP;								//Keep track of phase
		UNITS_CONTROL_STATUS currentUnitControlStatus = UNITS_CONTROL_STATUS::NOTHING;				//Keep track of status of unit control
		PATTERN_PLACEMENT_STATUS currentPatternPlacementStatus = PATTERN_PLACEMENT_STATUS::NOTHING;	//Keep track of what the pattern is being placed for
	

		//******VARIABLES**********//
		u32 roomSize[2]{ 10, 10 };		//Size to initialize the room with
		GAMEPLAY_SYSTEM_INTERFACE_H::Room m_Room;					//Room COntainer

		//Keep track of the parent of all the cells
		ECS::Entity m_Room_Parent_Entity;

		//Patterns
		std::vector<Pattern> pattern_Default, pattern_List;
		Pattern selected_Pattern;
		u32 selected_Pattern_Index{ 0 };

		//Abilities
		std::vector<Abilities> Abilities_List;
		Abilities selected_Abilities;

		//Cell that the mouse is hovering over
		ECS::Entity current_Moused_Over_Cell;

		//Tracks debug drawing for room
		b8 is_DebugDraw = false;

		//Cheats
		b8 godMode = false, cheat_abilitiesDoubleDamage = false;
	};
}

#endif