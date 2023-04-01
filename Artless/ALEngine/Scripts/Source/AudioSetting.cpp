/*!
file:	AudioSetting.cpp
author:	Wong Man Cong
email:	w.mancong\@digipen.edu
brief:	This file contain function definition for adjusting audio volumes

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#include <pch.h>
#include <AudioSetting.h>
#include <GameAudioManager.h>

namespace ALEngine::Script
{
	namespace
	{
		using namespace ECS;

		ECS::Entity	master_right{ ECS::MAX_ENTITIES }, master_left{ ECS::MAX_ENTITIES },
					sfx_right{ ECS::MAX_ENTITIES }, sfx_left{ ECS::MAX_ENTITIES },
					bgm_right{ ECS::MAX_ENTITIES }, bgm_left{ ECS::MAX_ENTITIES },
					tick_box{ ECS::MAX_ENTITIES }, tick{ ECS::MAX_ENTITIES };

		b8 mouseClicked{ false };
		f32 clickTimer{ 0.0f }, soundTimer{ 0.0f };
		f32 constexpr WAIT_FOR = 0.125f;

		u64 constexpr MASTER = static_cast<u64>(Engine::Channel::Master),
			SFX = static_cast<u64>(Engine::Channel::SFX),
			BGM = static_cast<u64>(Engine::Channel::BGM),
			TOTAL = static_cast<u64>(Engine::Channel::Total);

		f32 volumes[TOTAL]{};
		Entity textID[TOTAL]{ MAX_ENTITIES, MAX_ENTITIES, MAX_ENTITIES };

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

		void SetTextVolume(Engine::Channel channel)
		{
			u64 ch = static_cast<u64>(channel);
			Text& text = Coordinator::Instance()->GetComponent<Text>(textID[ch]);
			volumes[ch] = Engine::GetChannelVolume(channel);
			text.textString = std::to_string( static_cast<u64>( volumes[ch] * 100.0f ) );
		}

		void SetVolume(Engine::Channel channel, f32 flag = 1.0f)
		{
			f32 constexpr HOLD_FOR = 0.5f;

			if (!mouseClicked)
			{
				mouseClicked = true;
				u64 ch = static_cast<u64>(channel);
				soundTimer += Time::m_ActualDeltaTime;
				if (soundTimer >= WAIT_FOR)
				{
					volumes[ch] += 0.01f * flag;
					soundTimer = 0.0f;
					if (1.0f > volumes[ch] && 0.0f < volumes[ch])
						GameAudioManager::Play("VolumeControl");
				}

				volumes[ch] = std::clamp(volumes[ch], 0.0f, 1.0f);
				Engine::SetChannelVolume(channel, volumes[ch]);
				SetTextVolume(channel);
			}

			clickTimer += Time::m_ActualDeltaTime;
			if (clickTimer >= HOLD_FOR)
				mouseClicked = false;
		}

		// ---------------------------------------------------------------------------------

		void WhenMasterRightHover(Entity en)
		{
			Darken(en);
			if (Input::KeyDown(KeyCode::MouseLeftButton))
				SetVolume(Engine::Channel::Master);
		}

		void WhenMasterRightExit(Entity en)
		{
			Lighten(en);
		}

		void WhenMasterLeftHover(Entity en)
		{
			Darken(en);
			if (Input::KeyDown(KeyCode::MouseLeftButton))
				SetVolume(Engine::Channel::Master, -1.0f);
		}

		void WhenMasterLeftExit(Entity en)
		{
			Lighten(en);
		}

		// ---------------------------------------------------------------------------------

		void WhenSFXRightHover(Entity en)
		{
			Darken(en);
			if (Input::KeyDown(KeyCode::MouseLeftButton))
				SetVolume(Engine::Channel::SFX);
		}

		void WhenSFXRightExit(Entity en)
		{
			Lighten(en);
		}

		void WhenSFXLeftHover(Entity en)
		{
			Darken(en);
			if (Input::KeyDown(KeyCode::MouseLeftButton))
				SetVolume(Engine::Channel::SFX, -1.0f);
		}

		void WhenSFXLeftExit(Entity en)
		{
			Lighten(en);
		}

		// ---------------------------------------------------------------------------------

		void WhenBGMRightHover(Entity en)
		{
			Darken(en);
			if (Input::KeyDown(KeyCode::MouseLeftButton))
				SetVolume(Engine::Channel::BGM);
		}

		void WhenBGMRightExit(Entity en)
		{
			Lighten(en);
		}

		void WhenBGMLeftHover(Entity en)
		{
			Darken(en);
			if (Input::KeyDown(KeyCode::MouseLeftButton))
				SetVolume(Engine::Channel::BGM, -1.0f);
		}

		void WhenBGMLeftExit(Entity en)
		{
			Lighten(en);
		}

		// ---------------------------------------------------------------------------------

		void WhenTickBoxHover(Entity en)
		{
			Graphics::OpenGLWindow::ToggleScreen();
			SetActive(Graphics::OpenGLWindow::fullScreen, tick);
		}

		// ---------------------------------------------------------------------------------
	}

	void AudioSetting::Init(ECS::Entity en)
	{
		GetSceneGraph().FindImmediateChildren(en);
		std::vector<s32> const& children1{ GetSceneGraph().GetChildren() };

		Entity master_bd{}, sfx_bd{}, bgm_bd{};
		for (s32 child : children1)
		{
			EntityData const& ed = Coordinator::Instance()->GetComponent<EntityData>(static_cast<Entity>(child));
			if (ed.tag == "master_bd")
				master_bd = static_cast<Entity>(child);
			else if (ed.tag == "sfx_bd")
				sfx_bd = static_cast<Entity>(child);
			else if (ed.tag == "bgm_bd")
				bgm_bd = static_cast<Entity>(child);
			else if (ed.tag == "tick_box")
				tick_box = static_cast<Entity>(child);
		}

		// -------------------------------------------------------------------------------------------------------------

		GetSceneGraph().FindImmediateChildren(master_bd);
		// Find master_right, master_left, master_text
		std::vector<s32> const& children2{ GetSceneGraph().GetChildren() };
		for (s32 child : children2)
		{
			EntityData const& ed = Coordinator::Instance()->GetComponent<EntityData>(static_cast<Entity>(child));
			if (ed.tag == "master_right")
				master_right = static_cast<Entity>(child);
			else if (ed.tag == "master_left")
				master_left = static_cast<Entity>(child);
			else if (ed.tag == "master_text")
				textID[MASTER] = static_cast<Entity>(child);
		}

		CreateEventTrigger(master_right, true);
		Subscribe(master_right, Component::EVENT_TRIGGER_TYPE::ON_POINTER_STAY, WhenMasterRightHover);
		Subscribe(master_right, Component::EVENT_TRIGGER_TYPE::ON_POINTER_EXIT, WhenMasterRightExit);

		CreateEventTrigger(master_left, true);
		Subscribe(master_left, Component::EVENT_TRIGGER_TYPE::ON_POINTER_STAY, WhenMasterLeftHover);
		Subscribe(master_left, Component::EVENT_TRIGGER_TYPE::ON_POINTER_EXIT, WhenMasterLeftExit);

		SetTextVolume(Engine::Channel::Master);

		// -------------------------------------------------------------------------------------------------------------

		GetSceneGraph().FindImmediateChildren(sfx_bd);
		// Find sfx_right, sfx_left, sfx_text
		std::vector<s32> const& children3{ GetSceneGraph().GetChildren() };
		for (s32 child : children3)
		{
			EntityData const& ed = Coordinator::Instance()->GetComponent<EntityData>(static_cast<Entity>(child));
			if (ed.tag == "sfx_right")
				sfx_right = static_cast<Entity>(child);
			else if (ed.tag == "sfx_left")
				sfx_left = static_cast<Entity>(child);
			else if (ed.tag == "sfx_text")
				textID[SFX] = static_cast<Entity>(child);
		}

		CreateEventTrigger(sfx_right, true);
		Subscribe(sfx_right, Component::EVENT_TRIGGER_TYPE::ON_POINTER_STAY, WhenSFXRightHover);
		Subscribe(sfx_right, Component::EVENT_TRIGGER_TYPE::ON_POINTER_EXIT, WhenSFXRightExit);

		CreateEventTrigger(sfx_left, true);
		Subscribe(sfx_left, Component::EVENT_TRIGGER_TYPE::ON_POINTER_STAY, WhenSFXLeftHover);
		Subscribe(sfx_left, Component::EVENT_TRIGGER_TYPE::ON_POINTER_EXIT, WhenSFXLeftExit);

		SetTextVolume(Engine::Channel::SFX);

		// -------------------------------------------------------------------------------------------------------------

		GetSceneGraph().FindImmediateChildren(bgm_bd);
		// Find bgm_right, bgm_left, bgm_text
		std::vector<s32> const& children4{ GetSceneGraph().GetChildren() };
		for (s32 child : children4)
		{
			EntityData const& ed = Coordinator::Instance()->GetComponent<EntityData>(static_cast<Entity>(child));
			if (ed.tag == "bgm_right")
				bgm_right = static_cast<Entity>(child);
			else if (ed.tag == "bgm_left")
				bgm_left = static_cast<Entity>(child);
			else if (ed.tag == "bgm_text")
				textID[BGM] = static_cast<Entity>(child);
		}

		CreateEventTrigger(bgm_right, true);
		Subscribe(bgm_right, Component::EVENT_TRIGGER_TYPE::ON_POINTER_STAY, WhenBGMRightHover);
		Subscribe(bgm_right, Component::EVENT_TRIGGER_TYPE::ON_POINTER_EXIT, WhenBGMRightExit);

		CreateEventTrigger(bgm_left, true);
		Subscribe(bgm_left, Component::EVENT_TRIGGER_TYPE::ON_POINTER_STAY, WhenBGMLeftHover);
		Subscribe(bgm_left, Component::EVENT_TRIGGER_TYPE::ON_POINTER_EXIT, WhenBGMLeftExit);

		SetTextVolume(Engine::Channel::BGM);

		// -------------------------------------------------------------------------------------------------------------

		GetSceneGraph().FindImmediateChildren(static_cast<s32>(tick_box));
		tick = GetSceneGraph().GetChildren()[0];

		CreateEventTrigger(tick_box, true);
		Subscribe(tick_box, Component::EVENT_TRIGGER_TYPE::ON_POINTER_STAY, WhenTickBoxHover);
		SetActive(Graphics::OpenGLWindow::fullScreen, tick);

		// -------------------------------------------------------------------------------------------------------------
		mouseClicked = false;
		clickTimer = 0.0f;
	}

	void AudioSetting::Update([[maybe_unused]] ECS::Entity en)
	{
		if (Input::KeyReleased(KeyCode::MouseLeftButton))
		{
			mouseClicked = false;
			clickTimer = 0.0f;
			soundTimer = WAIT_FOR;
		}
	}

	void AudioSetting::Free([[maybe_unused]] ECS::Entity en)
	{
		master_right  = master_left = textID[MASTER] =
		sfx_right     = sfx_left = textID[SFX] =
		bgm_right     = bgm_left = textID[BGM] = 
		tick_box	  = tick     = ECS::MAX_ENTITIES;
	}
}