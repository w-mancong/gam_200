#include <pch.h>
#include <GameplaySystem_Interface_Management_GUI.h>
#include <GameplaySystem.h>
#include <Engine/Gameplay_Interface.h>
#include <GameplaySystem_Interface_Management_Enemy.h>

namespace ALEngine::Script
{
	namespace {
		std::shared_ptr<GameplaySystem> gameplaySystem;
		std::shared_ptr<GameplaySystem_Interface_Management_GUI> gameplaySystem_GUI;
	}


	void GameplaySystem_Interface_Management_GUI::Load(ECS::Entity en)
	{
		gameplaySystem = ECS::GetLogicComponent<GameplaySystem>(en);
		gameplaySystem_GUI = ECS::GetLogicComponent<GameplaySystem_Interface_Management_GUI>(en);
		Set_GameplayInterface_GUI(this);
	}

	void GameplaySystem_Interface_Management_GUI::Init(ECS::Entity en){

	}

	void GameplaySystem_Interface_Management_GUI::Update(ECS::Entity en)
	{

	}

	void GameplaySystem_Interface_Management_GUI::LateUpdate(ECS::Entity en)
	{

	}

	void GameplaySystem_Interface_Management_GUI::Free(ECS::Entity en)
	{

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
		Text& range_text = Coordinator::Instance()->GetComponent<Text>(getGuiManager().Unit_Range);
		//Sprite& profile = Coordinator::Instance()->GetComponent<Sprite>(getGuiManager().Unit_Profile);

		health_text.textString = std::to_string(unit.health) + "/" + std::to_string(unit.maxHealth);
		attack_text.textString = std::to_string(unit.minDamage) + "/" + std::to_string(unit.maxDamage);
		defense_text.textString = std::to_string(unit.defense);
		movement_text.textString = std::to_string(unit.movementPoints) + "/" + std::to_string(unit.maxMovementPoints);
		range_text.textString = std::to_string(unit.minRange);
		name_text.textString = unit.unit_Name;

		//profile.id = Engine::AssetManager::Instance()->GetGuid(unit.unit_Profile_Sprite_File);

		Transform& healthbar_transform = Coordinator::Instance()->GetComponent<Transform>(getGuiManager().Unit_Healthbar);
		healthbar_transform.localScale.x = (unit.health <= 0 ? 0 : ((f32)unit.health / (f32)unit.maxHealth));
	}

