#include <pch.h>

namespace ALEngine::Engine
{
	namespace
	{
		s32 constexpr MAX_CHANNELS{ 128 };

		AudioChannel& operator++(AudioChannel& ch)
		{
			/*
				Explicitly convert channel into an integral type,
				then incrementing it by 1
			*/
			s64 res = static_cast<s64>(ch); ++res;
			// To wrap the value of ch
			if (res >= static_cast<s64>(AudioChannel::Total))
				res = static_cast<s64>(AudioChannel::Invalid) + 1;
			// converting ch to the next value then returning it
			return (ch = static_cast<AudioChannel>(res));
		}

		AudioChannel operator++(AudioChannel& ch, int)
		{
			/*
				Explicitly convert channel into an integral type,
				then incrementing it by 1
			*/
			s64 res = static_cast<s64>(ch); ++res;
			// To wrap the value of ch
			if (res >= static_cast<s64>(AudioChannel::Total))
				res = static_cast<s64>(AudioChannel::Invalid) + 1;
			AudioChannel temp = ch;
			// converting ch to the next value
			ch = static_cast<AudioChannel>(res);	
			return temp;
		}

		AudioChannel operator--(AudioChannel ch)
		{
			/*
				Explicitly convert channel into an integral type,
				then decrementing it by 1
			*/
			s64 res = static_cast<s64>(ch); --res;
			// To wrap the value of ch
			if (res <= static_cast<s64>(AudioChannel::Invalid))
				res = static_cast<s64>(AudioChannel::Total) - 1;
			// converting ch to the previous value then returning it
			return (ch = static_cast<AudioChannel>(res));
		}

		AudioChannel operator--(AudioChannel ch, int)
		{
			/*
				Explicitly convert channel into an integral type,
				then decrementing it by 1
			*/
			s64 res = static_cast<s64>(ch); ++res;
			// To wrap the value of ch
			if (res >= static_cast<s64>(AudioChannel::Invalid))
				res = static_cast<s64>(AudioChannel::Total) - 1;
			AudioChannel temp = ch;
			// converting ch to the next value
			ch = static_cast<AudioChannel>(res);
			return temp;
		}
	}

	void AudioManager::Init(void)
	{
		FMOD_RESULT res = System_Create(&system);
		assert(res == FMOD_RESULT::FMOD_OK && "Fmod system not created properly!");
		system->init(MAX_CHANNELS, FMOD_INIT_NORMAL, reinterpret_cast<void*>(FMOD_OUTPUTTYPE::FMOD_OUTPUTTYPE_AUTODETECT));

		// Create channel groups
		s64 const TOTAL_CHANNELS{ static_cast<s64>(AudioChannel::Total) };
		c8 const* channelNames[TOTAL_CHANNELS] { "BGM", "SFX", "Master" };
		for (s64 i{}; i < TOTAL_CHANNELS; ++i)
		{
			res = system->createChannelGroup(channelNames[i], &channelGroup[i]);
			assert(res == FMOD_RESULT::FMOD_OK && "Unable to create channel groups!");
		}

		u64 const HALF_CHANNELS = MAX_CHANNELS >> 1;
		// filling my channels queue
		for (u64 i{}; i < HALF_CHANNELS; ++i)
		{
			/*
				all the channels stored inside these queues are definitely
				channels that are available for use
			*/ 
			sfxChannels.push({ nullptr, AudioChannel::SFX });
			bgmChannels.push({ nullptr, AudioChannel::BGM });
		}
	}

	void AudioManager::Update(void)
	{
		// Required by fmod
		system->update();

		// To remove any used channels and add them into the appropriate queue
		for (auto const& it : usedChannels)
		{
			bool audioPlaying{ true }; // Assume that all audio is playing
			it.ch->isPlaying(&audioPlaying);
			if (audioPlaying)
				continue;
			// If audio is no longer playing, add it to the appropriate queue
			switch (it.audioChannel)
			{
				case AudioChannel::BGM:
				{
					bgmChannels.push(it);
					break;
				}
				case AudioChannel::SFX:
				{
					sfxChannels.push(it);
					break;
				}
				default:
					break;
			}
		}
	}

	void AudioManager::Exit(void) 
	{
		system->release();
	}

	fmod::System*& AudioManager::GetSystem(void)
	{
		return system;
	}
}