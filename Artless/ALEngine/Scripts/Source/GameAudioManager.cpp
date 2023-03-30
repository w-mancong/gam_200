/*!
file:	GameAudioManager.cpp
author:	Wong Man Cong
email:	w.mancong\@digipen.edu
brief:	This file contain function declaration for loading all audios and using this class
		to play audios

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#include <pch.h>
#include <GameAudioManager.h>

namespace ALEngine::Script
{
	namespace
	{
		using namespace ECS;
		b8 initialized{ false };
	}

	using GAM = GameAudioManager;

	std::unordered_map<std::string, Engine::Audio> GAM::sounds{};

	void GameAudioManager::Init(ECS::Entity en)
	{
		auto InitAudio = [](std::string const& audioName, Engine::Channel channel = Engine::Channel::SFX)
		{
			Guid id = Engine::AssetManager::Instance()->GetGuid(audioName);
			Engine::Audio audio = Engine::AssetManager::Instance()->GetAudio(id);
			audio.m_Channel = channel;

			u64 const start = audioName.find_last_of('\\') + 1;
			sounds[audioName.substr(start, audioName.find_last_of('.') - start)] = audio;
		};

		// Audio
		if (!initialized)
		{
			InitAudio("Assets\\Audio\\MenuOpen.wav");
			InitAudio("Assets\\Audio\\MenuClose.wav");
			InitAudio("Assets\\Audio\\MenuButtonPress.wav");
			InitAudio("Assets\\Audio\\VolumeControl.wav");

			initialized = true;
		}
	}

	void GameAudioManager::Unload(ECS::Entity en)
	{
		sounds.clear();
		initialized = false;
	}

	void GameAudioManager::Play(std::string const& name)
	{
		if (sounds[name].IsPlaying())
			return;
		sounds[name].Play();
	}
}