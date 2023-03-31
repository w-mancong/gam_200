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

	void GameAudioManager::Init(ECS::Entity)
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
			// SFX
			InitAudio("Assets\\Audio\\MenuOpen.wav");
			InitAudio("Assets\\Audio\\MenuClose.wav");
			InitAudio("Assets\\Audio\\MenuButtonPress.wav");
			InitAudio("Assets\\Audio\\VolumeControl.wav");
			InitAudio("Assets\\Audio\\DialogueBoxOpen.wav");
			InitAudio("Assets\\Audio\\AttackSelectionAppear.wav");
			InitAudio("Assets\\Audio\\CannotPlace.wav");
			InitAudio("Assets\\Audio\\MouseClick.wav");
			InitAudio("Assets\\Audio\\PhaseChange.wav");
			InitAudio("Assets\\Audio\\TileBreak.wav");
			InitAudio("Assets\\Audio\\TileCrack.wav");
			InitAudio("Assets\\Audio\\TilePlace.wav");
			InitAudio("Assets\\Audio\\TileRotation.wav");

			// VO


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
		sounds[name].Play();
	}
}