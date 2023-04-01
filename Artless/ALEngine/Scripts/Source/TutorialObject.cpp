/*!
file:	TutorialObject.cpp
author:	Lucas Nguyen
email:	l.nguyen\@digipen.edu
brief:	This file contain function definition for the tutorial object.
		The tutorial object contains multiple objects that are used for different
		states of the tutorial.

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#include <pch.h>
#include <TutorialObject.h>
#include <GameplaySystem.h>
#include <Engine/Gameplay_Interface.h>

namespace ALEngine::Script
{
	namespace
	{
		using namespace ECS;
		std::shared_ptr<GameplaySystem> gs{ nullptr };
	}

	void TutorialObject::Init(ECS::Entity en)
	{
		m_TutorialObject = en;
		Gameplay::TutorialManager::Instance()->SetTutorialObject(en);

		// Variables Init
		m_EndTurn_ArrowBool = true;

		ALEngine::Tree::BinaryTree& sceneGraph = ECS::GetSceneGraph();

		sceneGraph.FindImmediateChildren(static_cast<s32>(m_TutorialObject));
		std::vector<s32> const& children = sceneGraph.GetChildren();

		// Set gameplay system
		gs = Gameplay::TutorialManager::Instance()->GetGameplaySystem();
		// Make the first one an |
		gs->pattern_List.insert(gs->pattern_List.begin(), gs->pattern_Default[1]);
		
		// Set sprites for the Patterns
		for (u32 i{ 1 }; i <= 4; ++i)
		{
			std::string tile_icon = "next_tile_icon" + std::to_string(i);

			ECS::Entity tileEtt = Coordinator::Instance()->GetEntityByTag(tile_icon);

			Sprite& sprite = Coordinator::Instance()->GetComponent<Sprite>(tileEtt);
			sprite.id = Engine::AssetManager::Instance()->GetGuid(gs->pattern_List[i - 1].file_path);
		}

		// Check children for Tutorial Object
		for (s32 child : children)
		{
			EntityData const& data = Coordinator::Instance()->GetComponent<EntityData>(child);

			if (data.tag == "Select Tile Arrow")
			{
				m_SelectTile = child;
				ECS::SetActive(false, m_SelectTile);
			}
			else if (data.tag == "Place First Tile")
			{
				m_PlaceFirstTile = child;
				ECS::SetActive(false, m_PlaceFirstTile);
			}
			else if(data.tag == "Show AP")
			{
				m_ShowAP = child;
				ECS::SetActive(false, m_ShowAP);
			}
			else if (data.tag == "Walk To Enemy")
			{
				m_WalkToEnemy = child;
				ECS::SetActive(false, m_WalkToEnemy);
			}
			else if (data.tag == "End Turn Arrow")
			{
				m_EndTurn_Arrow = child;
				ECS::SetActive(false, m_EndTurn_Arrow);
			}
			else if (data.tag == "Hard Drop")
			{
				m_HardDrop = child;
				ECS::SetActive(false, m_HardDrop);
			}
			else if (data.tag == "Construct Tile")
			{
				m_ConstructTile = child;
				ECS::SetActive(false, m_ConstructTile);
			}
		}

		// Select Tile Children
		sceneGraph.FindImmediateChildren(static_cast<s32>(m_SelectTile));
		for (s32 child : sceneGraph.GetChildren())
		{
			EntityData const& data = Coordinator::Instance()->GetComponent<EntityData>(child);

			if (data.tag == "Arrow")
				m_SelectTile_Arrow = child;
			else if (data.tag == "Bloom")
				m_SelectTile_Bloom = child;
		}

		// Place First Tile Children
		sceneGraph.FindImmediateChildren(static_cast<s32>(m_PlaceFirstTile));
		for (s32 child : sceneGraph.GetChildren())
		{
			EntityData const& data = Coordinator::Instance()->GetComponent<EntityData>(child);

			if (data.tag == "Arrow")
				m_PlaceFirstTile_Arrow = child;
			else if (data.tag == "Click Here")
				m_PlaceFirstTile_ClickHere = child;
		}

		// Place First Tile Children
		sceneGraph.FindImmediateChildren(static_cast<s32>(m_ShowAP));
		for (s32 child : sceneGraph.GetChildren())
		{
			EntityData const& data = Coordinator::Instance()->GetComponent<EntityData>(child);

			if (data.tag == "AP1")
				m_APs[0] = child;
			else if (data.tag == "AP2")
				m_APs[1] = child;
			else if (data.tag == "AP3")
				m_APs[2] = child;
			else if (data.tag == "AP4")
				m_APs[3] = child;
			else if (data.tag == "AP5")
				m_APs[4] = child;
			else if (data.tag == "AP6")
				m_APs[5] = child;
			else if (data.tag == "Arrow")
				m_APs_Arrow = child;
		}

		// Walk To Enemy Children
		sceneGraph.FindImmediateChildren(static_cast<s32>(m_WalkToEnemy));
		for (s32 child : sceneGraph.GetChildren())
		{
			EntityData const& data = Coordinator::Instance()->GetComponent<EntityData>(child);

			if (data.tag == "Click Here")
				m_WalkToEnemy_ClickHere = child;
			else if (data.tag == "Arrow")
				m_WalkToEnemy_Arrow = child;
		}

		// Hard Drop Children
		sceneGraph.FindImmediateChildren(static_cast<s32>(m_HardDrop));
		for (s32 child : sceneGraph.GetChildren())
		{
			EntityData const& data = Coordinator::Instance()->GetComponent<EntityData>(child);

			if (data.tag == "Click Here")
				m_HardDrop_ClickHere = child;
			else if (data.tag == "Arrow")
				m_HardDrop_Arrow_1 = child;
			else if (data.tag == "Arrow Blocks")
				m_HardDrop_Arrow_2 = child;
			else if (data.tag == "Blocks")
				m_HardDrop_Blocks = child;
		}

		// Construct Tile Children
		sceneGraph.FindImmediateChildren(static_cast<s32>(m_ConstructTile));
		for (s32 child : sceneGraph.GetChildren())
		{
			EntityData const& data = Coordinator::Instance()->GetComponent<EntityData>(child);

			if (data.tag == "Click Here")
				m_ConstructTile_ClickHere = child;
			else if (data.tag == "Arrow")
				m_ConstructTile_Arrow = child;
		}
	}

	void TutorialObject::Update(ECS::Entity en)
	{
		const Unit player = Coordinator::Instance()->GetComponent<Unit>(Gameplay::TutorialManager::Instance()->GetPlayerEntity());

		switch (Gameplay::TutorialManager::Instance()->GetState())
		{
		case Gameplay::TutorialState::TUTORIAL_SELECT_TILE:
			UpdateSelectTile();
			break;
		case Gameplay::TutorialState::TUTORIAL_ACTION_PHASE_CS:
			UpdateAP_CS();
			break;
		case Gameplay::TutorialState::TUTORIAL_ACTION_PHASE_WALK:
			UpdateWalkToEnemy();
			break;
		case Gameplay::TutorialState::TUTORIAL_MELEE_END_TURN:
			UpdateEndTurn();
			break;
		case Gameplay::TutorialState::TUTORIAL_SETUP_2:
			UpdateSetUpPhaseWait();
			break;
		case Gameplay::TutorialState::TUTORIAL_HARD_DROP:
			UpdateHardDrop();
			break;
		case Gameplay::TutorialState::TUTORIAL_ATTACK:
			UpdateAttack();
			break;
		case Gameplay::TutorialState::TUTORIAL_CONSTRUCT_TILE:
			UpdateConstructTile();
			break;
		case Gameplay::TutorialState::TUTORIAL_PLACE_CONSTRUCT_TILE:
			UpdatePlaceConstructTile();
			break;



		case Gameplay::TutorialState::TUTORIAL_WAIT_GUARD_DEFEAT:
			Gameplay::TutorialManager::Instance()->SetAllAbilitiesButHardDropOff();
			if (Gameplay::TutorialManager::Instance()->GetNumEnemiesKilled() == 1
				&& player.coordinate[0] >= 11)
			{
				Gameplay::TutorialManager::Instance()->NextState();
			}
			break;
		case Gameplay::TutorialState::TUTORIAL_WAIT_TILE_DESTROYER_DEFEAT:
			Gameplay::TutorialManager::Instance()->SetAllAbilitiesButHardDropOff();
			if (Gameplay::TutorialManager::Instance()->GetNumEnemiesKilled() == 2
				&& player.coordinate[0] >= 18)
				Gameplay::TutorialManager::Instance()->NextState();
			break;
		case Gameplay::TutorialState::TUTORIAL_FINAL_FIGHT:
			if (gs->enemyEntityList.size() == Gameplay::TutorialManager::Instance()->GetNumEnemiesKilled())
				Gameplay::TutorialManager::Instance()->NextState();
			break;
		default:
			break;
		}
	}

	void TutorialObject::UpdateSelectTile(void)
	{
		constexpr f32 bloomSpeed{ 0.7f };

		if (Gameplay::TutorialManager::Instance()->GetTileIsSelected() == false)
		{	// If tile is not selected, show arrow
			ECS::SetActive(true, m_SelectTile);
			ECS::SetActive(false, m_PlaceFirstTile);

			VariableScale(m_SelectTile_Arrow);

			static f32 bloomDir{ 1.f };
			Sprite& bloomSpr = Coordinator::Instance()->GetComponent<Sprite>(m_SelectTile_Bloom);
			bloomSpr.color.a += (bloomDir * Time::m_ActualDeltaTime * bloomSpeed);

			if (bloomSpr.color.a >= 1.f)
			{
				bloomSpr.color.a = 1.f;
				bloomDir = -1.f;
			}
			else if (bloomSpr.color.a <= 0.f)
			{
				bloomSpr.color.a = 0.f;
				bloomDir = 1.f;
			}
		}
		else
		{	// Tile not selected, show other 
			ECS::SetActive(false, m_SelectTile);
			ECS::SetActive(true, m_PlaceFirstTile);

			SetAllButOneTileInactive(7, 5);

			VariableScale(m_PlaceFirstTile_Arrow);
		}

		if (Gameplay::TutorialManager::Instance()->GetTileIsPlaced())
		{
			Gameplay::TutorialManager::Instance()->NextState();
			Gameplay::TutorialManager::Instance()->SetTileIsPlaced(false);
			ECS::SetActive(false, m_SelectTile);
			ECS::SetActive(false, m_PlaceFirstTile);
			SetAllTilesActive(true);
		}
	}

	void TutorialObject::UpdateAP_CS(void)
	{
		if (ALEngine::Engine::Scene::CutsceneManager::Instance()->GetCurrentCutsceneName() == "Action Points")
		{
			ECS::SetActive(true, m_ShowAP);

			// Scale arrow
			VariableScale(m_APs_Arrow);

			// Change alpha of the AP circle
			constexpr f32 bloomSpeed{ 0.7f };
			constexpr f32 min{ 0.3f }, max{ 1.f };

			static f32 bloomDir{ 1.f };

			Sprite& APspr = Coordinator::Instance()->GetComponent<Sprite>(m_APs[0]);
			APspr.color.a += (bloomDir * Time::m_ActualDeltaTime * bloomSpeed);

			if (APspr.color.a >= max)
			{
				APspr.color.a = max;
				bloomDir = -1.f;
			}
			else if (APspr.color.a <= min)
			{
				APspr.color.a = min;
				bloomDir = 1.f;
			}

			for (u8 i{ 1 }; i < 6; ++i)
			{
				Sprite& otherAPspr = Coordinator::Instance()->GetComponent<Sprite>(m_APs[i]);
				otherAPspr.color.a = APspr.color.a;
			}
		}
		else
			ECS::SetActive(false, m_ShowAP);
	}

	void TutorialObject::UpdateWalkToEnemy(void)
	{
		ECS::SetActive(true, m_WalkToEnemy);
		ECS::SetActive(false, m_ShowAP);

		Unit player = Coordinator::Instance()->GetComponent<Unit>(Gameplay::TutorialManager::Instance()->GetPlayerEntity());

		if (Gameplay::TutorialManager::Instance()->GetPlayerMoveFinished())
		{
			Gameplay::TutorialManager::Instance()->NextState();
			ECS::SetActive(false, m_WalkToEnemy);
			Gameplay::TutorialManager::Instance()->SetPlayerMoveFinished(false);
			//SetAllTilesActive(true);
			return;
		}

		//SetAllButOneTileInactive(9, 5);

		VariableScale(m_WalkToEnemy_Arrow);
	}

	void TutorialObject::UpdateEndTurn(void)
	{
		ECS::SetActive(m_EndTurn_ArrowBool, m_EndTurn_Arrow);

		VariableScale(m_EndTurn_Arrow);

		if (Gameplay::TutorialManager::Instance()->GetEndTurnPressed())
		{
			Gameplay::TutorialManager::Instance()->SetEndTurnPressed(false);
			m_EndTurn_ArrowBool = false;
		}

		if (m_EndTurn_ArrowBool == false && Gameplay::TutorialManager::Instance()->GetPlayerTurnStart())
		{
			m_EndTurn_EnemyAttacked = true;			
		}

		if (m_EndTurn_EnemyAttacked)
		{
			m_EndTurn_EnemyAttackedTimer -= Time::m_DeltaTime;

			if (m_EndTurn_EnemyAttackedTimer <= 0.f)
			{
				Gameplay::TutorialManager::Instance()->NextState();
				Gameplay::TutorialManager::Instance()->SetPlayerTurnStart(false);
				m_EndTurn_EnemyAttackedTimer = 2.f;
				m_EndTurn_EnemyAttacked = false;
			}
		}
	}

	void TutorialObject::UpdateSetUpPhaseWait(void)
	{
		if (Gameplay::TutorialManager::Instance()->GetTileIsPlaced())
		{
			Gameplay::TutorialManager::Instance()->NextState();
			Gameplay::TutorialManager::Instance()->SetTileIsPlaced(false);
		}
	}

	void TutorialObject::UpdateHardDrop(void)
	{
		Gameplay::TutorialManager::Instance()->SetAllAbilitiesButHardDropOff();
		ECS::SetActive(true, m_HardDrop);
		
		// If player selected Hard Drop
		if (gs->selected_Abilities != nullptr && gs->selected_Abilities->current_Ability_Name == ABILITY_NAME::HARD_DROP)
		{
			ECS::SetActive(false, m_HardDrop_ClickHere);
			ECS::SetActive(false, m_HardDrop_Arrow_1);
			ECS::SetActive(true, m_HardDrop_Arrow_2);
			ECS::SetActive(true, m_HardDrop_Blocks);

			VariableScale(m_HardDrop_Arrow_2);

			if (gs->currentPatternPlacementStatus == PATTERN_PLACEMENT_STATUS::PLACING_FOR_ABILITIES)
			{
				Gameplay::TutorialManager::Instance()->NextState();
				ECS::SetActive(false, m_HardDrop);
				return;
			}

		}
		else
		{	// No Selected Abilities
			ECS::SetActive(true, m_HardDrop_ClickHere);
			ECS::SetActive(true, m_HardDrop_Arrow_1);
			ECS::SetActive(false, m_HardDrop_Arrow_2);
			ECS::SetActive(false, m_HardDrop_Blocks);

			VariableScale(m_HardDrop_Arrow_1);
		}
	}

	void TutorialObject::UpdateAttack(void)
	{
		static f32 countdown{ 2.f };
		static b8 hasAttacked{ false };

		Gameplay::TutorialManager::Instance()->SetAllAbilitiesButHardDropOff();

		if (Gameplay::TutorialManager::Instance()->GetTileIsPlaced())
		{
			hasAttacked = true;
			Gameplay::TutorialManager::Instance()->SetTileIsPlaced(false);
		}

		if(hasAttacked)
		{
			countdown -= Time::m_DeltaTime;

			if(countdown <= 0.f)
			{
				Gameplay::TutorialManager::Instance()->NextState();
				Gameplay::TutorialManager::Instance()->SetTileIsPlaced(false);
				hasAttacked = false;
				countdown = 2.f;
			}
		}
	}

	void TutorialObject::UpdateConstructTile(void)
	{
		if (gs->currentPhaseStatus != PHASE_STATUS::PHASE_ACTION)
		{
			Unit player = Coordinator::Instance()->GetComponent<Unit>(Gameplay::TutorialManager::Instance()->GetPlayerEntity());
			if (player.actionPoints <= 2 && player.actionPoints > 0)
				Gameplay::TutorialManager::Instance()->GetGameplaySystem()->EndTurn();

			ECS::SetActive(false, m_ConstructTile);
			return;
		}

		ECS::SetActive(true, m_ConstructTile);

		Gameplay::TutorialManager::Instance()->SetAllAbilitiesButConstructTileOff();

		// Give player enough points for Construct Tile
		Unit& player = Coordinator::Instance()->GetComponent<Unit>(Gameplay::TutorialManager::Instance()->GetPlayerEntity());
		if (player.actionPoints < 2)
			player.actionPoints = 2;

		VariableScale(m_ConstructTile_Arrow);

		if (gs->selected_Abilities != nullptr &&
			gs->selected_Abilities->current_Ability_Name == ABILITY_NAME::CONSTRUCT_WALL)
		{
			if (gs->currentPatternPlacementStatus == PATTERN_PLACEMENT_STATUS::PLACING_FOR_ABILITIES)
			{
				Gameplay::TutorialManager::Instance()->NextState();
				ECS::SetActive(false, m_ConstructTile);
				return;
			}
			else
				ECS::SetActive(false, m_ConstructTile);
		}
	}

	void TutorialObject::UpdatePlaceConstructTile(void)
	{
		static f32 countdown{ 1.f };
		static b8 hasPlaced{ false };

		Gameplay::TutorialManager::Instance()->SetAllAbilitiesButConstructTileOff();

		if (Gameplay::TutorialManager::Instance()->GetTileIsPlaced())
			hasPlaced = true;

		if (hasPlaced)
		{
			countdown -= Time::m_DeltaTime;

			if (countdown <= 0.f)
			{
				Gameplay::TutorialManager::Instance()->NextState();
				hasPlaced = false;
				countdown = 1.f;
			}
		}
	}

	void TutorialObject::SetAllTilesActive(b8 value)
	{
		for(auto i : gs->m_Room.roomCellsArray)
		{
			if (Coordinator::Instance()->HasComponent<Cell>(i) == true)
			{
				if (Coordinator::Instance()->GetComponent<Cell>(i).m_isAccessible)
				{
					if (Coordinator::Instance()->HasComponent<EventTrigger>(i) == true)
					{
						EventTrigger& to_inactive = Coordinator::Instance()->GetComponent<EventTrigger>(i);
						to_inactive.isEnabled = value;
					}
				}
			}
		}
	}

	void TutorialObject::SetAllButOneTileInactive(u32 x, u32 y)
	{
		SetAllTilesActive(false);
		EventTrigger& to_inactive = Coordinator::Instance()->GetComponent<EventTrigger>(gs->getEntityCell(gs->m_Room, x, y));
		to_inactive.isEnabled = true;
	}

	
	void TutorialObject::VariableScale(ECS::Entity en)
	{
		const f32 max{ 250.f }, min{ 200.f };
		const f32 scaleSpeed{ 35.f };
		static b8 isShrink{ false };

		Transform& xform = Coordinator::Instance()->GetComponent<Transform>(en);

		if (isShrink)
		{
			xform.scale.x -= (scaleSpeed * Time::m_ActualDeltaTime);
			xform.scale.y = xform.scale.x;

			if (xform.scale.x <= min)
				isShrink = false;
		}
		else
		{
			xform.scale.x += (scaleSpeed * Time::m_ActualDeltaTime);
			xform.scale.y = xform.scale.x;

			if (xform.scale.x >= max)
				isShrink = true;
		}
	}
}