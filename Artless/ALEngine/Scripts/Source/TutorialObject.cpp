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

namespace ALEngine::Script
{
	namespace
	{
		using namespace ECS;
	}

	void TutorialObject::Init(ECS::Entity en)
	{
		m_TutorialObject = en;
		Gameplay::TutorialManager::Instance()->SetTutorialObject(en);

		ECS::GetSceneGraph().FindImmediateChildren(m_TutorialObject);
		std::vector<s32> const& children = ECS::GetSceneGraph().GetChildren();

		// Check children for Tutorial Object
		for (s32 child : children)
		{
			EntityData const& data = Coordinator::Instance()->GetComponent<EntityData>(child);

			if (data.tag == "Select Tile Arrow")
			{
				m_SelectTile = child;
				ECS::SetActive(false, m_SelectTile);
			}
		}

		ECS::GetSceneGraph().FindImmediateChildren(m_SelectTile);
		std::vector<s32> const& childrenSelectChild = ECS::GetSceneGraph().GetChildren();
		// Check children for Tutorial Object
		for (s32 child : childrenSelectChild)
		{
			EntityData const& data = Coordinator::Instance()->GetComponent<EntityData>(child);

			if (data.tag == "Arrow")
				m_SelectTile_Arrow = child;
			else if (data.tag == "Bloom")
				m_SelectTile_Bloom = child;
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

		}
	}
}