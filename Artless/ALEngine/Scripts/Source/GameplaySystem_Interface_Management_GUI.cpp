/*!
file:	GameplaySystem_Interface_Management_GUI.cpp
author:	Mohamed Zafir (75%)
co-author: Zheng Xiong (25%)
email:	m.zafir\@digipen.edu
brief:	This file contains the function definitions for GameplaySystem_Interface_Management_GUI.

		All content 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#include <pch.h>
#include <GameplaySystem_Interface_Management_GUI.h>
#include <GameplaySystem.h>
#include <Engine/Gameplay_Interface.h>
#include <GameplaySystem_Interface_Management_Enemy.h>

namespace ALEngine::Script
{
	namespace 
	{
		std::shared_ptr<GameplaySystem> gameplaySystem;
		std::shared_ptr<GameplaySystem_Interface_Management_GUI> gameplaySystem_GUI;
	}


	void GameplaySystem_Interface_Management_GUI::Load(ECS::Entity en)
	{

	}

	void GameplaySystem_Interface_Management_GUI::Init(ECS::Entity en){
		gameplaySystem = ECS::GetLogicComponent<GameplaySystem>(en);
		gameplaySystem_GUI = ECS::GetLogicComponent<GameplaySystem_Interface_Management_GUI>(en);
		Set_GameplayInterface_GUI(en);
	}

	void GameplaySystem_Interface_Management_GUI::Update(ECS::Entity en)
	{

	}

	void GameplaySystem_Interface_Management_GUI::LateUpdate(ECS::Entity en)
	{

	}

	void GameplaySystem_Interface_Management_GUI::Free(ECS::Entity en)
	{
		gameplaySystem.reset();
		gameplaySystem_GUI.reset();
	}

	void GameplaySystem_Interface_Management_GUI::Unload(ECS::Entity en)
	{

	}

	void GameplaySystem_Interface_Management_GUI::UpdateGUI_OnSelectUnit(ECS::Entity unitEntity) {
		Unit& unit = Coordinator::Instance()->GetComponent<Unit>(unitEntity);

		Text& health_text = Coordinator::Instance()->GetComponent<Text>(getGuiManager().Unit_Health);
		Text& name_text = Coordinator::Instance()->GetComponent<Text>(getGuiManager().Unit_Name);
		Text& attack_text = Coordinator::Instance()->GetComponent<Text>(getGuiManager().Unit_Attack);
		Text& defense_text = Coordinator::Instance()->GetComponent<Text>(getGuiManager().Unit_Defense);
		Text& movement_text = Coordinator::Instance()->GetComponent<Text>(getGuiManager().Unit_Movement);
		Sprite& profile = Coordinator::Instance()->GetComponent<Sprite>(getGuiManager().Unit_Profile);

		health_text.textString = std::to_string(unit.health) + "/" + std::to_string(unit.maxHealth);
		attack_text.textString = std::to_string(unit.minDamage) + "/" + std::to_string(unit.maxDamage);
		defense_text.textString = std::to_string(unit.defense);
		movement_text.textString = std::to_string(unit.actionPoints) + "/" + std::to_string(unit.maxActionPoints);
		name_text.textString = unit.unit_Name;

		profile.id = Engine::AssetManager::Instance()->GetGuid(unit.unit_Profile_Sprite_File);

		Transform& healthbar_transform = Coordinator::Instance()->GetComponent<Transform>(getGuiManager().Unit_Healthbar);
		healthbar_transform.localScale.x = (unit.health <= 0 ? 0 : ((f32)unit.health / (f32)unit.maxHealth)) * 0.5f;
	}

	void GameplaySystem_Interface_Management_GUI::InitializeGUI()
	{
		//Initialize GUI Text and Sprites
		guiManager.Unit_Name = Coordinator::Instance()->GetEntityByTag("text_playername");
		guiManager.Unit_Health = Coordinator::Instance()->GetEntityByTag("text_bar_hp");
		guiManager.Unit_Profile = Coordinator::Instance()->GetEntityByTag("profile_unit");
		guiManager.Unit_Attack = Coordinator::Instance()->GetEntityByTag("text_attack_output");
		guiManager.Unit_Defense = Coordinator::Instance()->GetEntityByTag("text_defense_output");
		guiManager.Unit_Movement = Coordinator::Instance()->GetEntityByTag("text_move_output");
		guiManager.Unit_Healthbar = Coordinator::Instance()->GetEntityByTag("red_health_bar");
		guiManager.Win_Clear = Coordinator::Instance()->GetEntityByTag("Win_Clear_Text");
		guiManager.Win_Button = Coordinator::Instance()->GetEntityByTag("Win_Button");
		guiManager.Lose_Clear = Coordinator::Instance()->GetEntityByTag("Lose_Clear_Text");
		guiManager.Lose_Button = Coordinator::Instance()->GetEntityByTag("Lose_Button");
		guiManager.Phase_Indicator = Coordinator::Instance()->GetEntityByTag("text_phaseindicator");
		guiManager.Tooltip_Skills_Card = Coordinator::Instance()->GetEntityByTag("tooltip_skills");
		guiManager.FPS_Label = Coordinator::Instance()->GetEntityByTag("FPS_label");
		guiManager.Pause_Button = Coordinator::Instance()->GetEntityByTag("pause_button");
		guiManager.AP_Indicators[0] = Coordinator::Instance()->GetEntityByTag("AP1");
		guiManager.AP_Indicators[1] = Coordinator::Instance()->GetEntityByTag("AP2");
		guiManager.AP_Indicators[2] = Coordinator::Instance()->GetEntityByTag("AP3");
		guiManager.AP_Indicators[3] = Coordinator::Instance()->GetEntityByTag("AP4");
		guiManager.AP_Indicators[4] = Coordinator::Instance()->GetEntityByTag("AP5");
		guiManager.AP_Indicators[5] = Coordinator::Instance()->GetEntityByTag("AP6");
		guiManager.endTurnBtnEntity = Coordinator::Instance()->GetEntityByTag("end_turn"); 
		guiManager.Highlight_blocks[0] = Coordinator::Instance()->GetEntityByTag("Highlight_Path1");
		guiManager.Highlight_blocks[1] = Coordinator::Instance()->GetEntityByTag("Highlight_Path2");
		guiManager.Highlight_blocks[2] = Coordinator::Instance()->GetEntityByTag("Highlight_Path3");
		guiManager.Highlight_blocks[3] = Coordinator::Instance()->GetEntityByTag("Highlight_Path4");
		guiManager.Highlight_blocks[4] = Coordinator::Instance()->GetEntityByTag("Highlight_Path5");
		guiManager.Highlight_blocks[5] = Coordinator::Instance()->GetEntityByTag("Highlight_Path6");
		guiManager.Enemy_Tip_Guard = Coordinator::Instance()->GetEntityByTag("guard_tip");
		guiManager.Enemy_Tip_Summoner = Coordinator::Instance()->GetEntityByTag("destoryer_tip");
		guiManager.Enemy_Tip_Flying = Coordinator::Instance()->GetEntityByTag("summoner_tip");

		ECS::SetActive(false, guiManager.endTurnBtnEntity);
		ECS::SetActive(false, guiManager.Win_Clear);
		ECS::SetActive(false, guiManager.Lose_Clear);

		ECS::CreateButton(guiManager.Win_Button);
		ECS::CreateButton(guiManager.Lose_Button);
	}

	void GameplaySystem_Interface_Management_GUI::UpdateFpsLabel()
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

	void GameplaySystem_Interface_Management_GUI::InitializePatternGUI(std::vector<ECS::Entity>& GUI_Pattern_Button_Entities) {
		//Clear GUI
		GUI_Pattern_Button_Entities.clear();

		//There will be a fix of 4 buttons
		for (int i = 1; i <= 3; ++i) {
			GUI_Pattern_Button_Entities.push_back(Coordinator::Instance()->GetEntityByTag("next_tile_icon" + std::to_string(i)));
		}

		//Set base x
		//u32 x_offset = 150;

		//First one will be the current
		Transform transform;
		transform.position = { 1000.f, 100.f, 0.f };
		transform.scale = { 100.f, 100.f };

		ECS::CreateButton(GUI_Pattern_Button_Entities[0]);
		ECS::CreateButton(GUI_Pattern_Button_Entities[1]);
		ECS::CreateButton(GUI_Pattern_Button_Entities[2]);
	}

	void GameplaySystem_Interface_Management_GUI::InitializeAbilitiesGUI(std::vector<ECS::Entity>& GUI_Abilities_Button_Entities) {
		//Clear GUI
		GUI_Abilities_Button_Entities.clear();

		//There will be a fix of 4 buttons
		for (int i = 1; i <= 6; ++i) {
			GUI_Abilities_Button_Entities.push_back(Coordinator::Instance()->GetEntityByTag("skill_icon" + std::to_string(i)));
		}

		//Set base x
		//u32 x_offset = 75;

		//Start pos
		Engine::Vector3 startPos = { 50.f, 100.f, 0.f };

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
			ECS::Subscribe(GUI_Abilities_Button_Entities[i], EVENT_TRIGGER_TYPE::ON_POINTER_ENTER, Script::Event_Button_Enter_Ability_GUI);
			ECS::Subscribe(GUI_Abilities_Button_Entities[i], EVENT_TRIGGER_TYPE::ON_POINTER_EXIT, Script::Event_Button_Exit_Ability_GUI);
		}

		for (int i = 0; i < 6; ++i) {
			EventTrigger& eventTrigger = Coordinator::Instance()->GetComponent<EventTrigger>(guiManager.GUI_Abilities_Button_List[i]);
			//Sprite& sprite = Coordinator::Instance()->GetComponent<Sprite>(guiManager.GUI_Abilities_Button_List[i]);

			eventTrigger.isEnabled = true;
		}
	}
	
	void GameplaySystem_Interface_Management_GUI::TogglePatternGUI(b8 istrue) {
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

	void GameplaySystem_Interface_Management_GUI::TogglePatternFirstOnlyGUI(b8 istrue) {
		TogglePatternGUI(false);

		//Just toggle the first
		EventTrigger& eventTrigger = Coordinator::Instance()->GetComponent<EventTrigger>(guiManager.GUI_Pattern_Button_List[0]);
		Sprite& sprite = Coordinator::Instance()->GetComponent<Sprite>(guiManager.GUI_Pattern_Button_List[0]);

		eventTrigger.isEnabled = istrue;

		if (istrue)
			sprite.color = { 1.f, 1.f, 1.f, 1.f };
		else
			sprite.color = { 0.1f, 0.1f, 0.1f, 1.f };
	}


	void GameplaySystem_Interface_Management_GUI::ToggleAbilitiesGUI(b8 istrue) {
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

		Update_Ability_Cooldown(gameplaySystem->Abilities_List, istrue);
	}


	void GameplaySystem_Interface_Management_GUI::DisableToolTipGUI()
	{
		ECS::Entity en_tooltip = Coordinator::Instance()->GetEntityByTag("tooltip_skills");
		ECS::SetActive(false, en_tooltip);
	}

	void GameplaySystem_Interface_Management_GUI::GuiUpdatePhaseIndicator(PHASE_STATUS status)
	{
		Text& phaseIndicator = Coordinator::Instance()->GetComponent<Text>(getGuiManager().Phase_Indicator);
		phaseIndicator.colour = Engine::Vector3(1.f, 1.f, 1.f);

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
			phaseIndicator.colour = Engine::Vector3(1.f, 0.32f, 0.28f);
			break;
		}
	}


	void GameplaySystem_Interface_Management_GUI::Update_AP_UI(int AP_count) {
		//Disable all
		for (int i = 0; i < guiManager.AP_Indicators.size(); ++i) {
			Sprite& sprite = Coordinator::Instance()->GetComponent<Sprite>(guiManager.AP_Indicators[i]);
			sprite.id = Engine::AssetManager::Instance()->GetGuid("Assets/Images/AP_Empty.png");
		}

		//Enable set amount
		for (int i = 0; i < AP_count; ++i) {
			Sprite& sprite = Coordinator::Instance()->GetComponent<Sprite>(guiManager.AP_Indicators[i]);
			sprite.id = Engine::AssetManager::Instance()->GetGuid("Assets/Images/AP_Green.png");
		}
	}

	void GameplaySystem_Interface_Management_GUI::Update_AP_UI_For_Cost(int AP_count, int AP_Cost) {
		//Disable all
		for (int i = 0; i < guiManager.AP_Indicators.size(); ++i) {
			Sprite& sprite = Coordinator::Instance()->GetComponent<Sprite>(guiManager.AP_Indicators[i]);
			sprite.id = Engine::AssetManager::Instance()->GetGuid("Assets/Images/AP_Empty.png");
		}

		//Enable set amount
		for (int i = 0; i < AP_count - AP_Cost; ++i) {
			Sprite& sprite = Coordinator::Instance()->GetComponent<Sprite>(guiManager.AP_Indicators[i]);
			sprite.id = Engine::AssetManager::Instance()->GetGuid("Assets/Images/AP_Green.png");
		}

		//Enable set amount
		for (int i = AP_count - AP_Cost; i < AP_count; ++i) {
			int index = i < 0 ? 0 : i;
			Sprite& sprite = Coordinator::Instance()->GetComponent<Sprite>(guiManager.AP_Indicators[index]);
			sprite.id = Engine::AssetManager::Instance()->GetGuid("Assets/Images/AP_Red.png");
		}
	}

	void GameplaySystem_Interface_Management_GUI::Update_Skill_Tip_Position()
	{
		EntityData& en = Coordinator::Instance()->GetComponent<EntityData>(gameplaySystem_GUI->guiManager.Tooltip_Skills_Card);
		if (en.active == false)
			return;

		Transform& trans = Coordinator::Instance()->GetComponent<Transform>(gameplaySystem_GUI->guiManager.Tooltip_Skills_Card);
		trans.position = Input::GetMouseWorldPos() + Math::Vec2(0.f, 290.f);
	}

	void GameplaySystem_Interface_Management_GUI::Update_Ability_Cooldown(std::vector<Abilities> ability_set, bool isAbilityGUIActive) {
		for (int i = 0; i < guiManager.GUI_Abilities_Button_List.size(); ++i) {
			Sprite& sprite = Coordinator::Instance()->GetComponent<Sprite>(guiManager.GUI_Abilities_Button_List[i]);
			EventTrigger& eventTrigger = Coordinator::Instance()->GetComponent<EventTrigger>(guiManager.GUI_Abilities_Button_List[i]);
			Text& guiBtn_Text = Coordinator::Instance()->GetComponent<Text>(guiManager.GUI_Abilities_Button_List[i]);

			guiBtn_Text.textString = std::to_string(ability_set[i].current_Cooldown);

			if (ability_set[i].current_Cooldown == 0) {
				guiBtn_Text.scale = 0;
			}
			else {
				guiBtn_Text.scale = 1.2f;
			}

			if (ability_set[i].current_Cooldown == 0 && isAbilityGUIActive) {
				sprite.color = { 1.f, 1.f, 1.f, 1.f };
				eventTrigger.isEnabled = true;
			}
			else {
				eventTrigger.isEnabled = false;
				sprite.color = { 0.1f, 0.1f, 0.1f, 1.f };
			}
		}//End for loop
	}

	void Event_Button_Enter_Ability_GUI(ECS::Entity invoker) 
	{
		if (utils::IsEqual(Time::m_Scale, 0.f)) {
			return;
		}
		ECS::SetActive(true, gameplaySystem_GUI->guiManager.Tooltip_Skills_Card);
		EntityData skill = Coordinator::Instance()->GetComponent<EntityData>(invoker);

		Transform& trans = Coordinator::Instance()->GetComponent<Transform>(gameplaySystem_GUI->guiManager.Tooltip_Skills_Card);
		Sprite& sprite = Coordinator::Instance()->GetComponent<Sprite>(gameplaySystem_GUI->guiManager.Tooltip_Skills_Card);

		trans.position = Input::GetMouseWorldPos() + Math::Vec2(0.f, 290.f);
		if (skill.tag == "skill_icon1")
		{
			sprite.id = Engine::AssetManager::Instance()->GetGuid("Assets/Images/Tooltip_Skills_HardDrop.png");
		}
		else if (skill.tag == "skill_icon2")
		{
			sprite.id = Engine::AssetManager::Instance()->GetGuid("Assets/Images/Tooltip_Skills_LifeDrain.png");
		}
		else if (skill.tag == "skill_icon3")
		{
			sprite.id = Engine::AssetManager::Instance()->GetGuid("Assets/Images/Tooltip_Skills_Overhang.png");
		}
		else if (skill.tag == "skill_icon4")
		{
			sprite.id = Engine::AssetManager::Instance()->GetGuid("Assets/Images/Tooltip_Skills_ConstructTile.png");
		}
		else if (skill.tag == "skill_icon5")
		{
			sprite.id = Engine::AssetManager::Instance()->GetGuid("Assets/Images/Tooltip_Skills_MatrixTrap.png");
		}
		else if (skill.tag == "skill_icon6")
		{
			sprite.id = Engine::AssetManager::Instance()->GetGuid("Assets/Images/Tooltip_Skills_VolaTile.png");
		}
	}

	void Event_Button_Exit_Ability_GUI([[maybe_unused]] ECS::Entity invoker) {
		if (utils::IsEqual(Time::m_Scale, 0.f)) {
			return;
		}

		gameplaySystem_GUI->DisableToolTipGUI();
	}
}