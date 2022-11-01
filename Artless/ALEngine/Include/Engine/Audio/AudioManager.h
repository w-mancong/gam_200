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

	void AudioManagerInit(void);
	void AudioManagerUpdate(void);
	void AudioManagerExit(void);

	fmod::System* const& GetAudioSystem(void);

	void PlayAudio(Audio& audio);
}

#endif