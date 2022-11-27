/*!
file:	Audio.cpp
author:	Wong Man Cong
email:	w.mancong\@digipen.edu
brief:	Wrapper function definition that contains all the data for playing an audio

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
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
		m_Mute = true;
	}

	void Audio::Unmute(void)
	{
		UnmuteAudio(*this);
		m_Mute = false;
	}

	void Audio::ToggleMute(void)
	{
		if (m_Mute)
			Unmute();
		else
			Mute();
	}

	void Audio::SetVolume(void)
	{
		SetAudioVolume(*this);
	}

	void Audio::SetLoop(b8 loop)
	{
		m_Loop = loop;
		SetAudioLoop(*this);
	}

	void Audio::ToggleLoop(void)
	{
		m_Loop = !m_Loop;
		SetAudioLoop(*this);
	}

	b8 Audio::IsPlaying(void)
	{
		return IsAudioPlaying(*this);
	}
}