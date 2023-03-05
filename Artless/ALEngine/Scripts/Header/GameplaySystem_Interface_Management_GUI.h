#ifndef	GAMEPLAY_SYSTEM_INTERFACE_GUI_H
#define GAMEPLAY_SYSTEM_INTERFACE_GUI_H

#include <ECS/Components/UniBehaviour.h>
#include <Engine/Gameplay_Interface.h>

namespace ALEngine::Script
{
	struct GUI
	{
		//UI
		ECS::Entity endTurnBtnEntity;

		//Keep track of GUI entities
		ECS::Entity Unit_Health, Unit_Name, Unit_Attack, Unit_Defense, Unit_Movement, Unit_Range;
		ECS::Entity Unit_Profile;
		ECS::Entity Unit_Healthbar, Phase_Indicator;
		ECS::Entity Skill_Tip_Icon, Skill_Tip_Header, Skill_Tip_Line1, Skill_Tip_Line2, Skill_Tip_Line3, Skill_Tip_Line4;
		ECS::Entity Tooltip_Skills_Card, FPS_Label, Pause_Button;
		std::array<ECS::Entity, 6> AP_Indicators;
		
		b8 fpsActive{ true };

		//Win
		ECS::Entity Win_Clear, Win_Button;

		//List for containing entities of GUI
		std::vector<ECS::Entity> GUI_Abilities_Button_List;
		std::vector<ECS::Entity> GUI_Pattern_Button_List;
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