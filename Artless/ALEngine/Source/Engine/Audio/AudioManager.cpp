#include <pch.h>

namespace ALEngine::Engine
{
	class AudioManager
	{
	public:
		void Init(void);
		void Update(void);
		void Exit(void);

		void PlayAudio(Audio& audio);

		fmod::System*& GetSystem(void);

	private:
		void PlaySfx(Audio& audio);
		void PlayBgm(Audio& audio);

		fmod::System* system{ nullptr };
		fmod::ChannelGroup* channelGroup[static_cast<s64>(Channel::Total)]{};

		// My own channel info which will be used to check if channel is for bgm/sfx
		struct ChannelInfo
		{
			fmod::Channel* ch{ nullptr };
			Channel audioChannel{ Channel::Invalid };
		};

		using ChannelQueue = std::queue<ChannelInfo>;
		using UsedChannels = std::vector<ChannelInfo>;

		ChannelQueue sfxChannels{}, bgmChannels{};	// To store all the avaliable channels for sfx, bgm
		UsedChannels usedChannels{};				// To store all the channels that are currently in used

		AudioManager(void) = default;
		~AudioManager(void) = default;
	};

	namespace
	{
		Channel& operator++(Channel& ch)
		{
			/*
				Explicitly convert channel into an integral type,
				then incrementing it by 1
			*/
			s64 res = static_cast<s64>(ch); ++res;
			// To wrap the value of ch
			if (res >= static_cast<s64>(Channel::Total))
				res = static_cast<s64>(Channel::Invalid) + 1;
			// converting ch to the next value then returning it
			return (ch = static_cast<Channel>(res));
		}

		Channel operator++(Channel& ch, int)
		{
			/*
				Explicitly convert channel into an integral type,
				then incrementing it by 1
			*/
			s64 res = static_cast<s64>(ch); ++res;
			// To wrap the value of ch
			if (res >= static_cast<s64>(Channel::Total))
				res = static_cast<s64>(Channel::Invalid) + 1;
			Channel temp = ch;
			// converting ch to the next value
			ch = static_cast<Channel>(res);
			return temp;
		}

		Channel operator--(Channel ch)
		{
			/*
				Explicitly convert channel into an integral type,
				then decrementing it by 1
			*/
			s64 res = static_cast<s64>(ch); --res;
			// To wrap the value of ch
			if (res <= static_cast<s64>(Channel::Invalid))
				res = static_cast<s64>(Channel::Total) - 1;
			// converting ch to the previous value then returning it
			return (ch = static_cast<Channel>(res));
		}

		Channel operator--(Channel ch, int)
		{
			/*
				Explicitly convert channel into an integral type,
				then decrementing it by 1
			*/
			s64 res = static_cast<s64>(ch); ++res;
			// To wrap the value of ch
			if (res >= static_cast<s64>(Channel::Invalid))
				res = static_cast<s64>(Channel::Total) - 1;
			Channel temp = ch;
			// converting ch to the next value
			ch = static_cast<Channel>(res);
			return temp;
		}

		s32 constexpr MAX_CHANNELS{ 128 };
		AudioManager* audioManager{ nullptr };
	}

	void AudioManager::Init(void)
	{
		FMOD_RESULT res = System_Create(&system);
		assert(res == FMOD_RESULT::FMOD_OK && "Fmod system not created properly!");
		system->init(MAX_CHANNELS, FMOD_INIT_NORMAL, reinterpret_cast<void*>(FMOD_OUTPUTTYPE::FMOD_OUTPUTTYPE_AUTODETECT));

		// Create channel groups
		s64 const TOTAL_CHANNELS{ static_cast<s64>(Channel::Total) };
		c8 const* channelNames[TOTAL_CHANNELS] { "BGM", "SFX", "Master" };
		for (s64 i{}; i < TOTAL_CHANNELS; ++i)
		{
			res = system->createChannelGroup(channelNames[i], &channelGroup[i]);
			assert(res == FMOD_RESULT::FMOD_OK && "Unable to create channel groups!");
		}

		fmod::ChannelGroup  *sfx{ channelGroup[static_cast<s64>(Channel::Master)] }, 
							*bgm{ channelGroup[static_cast<s64>(Channel::Master)] }, 
							*master{ channelGroup[static_cast<s64>(Channel::Master)] };		

		// adding channel group bgm and sfx into master
		master->addGroup(bgm);
		master->addGroup(sfx);

		// filling my channels queue
		u64 const SFX_CHANNELS = MAX_CHANNELS - 1;
		bgmChannels.push({ nullptr, Channel::BGM });
		/*
			all the channels stored inside these queues are definitely
			channels that are available for use
		*/ 
		for (u64 i{}; i < SFX_CHANNELS; ++i)
			sfxChannels.push({ nullptr, Channel::SFX });
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
				case Channel::BGM:
				{
					bgmChannels.push(it);
					break;
				}
				case Channel::SFX:
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

	void PlaySfx(Audio& audio)
	{

	}

	void PlayBgm(Audio& audio)
	{

	}

	void AudioManager::PlayAudio(Audio& audio)
	{

	}

	/***************************************************************************************************
							User interface for users to interact with audio manager						
	****************************************************************************************************/
	void AudioManagerInit(void)
	{
		audioManager = Memory::StaticMemory::New<AudioManager>();
		audioManager->Init();
	}

	void AudioManagerUpdate(void)
	{
		audioManager->Update();
	}

	void AudioManagerExit(void)
	{
		audioManager->Exit();
		Memory::StaticMemory::Delete(audioManager);
	}

	void PlayAudio(Audio& audio)
	{
		audioManager->PlayAudio(audio);
	}
}