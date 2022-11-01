#include <pch.h>

namespace ALEngine::Engine
{
	void Audio::Play(void)
	{
		PlayAudio(*this);
	}

	void Audio::Stop(void)
	{
		StopAudio(*this);
	}

	void Audio::Pause(void)
	{
		PauseAudio(*this);
	}

	void Audio::Unpause(void)
	{
		UnpauseAudio(*this);
	}

	void Audio::TogglePause(void)
	{
		ToggleAudioPause(*this);
	}

	void Audio::Mute(void)
	{
		MuteAudio(*this);
	}

	void Audio::Unmute(void)
	{
		UnmuteAudio(*this);
	}

	void Audio::SetVolume(f32 volume)
	{
		this->volume = volume;
	}

	void Audio::SetLoop(b8 loop)
	{
		this->loop = loop;
	}

	void Audio::SetSound(fmod::Sound* sound)
	{
		this->sound = sound;
	}
}