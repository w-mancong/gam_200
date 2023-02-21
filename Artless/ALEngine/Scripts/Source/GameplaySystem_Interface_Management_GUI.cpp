#include <pch.h>
#include <GameplaySystem_Interface_Management_GUI.h>
#include <GameplaySystem.h>
#include <Engine/Gameplay_Interface.h>
#include <GameplaySystem_Interface_Management_Enemy.h>

namespace ALEngine::Script
{
	namespace {
		std::shared_ptr<GameplaySystem> gameplaySystem;
	}


	void GameplaySystem_Interface_Management_GUI::Load(ECS::Entity en)
	{
		gameplaySystem = ECS::GetLogicComponent<GameplaySystem>(en);
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
		Sprite& profile = Coordinator::Instance()->GetComponent<Sprite>(getGuiManager().Unit_Profile);

		health_text.textString = std::to_string(unit.health) + "/" + std::to_string(unit.maxHealth);
		attack_text.textString = std::to_string(unit.minDamage) + "/" + std::to_string(unit.maxDamage);
		defense_text.textString = std::to_string(unit.defense);
		movement_text.textString = std::to_string(unit.movementPoints) + "/" + std::to_string(unit.maxMovementPoints);
		range_text.textString = std::to_string(unit.minRange);
		name_text.textString = unit.unit_Name;

		profile.id = Engine::AssetManager::Instance()->GetGuid(unit.unit_Profile_Sprite_File);

		Transform& healthbar_transform = Coordinator::Instance()->GetComponent<Transform>(getGuiManager().Unit_Healthbar);
		healthbar_transform.localScale.x = (unit.health <= 0 ? 0 : ((f32)unit.health / (f32)unit.maxHealth));
	}

	void GameplaySystem_Interface_Management_GUI::InitializeGUI()
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

	void GameplaySystem_Interface_Management_GUI::UpdateFpsLabel()
	{
		Text& fps = Coordinator::Instance()->GetComponent<Text>(getGuiManager().FPS_Label);
		if (guiManager.fpsActive)
		{
			AL_CORE_CRITICAL("DISPLAY FPS");
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
}