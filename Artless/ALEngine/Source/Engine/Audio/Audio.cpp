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
		mute = true;
	}

	void Audio::Unmute(void)
	{
		UnmuteAudio(*this);
		mute = false;
	}

	void Audio::ToggleMute(void)
	{
		
	}

	void Audio::SetVolume(f32 _volume)
	{
		volume = _volume;
	}

	void Audio::SetLoop(b8 _loop)
	{
		loop = _loop;
	}

	void Audio::SetSound(fmod::Sound* _sound)
	{
		sound = _sound;
	}
}