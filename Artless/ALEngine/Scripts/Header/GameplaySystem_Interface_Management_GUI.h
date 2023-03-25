/*!
file:	GameplaySystem_Interface_Management_GUI.h
author:	Mohamed Zafir (75%)
co-author: Zheng Xiong (25%)
email:	m.zafir\@digipen.edu
brief:	This file contains the function declarations for GameplaySystem_Interface_Management_GUI.

		All content 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#ifndef	GAMEPLAY_SYSTEM_INTERFACE_GUI_H
#define GAMEPLAY_SYSTEM_INTERFACE_GUI_H

#include <ECS/Components/UniBehaviour.h>
#include <Engine/Gameplay_Interface.h>

namespace ALEngine::Script
{
	struct GUI
	{
		//UI
		ECS::Entity endTurnBtnEntity{ ECS::MAX_ENTITIES };

		//Keep track of GUI entities
		ECS::Entity Unit_Health, Unit_Name, Unit_Attack, Unit_Defense, Unit_Movement, Unit_Rang, Unit_Profile;
		ECS::Entity Unit_Healthbar, Phase_Indicator, Your_Turn_Sign, Enemy_Tip_Health, Enemy_Tip_Healthbar;
		ECS::Entity Tooltip_Skills_Card, FPS_Label, Pause_Button, Enemy_Tip_Guard, Enemy_Tip_Summoner, Enemy_Tip_Flying;
		std::array<ECS::Entity, 6> AP_Indicators;
		std::array<ECS::Entity, 6> Highlight_blocks;
		
		b8 fpsActive{ true };

		//Win
		ECS::Entity Win_Clear, Win_Button;

		//Lose
		ECS::Entity Lose_Clear, Lose_Button;

		//Center Pattern Parent
		ECS::Entity GUI_Center_Pattern_Parent, GUI_Center_Pattern_BG, GUI_Bottom_Pattern_Parent;

		//List for containing entities of GUI
		std::vector<ECS::Entity> GUI_Abilities_Button_List;
		std::vector<ECS::Entity> GUI_Pattern_Button_List;
		std::vector<ECS::Entity> GUI_Pattern_Button_List_BG;
		std::vector<ECS::Entity> GUI_Center_Pattern_Button_List;
		std::vector<ECS::Entity> GUI_Center_Pattern_Button_List_BG;

		f32 Your_Turn_timer{};
	};

	class GameplaySystem_Interface_Management_GUI : public ECS::Component::UniBehaviour
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
			Initialize Pattern GUI
		***********************************************************************************/
		void InitializePatternGUI(std::vector<ECS::Entity>& GUI_Pattern_Button_Entities);

		/*!*********************************************************************************
		\brief
			Initialize Pattern GUI
		***********************************************************************************/
		void InitializeAbilitiesGUI(std::vector<ECS::Entity>& GUI_Abilities_Button_Entities);

		/*!*********************************************************************************
			\brief
				Toggle Pattern GUI active
		***********************************************************************************/
		void TogglePatternGUI(b8 istrue);

		/*!*********************************************************************************
			\brief
				Toggle Center Pattern GUI active
		***********************************************************************************/
		void ToggleCenterPatternGUI(b8 istrue);

		/*!*********************************************************************************
			\brief
				Toggle first Pattern GUI active
		***********************************************************************************/
		void TogglePatternFirstOnlyGUI(b8 istrue);

		/*!*********************************************************************************
		\brief
			Toggle Abilities GUI active
		***********************************************************************************/
		void ToggleAbilitiesGUI(b8 istrue);

		/*!*********************************************************************************
		\brief
			Update the unit information GUI when select the unit
		***********************************************************************************/
		void UpdateGUI_OnSelectUnit(ECS::Entity unitEntity);

		/*!*********************************************************************************
		\brief
			Disable GUI for tool tips.
		***********************************************************************************/
		void DisableToolTipGUI();

		/*!*********************************************************************************
		\brief
			Initialize GUI system.
		***********************************************************************************/
		void InitializeGUI();

		/*!*********************************************************************************
		\brief
			Updates the phase indicator.
		***********************************************************************************/
		void GuiUpdatePhaseIndicator(PHASE_STATUS);

		/*!*********************************************************************************
		\brief
			Updates the FPS label. Can be toggled on and off using ctrl.
		***********************************************************************************/
		void UpdateFpsLabel();

		/*!*********************************************************************************
		\brief
			Updates the Action point UI
		***********************************************************************************/
		void Update_AP_UI(int AP_count);

		/*!*********************************************************************************
		\brief
			Updates the Action point UI to visualize cost
		***********************************************************************************/
		void Update_AP_UI_For_Cost(int AP_count, int AP_Cost);

		void Update_Skill_Tip_Position();

		void DisplayYourTurn();

		void UpdateYourTurnSign();

		template <typename T>
		static T Lerp(T a, T b, float t);

		void Update_Ability_Cooldown(std::vector<Abilities> ability_set, bool isAbilityGUIActive);

		GUI guiManager;	
		
		GUI& getGuiManager()
		{
			return guiManager;
		}

		// For RTTR
		void DeserializeComponent(ECS::Entity en)
		{
			ECS::AddLogicComponent<GameplaySystem_Interface_Management_GUI>(en);
		};
		RTTR_ENABLE(ECS::Component::UniBehaviour)
	};
}

#endif