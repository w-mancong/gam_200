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

			u64 const start = audioName.find_last_of('_') + 1;
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
			InitAudio("Assets\\Audio\\VO_Guard_v01.wav", Engine::Channel::VO);
			InitAudio("Assets\\Audio\\VO_Guard_v02.wav", Engine::Channel::VO);
			InitAudio("Assets\\Audio\\VO_Summoner_v01.wav", Engine::Channel::VO);
			InitAudio("Assets\\Audio\\VO_Summoner_v02.wav", Engine::Channel::VO);
			InitAudio("Assets\\Audio\\VO_TileDestroyer_v01.wav", Engine::Channel::VO);
			InitAudio("Assets\\Audio\\VO_TileDestroyer_v02.wav", Engine::Channel::VO);
			// Guard SFX
			InitAudio("Assets\\Audio\\SFX_GuardAttack_1.wav");
			InitAudio("Assets\\Audio\\SFX_GuardAttack_2.wav");
			InitAudio("Assets\\Audio\\SFX_GuardDeath.wav");
			InitAudio("Assets\\Audio\\SFX_GuardHurt.wav");
			InitAudio("Assets\\Audio\\SFX_GuardMove.wav");
			// Summoner
			InitAudio("Assets\\Audio\\SFX_SummonerAttack_1.wav");
			InitAudio("Assets\\Audio\\SFX_SummonerAttack_2.wav");
			InitAudio("Assets\\Audio\\SFX_SummonerDeath.wav");
			InitAudio("Assets\\Audio\\SFX_SummonerHurt.wav");
			InitAudio("Assets\\Audio\\SFX_SummonerMove.wav");
			// Tile Destroyer
			InitAudio("Assets\\Audio\\SFX_TileDestroyerAttack_1.wav");
			InitAudio("Assets\\Audio\\SFX_TileDestroyerAttack_2.wav");
			InitAudio("Assets\\Audio\\SFX_TileDestroyerDeath.wav");
			InitAudio("Assets\\Audio\\SFX_TileDestroyerHurt.wav");
			InitAudio("Assets\\Audio\\SFX_TileDestroyerMove.wav");
			// Dror
			InitAudio("Assets\\Audio\\SFX_DrorAttack_1.wav");
			InitAudio("Assets\\Audio\\SFX_DrorAttack_2.wav");
			InitAudio("Assets\\Audio\\SFX_DrorAttack_3.wav");
			InitAudio("Assets\\Audio\\SFX_DrorDeath_1.wav");
			InitAudio("Assets\\Audio\\SFX_DrorDeath_2.wav");
			InitAudio("Assets\\Audio\\SFX_DrorDeath_3.wav");
			InitAudio("Assets\\Audio\\SFX_DrorHurt.wav");
			InitAudio("Assets\\Audio\\SFX_DrorMove.wav");
			InitAudio("Assets\\Audio\\SFX_DrorSelectSkillLoop.wav");

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