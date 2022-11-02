﻿/*!
file:	Audio.h
author:	Wong Man Cong
email:	w.mancong@digipen.edu
brief:	Wrapper function declaration that contains all the data for playing an audio

		All content � 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#ifndef	AUDIO_H
#define AUDIO_H

namespace ALEngine::Engine
{
	/*!*********************************************************************************
		\brief Audio class that contains all the necessary variables for playing an audio
	***********************************************************************************/
	class Audio
	{
	public:
		/*!*********************************************************************************
			\brief Play audio
		***********************************************************************************/
		void Play(void);

		/*!*********************************************************************************
			\brief Stop audio
		***********************************************************************************/
		void Stop(void);

		/*!*********************************************************************************
			\brief Pause audio
		***********************************************************************************/
		void Pause(void);

		/*!*********************************************************************************
			\brief Unpause audio
		***********************************************************************************/
		void Unpause(void);

		/*!*********************************************************************************
			\brief Toggle function to pause audio
		***********************************************************************************/
		void TogglePause(void);

		/*!*********************************************************************************
			\brief Mute audio
		***********************************************************************************/
		void Mute(void);

		/*!*********************************************************************************
			\brief Unmute audio
		***********************************************************************************/
		void Unmute(void);

		/*!*********************************************************************************
			\brief Toggle function to mute audio
		***********************************************************************************/
		void ToggleMute(void);

		fmod::Sound* sound{ nullptr };
		f32 volume{ 1.0f };
		b8 loop{ false };
		b8 mute{ false };
		Channel channel{ Channel::Invalid };
		fmod::Channel** ch{ nullptr };	// reference to fmod's channel
	};
}

#endif