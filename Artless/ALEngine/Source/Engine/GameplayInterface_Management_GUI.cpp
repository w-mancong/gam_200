/*!
file:	GamePlayInterface.cpp
author:	Mohamed Zafir (75%)
co-author:	Tan Zhen Xiong (25%)
email:	t.zhenxiong@digipen.edu
		m.zafir@digipen.edu
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
	
	void Event_Button_Enter_Ability_GUI(ECS::Entity invoker) {
		if (utils::IsEqual(Time::m_Scale, 0.f)) {
			return;
		}

		ECS::SetActive(true, guiManager.Tooltip_Skills_Card);
		EntityData skill = Coordinator::Instance()->GetComponent<EntityData>(invoker);
		Sprite& sprite = Coordinator::Instance()->GetComponent<Sprite>(guiManager.Skill_Tip_Icon);
		Text& headerText = Coordinator::Instance()->GetComponent<Text>(guiManager.Skill_Tip_Header);
		Text& line1 = Coordinator::Instance()->GetComponent<Text>(guiManager.Skill_Tip_Line1);
		Text& line2 = Coordinator::Instance()->GetComponent<Text>(guiManager.Skill_Tip_Line2);
		Text& line3 = Coordinator::Instance()->GetComponent<Text>(guiManager.Skill_Tip_Line3);
		Text& line4 = Coordinator::Instance()->GetComponent<Text>(guiManager.Skill_Tip_Line4);
		
		if (skill.tag == "skill_icon1")
		{
			sprite.id = AssetManager::Instance()->GetGuid("Assets/Images/HardDrop.png");
			headerText.textString = "Hard Drop";
			line1.textString = "Slam a Tetromino on enemies,";
			line2.textString = "dealing 15 DMG on each";
			line3.textString = "impacted enemy.";
			line4.textString = "";
		}
		else if (skill.tag == "skill_icon2")
		{
			sprite.id = AssetManager::Instance()->GetGuid("Assets/Images/LifeDrain.png");
			headerText.textString = "Life Drain";
			line1.textString = "Deal DMG to enemies and heal";
			line2.textString = "for half of all DMG dealt.";
			line3.textString = "[Lifesteal is WIP]";
			line4.textString = "";
		}
		else if (skill.tag == "skill_icon3")
		{
			sprite.id = AssetManager::Instance()->GetGuid("Assets/Images/Icon_Skill_ConstructTile.png");
			headerText.textString = "Construct Tile";
			line1.textString = "Erect a wall that blocks";
			line2.textString = "enemies from passing or";
			line3.textString = "attacking.";
			line4.textString = "[Lasts for 2 turns]";
		}
		else if (skill.tag == "skill_icon4")
		{
			//sprite.id = AssetManager::Instance()->GetGuid("Assets/Images/LifeDrain.png");
			headerText.textString = "TBD";
			line1.textString = "To be Developed";
			line2.textString = "";
			line3.textString = "";
			line4.textString = "";
		}
		else if (skill.tag == "skill_icon5")
		{
			//sprite.id = AssetManager::Instance()->GetGuid("Assets/Images/LifeDrain.png");
			headerText.textString = "TBD";
			line1.textString = "To be Developed";
			line2.textString = "";
			line3.textString = "";
			line4.textString = "";
		}
		else if (skill.tag == "skill_icon6")
		{
			//sprite.id = AssetManager::Instance()->GetGuid("Assets/Images/LifeDrain.png");
			headerText.textString = "TBD";
			line1.textString = "To be Developed";
			line2.textString = "";
			line3.textString = "";
			line4.textString = "";
		}
	}

	void Event_Button_Exit_Ability_GUI(ECS::Entity invoker) {
		if (utils::IsEqual(Time::m_Scale, 0.f)) {
			return;
		}

		DisableToolTipGUI();
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

		for (int i = 0; i < 6; ++i) {
			ECS::Subscribe(GUI_Abilities_Button_Entities[i], EVENT_TRIGGER_TYPE::ON_POINTER_ENTER, Event_Button_Enter_Ability_GUI);
			ECS::Subscribe(GUI_Abilities_Button_Entities[i], EVENT_TRIGGER_TYPE::ON_POINTER_EXIT, Event_Button_Exit_Ability_GUI);
		}

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
		ECS::SetActive(false, en_tooltip);
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
		guiManager.Skill_Tip_Icon = Coordinator::Instance()->GetEntityByTag("skill_icon");
		guiManager.Skill_Tip_Header = Coordinator::Instance()->GetEntityByTag("text_skillname");
		guiManager.Skill_Tip_Line1 = Coordinator::Instance()->GetEntityByTag("tooltip_skill_line1");
		guiManager.Skill_Tip_Line2 = Coordinator::Instance()->GetEntityByTag("tooltip_skill_line2");
		guiManager.Skill_Tip_Line3 = Coordinator::Instance()->GetEntityByTag("tooltip_skill_line3");
		guiManager.Skill_Tip_Line4 = Coordinator::Instance()->GetEntityByTag("tooltip_skill_line4");
		guiManager.Tooltip_Skills_Card = Coordinator::Instance()->GetEntityByTag("tooltip_skills");
		guiManager.FPS_Label = Coordinator::Instance()->GetEntityByTag("FPS_label");

		ECS::SetActive(false, guiManager.endTurnBtnEntity);
		ECS::SetActive(false, guiManager.Win_Clear);

		ECS::CreateButton(guiManager.Win_Button);
	}

	void UpdateFpsLabel()
	{
		Text& fps = Coordinator::Instance()->GetComponent<Text>(getGuiManager().FPS_Label);
		if (guiManager.fpsActive)
		{
			std::ostringstream oss{};
			oss << "FPS: " << (int)Time::m_FPS;
			fps.textString = oss.str();
		}
		else
		{
			fps.textString = "";
		}

		if (Input::KeyTriggered(KeyCode::Ctrl))
			guiManager.fpsActive = !guiManager.fpsActive;
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