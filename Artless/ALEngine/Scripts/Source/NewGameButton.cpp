/*!
file:	NewGameButton.cpp
author:	Wong Man Cong
email:	w.mancong\@digipen.edu
brief:	This file contain function definition for new game button

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#include <pch.h>
#include <NewGameButton.h>
#include <SceneChangeHelper.h>
#include <PauseButtonFlag.h>
#include <TutorialManager.h>
#include <GameAudioManager.h>

namespace ALEngine::Script
{
	void SetMap(u64 index);

	namespace
	{
		using namespace ECS;
		f32 constexpr ALPHA_VALUE{ 0.925f };
		ECS::Entity scene_transition{ ECS::MAX_ENTITIES }, 
			tutorial_prompt{ ECS::MAX_ENTITIES }, yes{ ECS::MAX_ENTITIES }, no{ ECS::MAX_ENTITIES };
		b8 roomSet{ false };

		void ChangeScene(u64 roomIndex)
		{
			std::shared_ptr<SceneChangeHelper> ptr = GetLogicComponent<SceneChangeHelper>(scene_transition);
			ptr->NextScene();
			SetMap(roomIndex);
			roomSet = true;
		}

		void Darken(Entity en)
		{
			Sprite& sprite = Coordinator::Instance()->GetComponent<Sprite>(en);
			sprite.color = { 0.6f, 0.6f, 0.6f, 1.0f };
		}

		void Lighten(Entity en)
		{
			Sprite& sprite = Coordinator::Instance()->GetComponent<Sprite>(en);
			sprite.color = { 1.0f, 1.0f, 1.0f, 1.0f };
		}

		void WhenHover(Entity en)
		{
			if (ALPHA_VALUE > Coordinator::Instance()->GetComponent<Sprite>(en).color.a || PauseButtonFlag::confirmationBG)
				return;
			Darken(en);
			if (Input::KeyDown(KeyCode::MouseLeftButton))
			{
				SetActive(true, tutorial_prompt);
				PauseButtonFlag::confirmationBG = true;
				Lighten(en);
				GameAudioManager::Play("MenuButtonPress");
			}
		}

		void WhenExit(Entity en)
		{
			if (ALPHA_VALUE > Coordinator::Instance()->GetComponent<Sprite>(en).color.a)
				return;
			Lighten(en);
		}

		void WhenYesHover(Entity en)
		{
			if (roomSet)
				return;
			Darken(en);
			if (Input::KeyDown(KeyCode::MouseLeftButton))
			{
				ChangeScene(0);
				Lighten(en);
				Gameplay::TutorialManager::Instance()->SetTutorialIsPlaying(true);
			}
		}

		void WhenYesExit(Entity en)
		{
			Lighten(en);
		}

		void WhenNoHover(Entity en)
		{
			if (roomSet)
				return;
			Darken(en);
			if (Input::KeyDown(KeyCode::MouseLeftButton))
			{
				ChangeScene(1);
				Lighten(en);
				Gameplay::TutorialManager::Instance()->SetTutorialIsPlaying(false);
			}
		}

		void WhenNoExit(Entity en)
		{
			Lighten(en);
		}
	}

	void NewGameButton::Init(ECS::Entity en)
	{
		CreateEventTrigger(en, true);
		Subscribe(en, Component::EVENT_TRIGGER_TYPE::ON_POINTER_STAY, WhenHover);
		Subscribe(en, Component::EVENT_TRIGGER_TYPE::ON_POINTER_EXIT, WhenExit);

		scene_transition = Coordinator::Instance()->GetEntityByTag("scene_transition");

		if (!Engine::IsChannelPlaying(Engine::Channel::Master))
		{
			Guid id = Engine::AssetManager::Instance()->GetGuid("Assets\\Audio\\Cutscene_MainMenu_BGM.wav");
			Engine::Audio ad = Engine::AssetManager::Instance()->GetAudio(id);
			ad.m_Channel = Engine::Channel::BGM;
			ad.m_Loop = true;
			ad.Play();
		}

		{
			GetSceneGraph().FindImmediateChildren(en);
			std::vector<s32> const& children = GetSceneGraph().GetChildren();

			for (s32 child : children)
			{
				EntityData const& ed = Coordinator::Instance()->GetComponent<EntityData>(static_cast<Entity>(child));
				if (ed.tag == "tutorial_prompt")
					tutorial_prompt = static_cast<Entity>(child);
			}
		}

		{
			GetSceneGraph().FindImmediateChildren(tutorial_prompt);
			std::vector<s32> const& children = GetSceneGraph().GetChildren();

			for (s32 child : children)
			{
				EntityData const& ed = Coordinator::Instance()->GetComponent<EntityData>(static_cast<Entity>(child));
				if (ed.tag == "yes")
					yes = static_cast<Entity>(child);
				else if (ed.tag == "no")
					no  = static_cast<Entity>(child);
			}

			CreateEventTrigger(yes, true);
			Subscribe(yes, Component::EVENT_TRIGGER_TYPE::ON_POINTER_STAY, WhenYesHover);
			Subscribe(yes, Component::EVENT_TRIGGER_TYPE::ON_POINTER_EXIT, WhenYesExit);

			CreateEventTrigger(no, true);
			Subscribe(no, Component::EVENT_TRIGGER_TYPE::ON_POINTER_STAY, WhenNoHover);
			Subscribe(no, Component::EVENT_TRIGGER_TYPE::ON_POINTER_EXIT, WhenNoExit);
		}

		Font::EnableTextRendering(true);

		roomSet = false;
	}

	void NewGameButton::Update(ECS::Entity en)
	{
		if (Coordinator::Instance()->GetComponent<EntityData>(tutorial_prompt).active)
		{
			if (Input::KeyTriggered(KeyCode::Escape))
			{
				SetActive(false, tutorial_prompt);
				Lighten(yes), Lighten(no);
				PauseButtonFlag::confirmationBG = false;
			}
		}
	}

	void NewGameButton::Free(ECS::Entity en)
	{
		tutorial_prompt = yes = no = scene_transition = ECS::MAX_ENTITIES;
		PauseButtonFlag::confirmationBG = false;
	}
}