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

		ALEngine::Tree::BinaryTree& sceneGraph = ECS::GetSceneGraph();

		sceneGraph.FindImmediateChildren(m_TutorialObject);
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
		}

		sceneGraph.FindImmediateChildren(m_SelectTile);
		// Check children for Tutorial Object
		for (s32 child : sceneGraph.GetChildren())
		{
			EntityData const& data = Coordinator::Instance()->GetComponent<EntityData>(child);

			if (data.tag == "Arrow")
				m_SelectTile_Arrow = child;
			else if (data.tag == "Bloom")
				m_SelectTile_Bloom = child;
		}

		sceneGraph.FindImmediateChildren(m_PlaceFirstTile);
		// Check children for Tutorial Object
		for (s32 child : sceneGraph.GetChildren())
		{
			EntityData const& data = Coordinator::Instance()->GetComponent<EntityData>(child);

			if (data.tag == "Arrow")
				m_PlaceFirstTile_Arrow = child;
			else if (data.tag == "Click Here")
				m_PlaceFirstTile_ClickHere = child;
		}
	}

	void TutorialObject::Update(ECS::Entity en)
	{
		switch (Gameplay::TutorialManager::Instance()->GetState())
		{
		case Gameplay::TutorialState::TUTORIAL_SELECT_TILE:
			UpdateSelectTile();
			break;
		default:
			break;
		}
	}

	void TutorialObject::UpdateSelectTile(void)
	{
		if (Gameplay::TutorialManager::Instance()->GetTileIsSelected() == false)
		{	// If tile is not selected, show arrow
			ECS::SetActive(true, m_SelectTile);
			ECS::SetActive(false, m_PlaceFirstTile);

			const f32 arrowMax{ 250.f }, arrowMin{ 200.f };
			const f32 scaleSpeed{ 35.f }, bloomSpeed{ 0.7f };

			Transform& arrowXform = Coordinator::Instance()->GetComponent<Transform>(m_SelectTile_Arrow);

			if (m_SelectTile_ArrowIsShrink)
			{
				arrowXform.scale.x -= (scaleSpeed * Time::m_DeltaTime);
				arrowXform.scale.y = arrowXform.scale.x;

				if (arrowXform.scale.x <= arrowMin)
					m_SelectTile_ArrowIsShrink = false;
			}
			else
			{
				arrowXform.scale.x += (scaleSpeed * Time::m_DeltaTime);
				arrowXform.scale.y = arrowXform.scale.x;

				if (arrowXform.scale.x >= arrowMax)
					m_SelectTile_ArrowIsShrink = true;
			}

			static f32 bloomDir{ 1.f };
			Sprite& bloomSpr = Coordinator::Instance()->GetComponent<Sprite>(m_SelectTile_Bloom);
			bloomSpr.color.a += (bloomDir * Time::m_DeltaTime * bloomSpeed);

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
		}

		if (Gameplay::TutorialManager::Instance()->GetTileIsPlaced())
		{
			Gameplay::TutorialManager::Instance()->NextState();
			Gameplay::TutorialManager::Instance()->SetTileIsPlaced(false);
			ECS::SetActive(false, m_SelectTile);
			ECS::SetActive(false, m_PlaceFirstTile);
		}
	}
}