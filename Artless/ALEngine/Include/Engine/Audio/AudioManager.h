#ifndef	AUDIO_MANAGER_H
#define AUDIO_MANAGER_H

namespace ALEngine::Engine
{
	namespace fmod = FMOD;

	enum class Channel : s64
	{
		Invalid = -1,
		BGM,
		SFX,
		Master,
		Total,
	};

	// Forward declaration
	struct Audio;

	/***************************************************************************************************
							User interface for users to interact with audio manager
	****************************************************************************************************/

	void AudioManagerInit(void);
	void AudioManagerUpdate(void);
	void AudioManagerExit(void);

	fmod::System* const& GetAudioSystem(void);

	/***************************************************************************************************
									Interface for adjusting a single audio								
	****************************************************************************************************/
	void PlayAudio(Audio& audio);
	void StopAudio(Audio const& audio);
	void PauseAudio(Audio const& audio);
	void UnpauseAudio(Audio const& audio);
	void ToggleAudioPause(Audio const& audio);
	void MuteAudio(Audio const& audio);
	void UnmuteAudio(Audio const& audio);
	void ToggleMuteAudio(Audio const& audio);
	void SetAudioVolume(Audio const& audio);
	void SetAudioLoop(Audio const& audio);

	/***************************************************************************************************
									Interface for adjusting channel group								
	****************************************************************************************************/
	void StopChannel(Channel channel);
	void PauseChannel(Channel channel);
	void UnpauseChannel(Channel channel);
	void TogglePauseChannel(Channel channel);
	void MuteChannel(Channel channel);
	void UnmuteChannel(Channel channel);
	void ToggleMuteChannel(Channel channel);
	void SetChannelVolume(Channel channel, f32 volume);
}

#endif