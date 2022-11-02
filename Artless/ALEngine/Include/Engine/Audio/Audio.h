#ifndef	AUDIO_H
#define AUDIO_H

namespace ALEngine::Engine
{
	class Audio
	{
	public:
		void Play(void);
		void Stop(void);
		void Pause(void);
		void Unpause(void);
		void TogglePause(void);
		void Mute(void);
		void Unmute(void);
		void ToggleMute(void);
		void SetVolume(f32 volume);
		void SetLoop(b8 loop);
		void SetSound(fmod::Sound* sound);

		fmod::Sound* sound{ nullptr };
		f32 volume{ 1.0f };
		b8 loop{ false };
		b8 mute{ false };
		Channel channel{ Channel::Invalid };
		fmod::Channel** ch{ nullptr };	// reference to fmod's channel
	};
}

#endif