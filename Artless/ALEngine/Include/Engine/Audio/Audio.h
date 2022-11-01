#ifndef	AUDIO_H
#define AUDIO_H

namespace ALEngine::Engine
{
	struct Audio
	{
		fmod::Sound *sound;
		AudioChannel channel;
	};
}

#endif