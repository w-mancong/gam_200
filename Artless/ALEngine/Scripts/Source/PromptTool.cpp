/*!
file:	PromptTool.cpp
author:	Wong Man Cong
email:	w.mancong\@digipen.edu
brief:	This file contain function definition for generating and moving water background

		All content � 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#include <pch.h>
#include <PromptTool.h>
#include <GameplaySystem.h>

namespace ALEngine::Script
{
	namespace
	{
		using namespace ECS;
	}

	b8 PromptTool::showPrompt{ true };

	void PromptTool::Init([[maybe_unused]] ECS::Entity en)
	{
		mouse_sprite = Coordinator::Instance()->CreateEntity();
		Coordinator::Instance()->GetComponent<EntityData>(mouse_sprite).tag = "prompt_sprites";

		Transform trans_{};
		trans_.scale = { 125.0f, 125.0f };
		trans_.position = { 800.0f, 450.0f };
		Coordinator::Instance()->AddComponent(mouse_sprite, trans_);
		trans = &Coordinator::Instance()->GetComponent<Transform>(mouse_sprite);

		CreateSprite(mouse_sprite, "Assets\\Images\\KeycodePrompt.png");
		sprite = &Coordinator::Instance()->GetComponent<Sprite>(mouse_sprite);
		sprite->layer = 3250;
		sprite->color = { 1.0f, 1.0f, 1.0f, 0.0f };

		ECS::GetSceneGraph().Push(-1, mouse_sprite);
	}

	void PromptTool::Update([[maybe_unused]] ECS::Entity en)
	{
		if (Input::KeyTriggered(KeyCode::Tab))
			showPrompt = !showPrompt;

		if (!showPrompt)
		{
			timer = sprite->color.a = 0.0f;
			return;
		}

		if (*currentPatternPlacementStatus != PATTERN_PLACEMENT_STATUS::NOTHING)
		{
			timer += Time::m_DeltaTime;
			if (timer < WAIT_TIME)
				return;

			sprite->color.a += Time::m_DeltaTime * SPEED;
			sprite->color.a = std::clamp(sprite->color.a, 0.0f, 1.0f);

			trans->position = Input::GetMouseWorldPos();
		}
		else
		{
			if(sprite != nullptr)
				timer = sprite->color.a = 0.0f;
		}
	}

	void PromptTool::InitPatternPlacementStatusVariable(PATTERN_PLACEMENT_STATUS* pattern)
	{
		currentPatternPlacementStatus = pattern;
	}
}