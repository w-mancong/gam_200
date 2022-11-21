/*!
file:	Audio.h
author:	Wong Man Cong
email:	w.mancong@digipen.edu
brief:	Wrapper function declaration that contains all the data for playing an audio

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
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
			\brief Toggle function to m_Mute audio
		***********************************************************************************/
		void ToggleMute(void);

		fmod::Sound* m_Sound{ nullptr };
		std::string m_AudioName{ "" };
		u32 m_ID{ 0 };
		f32 m_Volume{ 1.0f };
		b8 m_Loop{ false };
		b8 m_Mute{ false };
		Channel m_Channel{ Channel::Invalid };
		fmod::Channel** m_Ch{ nullptr };	// reference to fmod's m_Channel
	};

	/*!*********************************************************************************
		\brief Struct to store a map of different audios 
	***********************************************************************************/
	struct AudioSource
	{
		std::unordered_map<u32, Audio> list;
		u32 id;
	};
}

#endif