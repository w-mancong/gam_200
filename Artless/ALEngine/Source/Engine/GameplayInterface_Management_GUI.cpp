/*!
file:	GamePlayInterface.cpp
author:	Mohamed Zafir
email:	m.zafir\@digipen.edu
brief:	This file contains the function definition for GamePlayInterface.cpp

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/

#include "pch.h"
#include <Engine/GameplayInterface_Management_GUI.h>

namespace ALEngine::Engine::GameplayInterface_Management_GUI
{
	GUI guiManager;

	GUI& getGuiManager()
	{
		return guiManager;
	}

	void InitializePatternGUI(std::vector<ECS::Entity>& GUI_Pattern_Button_Entities) {
		//Clear GUI
		GUI_Pattern_Button_Entities.clear();

		//There will be a fix of 4 buttons
		for (int i = 1; i <= 4; ++i) {
			GUI_Pattern_Button_Entities.push_back(Coordinator::Instance()->GetEntityByTag("next_tile_icon" + std::to_string(i)));
		}

		//Set base x
		u32 x_offset = 150;

		//First one will be the current
		Transform transform;
		transform.position = { 1000.f, 100.f, 0.f };
		transform.scale = { 100.f, 100.f };

		ECS::CreateButton(GUI_Pattern_Button_Entities[0]);
		ECS::CreateButton(GUI_Pattern_Button_Entities[1]);
		ECS::CreateButton(GUI_Pattern_Button_Entities[2]);
		ECS::CreateButton(GUI_Pattern_Button_Entities[3]);
	}

	void InitializeAbilitiesGUI(std::vector<ECS::Entity>& GUI_Abilities_Button_Entities) {
		//Clear GUI
		GUI_Abilities_Button_Entities.clear();

		//There will be a fix of 4 buttons
		for (int i = 1; i <= 6; ++i) {
			GUI_Abilities_Button_Entities.push_back(Coordinator::Instance()->GetEntityByTag("skill_icon" + std::to_string(i)));
			//GUI_Abilities_Button_Entities.push_back(Coordinator::Instance()->CreateEntity());
		}

		//Set base x
		u32 x_offset = 75;

		//Start pos
		Vector3 startPos = { 50.f, 100.f, 0.f };

		//First one will be the current
		Transform transform;
		transform.position = startPos;
		transform.scale = { 50.f, 50.f };

		ECS::CreateButton(GUI_Abilities_Button_Entities[0]);
		ECS::CreateButton(GUI_Abilities_Button_Entities[1]);
		ECS::CreateButton(GUI_Abilities_Button_Entities[2]);
		ECS::CreateButton(GUI_Abilities_Button_Entities[3]);
		ECS::CreateButton(GUI_Abilities_Button_Entities[4]);
		ECS::CreateButton(GUI_Abilities_Button_Entities[5]);

		for (int i = 0; i < 3; ++i) {
			EventTrigger& eventTrigger = Coordinator::Instance()->GetComponent<EventTrigger>(guiManager.GUI_Abilities_Button_List[i]);
			Sprite& sprite = Coordinator::Instance()->GetComponent<Sprite>(guiManager.GUI_Abilities_Button_List[i]);

			eventTrigger.isEnabled = true;
		}
	}

	void TogglePatternGUI(b8 istrue) {
		//Toggle the pattern GUI accordingly
		for (int i = 0; i < guiManager.GUI_Pattern_Button_List.size(); ++i) {
			EventTrigger& eventTrigger = Coordinator::Instance()->GetComponent<EventTrigger>(guiManager.GUI_Pattern_Button_List[i]);
			Sprite& sprite = Coordinator::Instance()->GetComponent<Sprite>(guiManager.GUI_Pattern_Button_List[i]);

			eventTrigger.isEnabled = istrue;

			if (istrue)
				sprite.color = { 1.f, 1.f, 1.f, 1.f };
			else
				sprite.color = { 0.1f, 0.1f, 0.1f, 1.f };
		}
	}

	void ToggleAbilitiesGUI(b8 istrue) {
		//Disable all the abilities GUI
		for (int i = 0; i < guiManager.GUI_Abilities_Button_List.size(); ++i) {
			EventTrigger& eventTrigger = Coordinator::Instance()->GetComponent<EventTrigger>(guiManager.GUI_Abilities_Button_List[i]);
			Sprite& sprite = Coordinator::Instance()->GetComponent<Sprite>(guiManager.GUI_Abilities_Button_List[i]);

			eventTrigger.isEnabled = istrue;

			if (istrue) {
				sprite.color = { 1.f, 1.f, 1.f, 1.f };
			}
			else
				sprite.color = { 0.1f, 0.1f, 0.1f, 1.f };
		}
	}

	void DisableToolTipGUI() 
	{
		ECS::Entity en_tooltip = Coordinator::Instance()->GetEntityByTag("tooltip_skills");
		ECS::Entity en_skillicon = Coordinator::Instance()->GetEntityByTag("skill_icon");
		ECS::Entity en_textskill = Coordinator::Instance()->GetEntityByTag("text_skillname");
		ECS::Entity en_hard_drop = Coordinator::Instance()->GetEntityByTag("hard_drop_des1");
		ECS::Entity en_life_drain = Coordinator::Instance()->GetEntityByTag("life_drain_des1");

		ECS::SetActive(false, en_tooltip);
		ECS::SetActive(false, en_skillicon);
		ECS::SetActive(false, en_textskill);
		ECS::SetActive(false, en_hard_drop);
		ECS::SetActive(false, en_life_drain);
	}

	void InitializeGUI()
	{
		//Initialize GUI Text and Sprites zafir
		guiManager.Unit_Name = Coordinator::Instance()->GetEntityByTag("text_playername");
		guiManager.Unit_Health = Coordinator::Instance()->GetEntityByTag("text_bar_hp");
		guiManager.Unit_Profile = Coordinator::Instance()->GetEntityByTag("profile_player");
		guiManager.Unit_Attack = Coordinator::Instance()->GetEntityByTag("text_attack_output");
		guiManager.Unit_Defense = Coordinator::Instance()->GetEntityByTag("text_defense_output");
		guiManager.Unit_Movement = Coordinator::Instance()->GetEntityByTag("text_move_output");
		guiManager.Unit_Range = Coordinator::Instance()->GetEntityByTag("text_range_output");
		guiManager.Unit_Healthbar = Coordinator::Instance()->GetEntityByTag("red health bar");
		guiManager.Win_Clear = Coordinator::Instance()->GetEntityByTag("Win_Clear_Text");
		guiManager.Win_Button = Coordinator::Instance()->GetEntityByTag("Win_Button");
		guiManager.Phase_Indicator = Coordinator::Instance()->GetEntityByTag("text_phaseindicator");

		ECS::SetActive(false, guiManager.endTurnBtnEntity);
		ECS::SetActive(false, guiManager.Win_Clear);

		ECS::CreateButton(guiManager.Win_Button);
	}

	void GuiUpdatePhaseIndicator(PHASE_STATUS status)
	{
		Text& phaseIndicator = Coordinator::Instance()->GetComponent<Text>(getGuiManager().Phase_Indicator);
		phaseIndicator.colour = Vector3(1.f, 1.f, 1.f);

		switch (status)
		{
		case PHASE_STATUS::PHASE_SETUP:
			phaseIndicator.textString = "Setup Phase";
			break;

		case PHASE_STATUS::PHASE_ACTION:
			phaseIndicator.textString = "Action Phase";
			break;

		case PHASE_STATUS::PHASE_ENEMY:
			phaseIndicator.textString = "Enemy Phase";
			phaseIndicator.colour = Vector3(1.f, 0.32f, 0.28f);
			break;
		}
	}
}