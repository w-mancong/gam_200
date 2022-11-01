#ifndef	AUDIO_H
#define AUDIO_H

namespace ALEngine::Engine
{
	class Audio
	{
	public:


		fmod::Sound* sound{ nullptr };
		f32 volume{ 1.0f };
		b8 loop{ false };
		Channel channel{ Channel::Invalid };
		fmod::Channel** ch{ nullptr };	// reference to fmod's channel
	};
}

#endif