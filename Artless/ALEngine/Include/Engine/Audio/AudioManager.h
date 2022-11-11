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
		\brief Play audio's sound

		\param [in] audio: Audio with the different audio settings
	***********************************************************************************/
	void PlayAudio(Audio& audio);

	/*!*********************************************************************************
		\brief Stop audio's sound

		\param [in] audio: Audio with the different audio settings
	***********************************************************************************/
	void StopAudio(Audio const& audio);

	/*!*********************************************************************************
		\brief Pause audio's sound

		\param [in] audio: Audio with the different audio settings
	***********************************************************************************/
	void PauseAudio(Audio const& audio);

	/*!*********************************************************************************
		\brief Unpause audio's sound

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
		\brief Set audio's volume

		\param [in] audio: Audio with the different audio settings
	***********************************************************************************/
	void SetAudioVolume(Audio const& audio);

	/*!*********************************************************************************
		\brief Using audio's channel and loop the audio

		\param [in] audio: Audio with the different audio settings
	***********************************************************************************/
	void SetAudioLoop(Audio const& audio);

	/***************************************************************************************************
									Interface for adjusting channel group								
	****************************************************************************************************/
	/*!*********************************************************************************
		\brief Stop channel group

		\param [in] channel: Channel group
	***********************************************************************************/
	void StopChannel(Channel channel);

	/*!*********************************************************************************
		\brief Pause channel group

		\param [in] channel: Channel group
	***********************************************************************************/
	void PauseChannel(Channel channel);

	/*!*********************************************************************************
		\brief Unpause channel group

		\param [in] channel: Channel group
	***********************************************************************************/
	void UnpauseChannel(Channel channel);

	/*!*********************************************************************************
		\brief Toggle function to pause channel group

		\param [in] channel: Channel group
	***********************************************************************************/
	void TogglePauseChannel(Channel channel);

	/*!*********************************************************************************
		\brief Mute channel

		\param [in] channel: Channel group
	***********************************************************************************/
	void MuteChannel(Channel channel);

	/*!*********************************************************************************
		\brief Unmute channel

		\param [in] channel: Channel group
	***********************************************************************************/
	void UnmuteChannel(Channel channel);

	/*!*********************************************************************************
		\brief Toggle function to mute channel group

		\param [in] channel: Channel group
	***********************************************************************************/
	void ToggleMuteChannel(Channel channel);

	/*!*********************************************************************************
		\brief Set channel's volume

		\param [in] channel: Channel group
	***********************************************************************************/
	void SetChannelVolume(Channel channel, f32 volume);
}

#endif