/*!
file:	Audio.cpp
author:	Wong Man Cong
email:	w.mancong@digipen.edu
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
		mute = true;
	}

	void Audio::Unmute(void)
	{
		UnmuteAudio(*this);
		mute = false;
	}

	void Audio::ToggleMute(void)
	{
		if (mute)
			Unmute();
		else
			Mute();
	}
}