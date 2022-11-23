/*!
file:	AudioManager.h
author:	Wong Man Cong
email:	w.mancong@digipen.edu
brief:	Function declaration for interacting with AudioManager

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#ifndef	AUDIO_MANAGER_H
#define AUDIO_MANAGER_H

namespace ALEngine::Engine
{
	namespace fmod = FMOD;

	/*!*********************************************************************************
		\brief Enum class used to differentiate the different channels
	***********************************************************************************/
	enum class Channel : s64
	{
		Invalid = -1,
		BGM,
		SFX,
		Master,
		Total,
	};

	// Forward declaration
	class Audio;

	/***************************************************************************************************
							User interface for users to interact with audio manager
	****************************************************************************************************/
	/*!*********************************************************************************
		\brief Initialise AudioManager
	***********************************************************************************/
	void AudioManagerInit(void);

	/*!*********************************************************************************
		\brief Update AudioManager
	***********************************************************************************/
	void AudioManagerUpdate(void);

	/*!*********************************************************************************
		\brief Release memory initialised by AudioManager
	***********************************************************************************/
	void AudioManagerExit(void);

	fmod::System* const& GetAudioSystem(void);

	/***************************************************************************************************
									Interface for adjusting a single audio								
	****************************************************************************************************/
	/*!*********************************************************************************
		\brief Play audio's m_Sound

		\param [in] audio: Audio with the different audio settings
	***********************************************************************************/
	void PlayAudio(Audio& audio);

	/*!*********************************************************************************
		\brief Stop audio's m_Sound

		\param [in] audio: Audio with the different audio settings
	***********************************************************************************/
	void StopAudio(Audio const& audio);

	/*!*********************************************************************************
		\brief Pause audio's m_Sound

		\param [in] audio: Audio with the different audio settings
	***********************************************************************************/
	void PauseAudio(Audio const& audio);

	/*!*********************************************************************************
		\brief Unpause audio's m_Sound

		\param [in] audio: Audio with the different audio settings
	***********************************************************************************/
	void UnpauseAudio(Audio const& audio);

	/*!*********************************************************************************
		\brief Toggle function for pausing

		\param [in] audio: Audio with the different audio settings
	***********************************************************************************/
	void ToggleAudioPause(Audio const& audio);

	/*!*********************************************************************************
		\brief Mute audio

		\param [in] audio: Audio with the different audio settings
	***********************************************************************************/
	void MuteAudio(Audio const& audio);

	/*!*********************************************************************************
		\brief Unmute audio

		\param [in] audio: Audio with the different audio settings
	***********************************************************************************/
	void UnmuteAudio(Audio const& audio);

	/*!*********************************************************************************
		\brief Toggle function for muting 

		\param [in] audio: Audio with the different audio settings
	***********************************************************************************/
	void ToggleMuteAudio(Audio const& audio);

	/*!*********************************************************************************
		\brief Set audio's m_Volume

		\param [in] audio: Audio with the different audio settings
	***********************************************************************************/
	void SetAudioVolume(Audio const& audio);

	/*!*********************************************************************************
		\brief Using audio's m_Channel and m_Loop the audio

		\param [in] audio: Audio with the different audio settings
	***********************************************************************************/
	void SetAudioLoop(Audio const& audio);

	/*!*********************************************************************************
		\brief Check if the audio is playing

		\param [in] audio: To check if this audio is still playing
	***********************************************************************************/
	b8 IsAudioPlaying(Audio const& audio);

	/***************************************************************************************************
									Interface for adjusting m_Channel group								
	****************************************************************************************************/
	/*!*********************************************************************************
		\brief Stop m_Channel group

		\param [in] m_Channel: Channel group
	***********************************************************************************/
	void StopChannel(Channel m_Channel);

	/*!*********************************************************************************
		\brief Pause m_Channel group

		\param [in] m_Channel: Channel group
	***********************************************************************************/
	void PauseChannel(Channel m_Channel);

	/*!*********************************************************************************
		\brief Unpause m_Channel group

		\param [in] m_Channel: Channel group
	***********************************************************************************/
	void UnpauseChannel(Channel m_Channel);

	/*!*********************************************************************************
		\brief Toggle function to pause m_Channel group

		\param [in] m_Channel: Channel group
	***********************************************************************************/
	void TogglePauseChannel(Channel m_Channel);

	/*!*********************************************************************************
		\brief Mute m_Channel

		\param [in] m_Channel: Channel group
	***********************************************************************************/
	void MuteChannel(Channel m_Channel);

	/*!*********************************************************************************
		\brief Unmute m_Channel

		\param [in] m_Channel: Channel group
	***********************************************************************************/
	void UnmuteChannel(Channel m_Channel);

	/*!*********************************************************************************
		\brief Toggle function to m_Mute m_Channel group

		\param [in] m_Channel: Channel group
	***********************************************************************************/
	void ToggleMuteChannel(Channel m_Channel);

	/*!*********************************************************************************
		\brief Set m_Channel's m_Volume

		\param [in] m_Channel: Channel group
	***********************************************************************************/
	void SetChannelVolume(Channel m_Channel, f32 m_Volume);
}

#endif