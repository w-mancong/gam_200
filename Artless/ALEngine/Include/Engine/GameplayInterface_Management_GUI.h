/*!
file:   GamePlayInterface.h
author: Tan Zhen Xiong
email:  t.zhenxiong\@digipen.edu
brief:	This file contains the function definition for GamePlayInterface.h

		All content :copyright: 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/

#ifndef	GAMEPLAY_INTERFACE_MANAGEMENT_GUI_H
#define GAMEPLAY_INTERFACE_MANAGEMENT_GUI_H
namespace ALEngine::Engine::GameplayInterface_Management_GUI
{
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

	struct GUI
	{
		//UI
		ECS::Entity endTurnBtnEntity;

		//Keep track of GUI entities
		ECS::Entity Unit_Health, Unit_Name, Unit_Attack, Unit_Defense, Unit_Movement, Unit_Range;
		ECS::Entity Unit_Profile;
		ECS::Entity Unit_Healthbar, Phase_Indicator;
		ECS::Entity Skill_Tip_Icon, Skill_Tip_Header, Skill_Tip_Line1, Skill_Tip_Line2, Skill_Tip_Line3, Skill_Tip_Line4;
		ECS::Entity Tooltip_Skills_Card;

		//Win
		ECS::Entity Win_Clear, Win_Button;

		//List for containing entities of GUI
		std::vector<ECS::Entity> GUI_Abilities_Button_List;
		std::vector<ECS::Entity> GUI_Pattern_Button_List;
	};

	GUI& getGuiManager();

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

	void DisableToolTipGUI();

	void InitializeGUI();

	void GuiUpdatePhaseIndicator(PHASE_STATUS);
}
#endif