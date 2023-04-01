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

			if (channel == Engine::Channel::BGM)
				audio.m_Loop = true;

			u64 const start = audioName.find_first_of('_') + 1;
			sounds[audioName.substr(start, audioName.find_last_of('.') - start)] = audio;
		};

		// Audio
		if (!initialized)
		{
			// SFX
			InitAudio("Assets\\Audio\\SFX_TileBreak.wav");
			InitAudio("Assets\\Audio\\SFX_TileCrack.wav");
			InitAudio("Assets\\Audio\\SFX_TilePlace.wav");
			InitAudio("Assets\\Audio\\SFX_TileRotation.wav");
			// UI SFX
			InitAudio("Assets\\Audio\\SFX_AttackSelectionAppear.wav");
			InitAudio("Assets\\Audio\\SFX_CannotPlace.wav");
			InitAudio("Assets\\Audio\\SFX_DeselectSkill.wav");
			InitAudio("Assets\\Audio\\SFX_DialogueBoxOpen.wav");
			InitAudio("Assets\\Audio\\SFX_MenuButtonPress.wav");
			InitAudio("Assets\\Audio\\SFX_MenuClose.wav");
			InitAudio("Assets\\Audio\\SFX_MenuOpen.wav");
			InitAudio("Assets\\Audio\\SFX_MouseClick.wav");
			InitAudio("Assets\\Audio\\SFX_NewGameStart_ButtonPress(AfterTutorialPrompt).wav");
			InitAudio("Assets\\Audio\\SFX_PhaseChange.wav");
			InitAudio("Assets\\Audio\\SFX_SelectSkill.wav");
			InitAudio("Assets\\Audio\\SFX_VolumeControl.wav");
			InitAudio("Assets\\Audio\\SFX_Click_1.wav");
			// Skills SFX
			InitAudio("Assets\\Audio\\SFX_MatrixTrapPlace.wav");
			InitAudio("Assets\\Audio\\SFX_Overhang.wav");
			InitAudio("Assets\\Audio\\SFX_VolaTilePlace.wav");
			InitAudio("Assets\\Audio\\SFX_VolatTileActivate.wav");
			InitAudio("Assets\\Audio\\SFX_ConstructTileBreak.wav");
			InitAudio("Assets\\Audio\\SFX_ConstructTilePlace.wav");
			InitAudio("Assets\\Audio\\SFX_LifeDrain.wav");
			InitAudio("Assets\\Audio\\SFX_MatrixTrapActivate.wav");
			// VO
			InitAudio("Assets\\Audio\\VO_Guardv01.wav");
			InitAudio("Assets\\Audio\\VO_Guardv02.wav");
			InitAudio("Assets\\Audio\\VO_Summonerv01.wav");
			InitAudio("Assets\\Audio\\VO_Summonerv02.wav");
			InitAudio("Assets\\Audio\\VO_TileDestroyerv01.wav");
			InitAudio("Assets\\Audio\\VO_TileDestroyerv02.wav");
			// Guard SFX
			InitAudio("Assets\\Audio\\SFX_GuardAttack1.wav");
			InitAudio("Assets\\Audio\\SFX_GuardAttack2.wav");
			InitAudio("Assets\\Audio\\SFX_GuardDeath.wav");
			InitAudio("Assets\\Audio\\SFX_GuardHurt.wav");
			InitAudio("Assets\\Audio\\SFX_GuardMove.wav");
			// Summoner
			InitAudio("Assets\\Audio\\SFX_SummonerAttack1.wav");
			InitAudio("Assets\\Audio\\SFX_SummonerAttack2.wav");
			InitAudio("Assets\\Audio\\SFX_SummonerDeath.wav");
			InitAudio("Assets\\Audio\\SFX_SummonerHurt.wav");
			InitAudio("Assets\\Audio\\SFX_SummonerMove.wav");
			// Tile Destroyer
			InitAudio("Assets\\Audio\\SFX_TileDestroyerAttack1.wav");
			InitAudio("Assets\\Audio\\SFX_TileDestroyerAttack2.wav");
			InitAudio("Assets\\Audio\\SFX_TileDestroyerDeath.wav");
			InitAudio("Assets\\Audio\\SFX_TileDestroyerHurt.wav");
			InitAudio("Assets\\Audio\\SFX_TileDestroyerMove.wav");
			// Dror
			InitAudio("Assets\\Audio\\SFX_DrorAttack1.wav");
			InitAudio("Assets\\Audio\\SFX_DrorAttack2.wav");
			InitAudio("Assets\\Audio\\SFX_DrorAttack3.wav");
			InitAudio("Assets\\Audio\\SFX_DrorDeath1.wav");
			InitAudio("Assets\\Audio\\SFX_DrorDeath2.wav");
			InitAudio("Assets\\Audio\\SFX_DrorDeath3.wav");
			InitAudio("Assets\\Audio\\SFX_DrorHurt.wav");
			InitAudio("Assets\\Audio\\SFX_DrorMove.wav");
			InitAudio("Assets\\Audio\\SFX_DrorSelectSkillLoop.wav");

			// BGM
			InitAudio("Assets\\Audio\\BGM_Gameplay_Loop.wav", Engine::Channel::BGM);

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

	void GameAudioManager::Stop(std::string const& name)
	{
		sounds[name].Stop();
	}

	Engine::Audio const& GameAudioManager::Get(std::string const& name)
	{
		return sounds[name];
	}
}