	void GameplaySystem_Interface_Management_GUI::InitializeGUI()
	{
		//Initialize GUI Text and Sprites zafir2
		guiManager.Unit_Name = Coordinator::Instance()->GetEntityByTag("text_playername");
		guiManager.Unit_Health = Coordinator::Instance()->GetEntityByTag("text_bar_hp");
		//guiManager.Unit_Profile = Coordinator::Instance()->GetEntityByTag("profile_player");
		guiManager.Unit_Attack = Coordinator::Instance()->GetEntityByTag("text_attack_output");
		guiManager.Unit_Defense = Coordinator::Instance()->GetEntityByTag("text_defense_output");
		guiManager.Unit_Movement = Coordinator::Instance()->GetEntityByTag("text_move_output");
		guiManager.Unit_Range = Coordinator::Instance()->GetEntityByTag("text_range_output");
		guiManager.Unit_Healthbar = Coordinator::Instance()->GetEntityByTag("red_health_bar");
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
		guiManager.Pause_Button = Coordinator::Instance()->GetEntityByTag("pause_button");
		guiManager.AP_Indicators[0] = Coordinator::Instance()->GetEntityByTag("AP1");
		guiManager.AP_Indicators[1] = Coordinator::Instance()->GetEntityByTag("AP2");
		guiManager.AP_Indicators[2] = Coordinator::Instance()->GetEntityByTag("AP3");
		guiManager.AP_Indicators[3] = Coordinator::Instance()->GetEntityByTag("AP4");
		guiManager.AP_Indicators[4] = Coordinator::Instance()->GetEntityByTag("AP5");
		guiManager.AP_Indicators[5] = Coordinator::Instance()->GetEntityByTag("AP6");
		guiManager.Highlight_blocks[0] = Coordinator::Instance()->GetEntityByTag("Highlight_Path1");
		guiManager.Highlight_blocks[1] = Coordinator::Instance()->GetEntityByTag("Highlight_Path2");
		guiManager.Highlight_blocks[2] = Coordinator::Instance()->GetEntityByTag("Highlight_Path3");
		guiManager.Highlight_blocks[3] = Coordinator::Instance()->GetEntityByTag("Highlight_Path4");
		guiManager.Highlight_blocks[4] = Coordinator::Instance()->GetEntityByTag("Highlight_Path5");
		guiManager.Highlight_blocks[5] = Coordinator::Instance()->GetEntityByTag("Highlight_Path6");

		ECS::SetActive(false, guiManager.endTurnBtnEntity);
		ECS::SetActive(false, guiManager.Win_Clear);

		ECS::CreateButton(guiManager.Win_Button);
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
		for (int i = 1; i <= 4; ++i) {
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
		ECS::CreateButton(GUI_Pattern_Button_Entities[3]);
	}

	void GameplaySystem_Interface_Management_GUI::InitializeAbilitiesGUI(std::vector<ECS::Entity>& GUI_Abilities_Button_Entities) {
		//Clear GUI
		GUI_Abilities_Button_Entities.clear();

		//There will be a fix of 4 buttons
		for (int i = 1; i <= 6; ++i) {
			GUI_Abilities_Button_Entities.push_back(Coordinator::Instance()->GetEntityByTag("skill_icon" + std::to_string(i)));
			//GUI_Abilities_Button_Entities.push_back(Coordinator::Instance()->CreateEntity());
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

		for (int i = 0; i < 3; ++i) {
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

	void Event_Button_Enter_Ability_GUI(ECS::Entity invoker) {
		if (utils::IsEqual(Time::m_Scale, 0.f)) {
			return;
		}

		ECS::SetActive(true, gameplaySystem_GUI->guiManager.Tooltip_Skills_Card);
		EntityData skill = Coordinator::Instance()->GetComponent<EntityData>(invoker);
		Sprite& sprite = Coordinator::Instance()->GetComponent<Sprite>(gameplaySystem_GUI->guiManager.Skill_Tip_Icon);
		Text& headerText = Coordinator::Instance()->GetComponent<Text>(gameplaySystem_GUI->guiManager.Skill_Tip_Header);
		Text& line1 = Coordinator::Instance()->GetComponent<Text>(gameplaySystem_GUI->guiManager.Skill_Tip_Line1);
		Text& line2 = Coordinator::Instance()->GetComponent<Text>(gameplaySystem_GUI->guiManager.Skill_Tip_Line2);
		Text& line3 = Coordinator::Instance()->GetComponent<Text>(gameplaySystem_GUI->guiManager.Skill_Tip_Line3);
		Text& line4 = Coordinator::Instance()->GetComponent<Text>(gameplaySystem_GUI->guiManager.Skill_Tip_Line4);

		if (skill.tag == "skill_icon1")
		{
			sprite.id = Engine::AssetManager::Instance()->GetGuid("Assets/Images/HardDrop.png");
			headerText.textString = "Hard Drop";
			line1.textString = "Slam a Tetromino on enemies,";
			line2.textString = "dealing 15 DMG on each";
			line3.textString = "impacted enemy.";
			line4.textString = "";
		}
		else if (skill.tag == "skill_icon2")
		{
			sprite.id = Engine::AssetManager::Instance()->GetGuid("Assets/Images/LifeDrain.png");
			headerText.textString = "Life Drain";
			line1.textString = "Deal DMG to enemies and heal";
			line2.textString = "for half of all DMG dealt.";
			line3.textString = "[Lifesteal is WIP]";
			line4.textString = "";
		}
		else if (skill.tag == "skill_icon3")
		{
			sprite.id = Engine::AssetManager::Instance()->GetGuid("Assets/Images/Icon_Skill_ConstructTile.png");
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

	void Event_Button_Exit_Ability_GUI([[maybe_unused]] ECS::Entity invoker) {
		if (utils::IsEqual(Time::m_Scale, 0.f)) {
			return;
		}

		gameplaySystem_GUI->DisableToolTipGUI();
	}
}