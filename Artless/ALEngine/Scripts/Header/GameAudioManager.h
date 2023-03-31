/*!
file:	GameAudioManager.h
author:	Wong Man Cong
email:	w.mancong\@digipen.edu
brief:	This file contain function declaration for loading all audios and using this class
		to play audios

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#ifndef	GAME_AUDIO_MANAGER_H
#define GAME_AUDIO_MANAGER_H

#include <ECS/Components/UniBehaviour.h>

namespace ALEngine::Script
{
	class GameAudioManager : public ECS::Component::UniBehaviour
	{
	public:
		/*!*********************************************************************************
			\brief Used to initialise any values to it's default value
		***********************************************************************************/
		void Init(ECS::Entity en);

	private:
		/*!*********************************************************************************
			\brief Used to initialise any values to it's default value
		***********************************************************************************/
		void Unload(ECS::Entity en);

		// For RTTR
		void DeserializeComponent(ECS::Entity en)
		{
			ECS::AddLogicComponent<GameAudioManager>(en);
		};
		RTTR_ENABLE(ECS::Component::UniBehaviour)

		static std::unordered_map<std::string, Engine::Audio> sounds;
	public:
		static void Play(std::string const& name);
	};
}

#endif