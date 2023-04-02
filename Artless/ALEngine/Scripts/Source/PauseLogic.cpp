/*!
file:	PauseLogic.cpp
author:	Wong Man Cong
email:	w.mancong\@digipen.edu
brief:	This file contain function declaration for a pause menu

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#include <pch.h>
#include <PauseLogic.h>
#include <PauseButtonFlag.h>
#include <Engine/GSM/GameStateManager.h>
#include <SceneManager/CutsceneManager.h>
#include <GameAudioManager.h>
#include <ECS/Components/Text.h>

namespace ALEngine::Script
{
	namespace
	{
		using namespace ECS;

		u64 constexpr const TEXTS{ 6 };
		Entity bd_pause{ ECS::MAX_ENTITIES };
		Text* skill_text[TEXTS]{};
	}

	void PauseLogic::Init(ECS::Entity en)
	{
		GetSceneGraph().FindImmediateChildren(en);
		std::vector<s32> const& children = GetSceneGraph().GetChildren();

		for (s32 child : children)
		{
			EntityData const& ed = Coordinator::Instance()->GetComponent<EntityData>(static_cast<Entity>(child));
			if (ed.tag == "bd_pause")
				bd_pause = static_cast<Entity>(child);
		}

		SetActive(false, bd_pause);	// to make sure that bd_pause is always false when init

		text_bar_hp = Coordinator::Instance()->GetEntityByTag("text_bar_hp");
		for (u64 i{}; i < TEXTS; ++i)
		{
			std::ostringstream oss{};
			oss << "skill_icon" << i + 1;
			*(skill_text + i) = &Coordinator::Instance()->GetComponent<Text>( Coordinator::Instance()->GetEntityByTag( oss.str() ) );
		}
	}

	void PauseLogic::Update(ECS::Entity en)
	{
		if (Engine::Scene::CutsceneManager::Instance()->CutsceneIsPlaying())
			return;
		if (Input::KeyTriggered(KeyCode::Escape) && !PauseButtonFlag::confirmationBG)
		{
			b8 active = Coordinator::Instance()->GetComponent<EntityData>(bd_pause).active;
			SetActive(!active, bd_pause);
			Time::m_Scale = static_cast<f32>(active);

			if (!active)
			{
				ParticleSystem::GetParticleSystem().pauseRender = true;
				GameAudioManager::Play("MenuOpen");
			}
			else
			{
				ParticleSystem::GetParticleSystem().pauseRender = false;
				GameAudioManager::Play("MenuClose");
			}
		}
		SetActive(static_cast<b8>(Time::m_Scale), text_bar_hp);
		for (u64 i{}; i < TEXTS; ++i)
			skill_text[i]->active = static_cast<b8>(Time::m_Scale);
	}

	void PauseLogic::Free(ECS::Entity en)
	{
		bd_pause = ECS::MAX_ENTITIES;
		memset(skill_text, 0, sizeof(skill_text));
	}
}