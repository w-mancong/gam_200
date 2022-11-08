/*!
file:	AudioManager.cpp
author:	Wong Man Cong
email:	w.mancong@digipen.edu
brief:	Function declaration for interacting with AudioManager

		All content � 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
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

		// interacting with the channels
		void StopChannel(Channel channel);
		void PauseChannel(Channel channel);
		void UnpauseChannel(Channel channel);
		void TogglePauseChannel(Channel channel);
		void MuteChannel(Channel channel);
		void UnmuteChannel(Channel channel);
		void ToggleMuteChannel(Channel channel);
		void SetChannelVolume(Channel channel, f32 volume);

		fmod::System* const& GetSystem(void) const;

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
		using ChannelIterator = std::vector<std::vector<ChannelInfo>::iterator>;

		ChannelQueue sfxChannels{}, bgmChannels{};	// To store all the avaliable channels for sfx, bgm
		UsedChannels usedChannels{};				// To store all the channels that are currently in used		
		ChannelIterator iterators{};				// To store all the iterator of used channels

		AudioManager(void) = default;
		~AudioManager(void) = default;

		friend Memory::StaticMemory;
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

		s32 constexpr MAX_CHANNELS{ 256 }, BGM_CHANNELS{ 32 };
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

		fmod::ChannelGroup	*bgm{ channelGroup[static_cast<s64>(Channel::BGM)] },
							*sfx{ channelGroup[static_cast<s64>(Channel::SFX)] }, 
							*master{ channelGroup[static_cast<s64>(Channel::Master)] };		

		// adding master channel as an input group to bgm and sfx
		master->addGroup(bgm);
		master->addGroup(sfx);

		u64 const SFX_CHANNELS = MAX_CHANNELS - BGM_CHANNELS;
		// filling my channels queue
		/*
			all the channels stored inside these queues are definitely
			channels that are available for use
		*/
		for (u64 i{}; i < BGM_CHANNELS; ++i)
			bgmChannels.push({ nullptr, Channel::BGM });
		for (u64 i{}; i < SFX_CHANNELS; ++i)
			sfxChannels.push({ nullptr, Channel::SFX });
		// Reserving a size of iterators to be max_channels
		iterators.reserve(MAX_CHANNELS);
	}

	void AudioManager::Update(void)
	{
		// Required by fmod
		system->update();

		// To remove any used channels and add them into the appropriate queue
		for (auto it{ usedChannels.begin() }; it != usedChannels.end(); ++it)
		{
			fmod::Channel* const& ch = (*it).ch;
			b8 isPlaying{ true }; // Assume that all audio is playing
			ch->isPlaying(&isPlaying);
			if (isPlaying)
				continue;
			// If audio is no longer playing, add it to the appropriate queue
			switch ((*it).audioChannel)
			{
			case Channel::BGM:
			{
				bgmChannels.push(*it);
				break;
			}
			case Channel::SFX:
			{
				sfxChannels.push(*it);
				break;
			}
			default:
				break;
			}
			iterators.push_back(it);
		}

		// To remove all the iterators from usedChannels, have to do this here has it might crash if I were to iterator and removing the same vector
		while (iterators.size())
		{
			usedChannels.erase(iterators.back());
			iterators.pop_back();
		}
	}

	void AudioManager::Exit(void) 
	{
		system->release();
	}

	fmod::System* const& AudioManager::GetSystem(void) const
	{
		return system;
	}

	void AudioManager::PlaySfx(Audio& audio)
	{
		// if channel size == 0, means no avaliable channel to play audio
		if (!sfxChannels.size())
			return;
		// Get the first avaliable channel, then remove it from the queue
		ChannelInfo& channelInfo = sfxChannels.front(); sfxChannels.pop();
		fmod::Channel*& ch = channelInfo.ch;
		system->playSound(audio.sound, channelGroup[static_cast<s64>(Channel::SFX)], false, &ch);
		// Set audio's channel
		audio.ch = &ch;
		ch->setVolume(audio.volume);
		u32 const LOOP = audio.loop ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF;
		ch->setMode(LOOP);
		ch->setChannelGroup(channelGroup[static_cast<s64>(Channel::SFX)]);
		usedChannels.push_back(channelInfo);
	}

	void AudioManager::PlayBgm(Audio& audio)
	{
		// if channel size == 0, means no avaliable channel to play audio
		if (!bgmChannels.size())
			return;
		// Get the first avaliable channel, then remove it from the queue
		ChannelInfo& channelInfo = bgmChannels.front(); bgmChannels.pop();
		fmod::Channel*& ch = channelInfo.ch;
		system->playSound(audio.sound, channelGroup[static_cast<s64>(Channel::BGM)], false, &ch);
		// Set audio's channel
		audio.ch = &ch;
		ch->setVolume(audio.volume);
		u32 const LOOP = audio.loop ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF;
		ch->setMode(LOOP);
		ch->setChannelGroup(channelGroup[static_cast<s64>(Channel::BGM)]);
		usedChannels.push_back(channelInfo);
	}

	void AudioManager::PlayAudio(Audio& audio)
	{
		switch (audio.channel)
		{
			case Channel::BGM:
			{
				PlayBgm(audio);
				break;
			}
			case Channel::SFX:
			{
				PlaySfx(audio);
				break;
			}
			default:
				break;
		}
	}

	void AudioManager::StopChannel(Channel channel)
	{
		s64 const ch{ static_cast<s64>(channel) };
		channelGroup[ch]->stop();
	}

	void AudioManager::PauseChannel(Channel channel)
	{
		s64 const ch{ static_cast<s64>(channel) };
		channelGroup[ch]->setPaused(true);
	}

	void AudioManager::UnpauseChannel(Channel channel)
	{
		s64 const ch{ static_cast<s64>(channel) };
		channelGroup[ch]->setPaused(false);
	}

	void AudioManager::TogglePauseChannel(Channel channel)
	{
		s64 const ch{ static_cast<s64>(channel) };
		b8 isPaused{};
		channelGroup[ch]->getPaused(&isPaused);
		channelGroup[ch]->setPaused(!isPaused);
	}

	void AudioManager::MuteChannel(Channel channel)
	{
		s64 const ch{ static_cast<s64>(channel) };
		channelGroup[ch]->setVolume(true);
	}

	void AudioManager::UnmuteChannel(Channel channel)
	{
		s64 const ch{ static_cast<s64>(channel) };
		channelGroup[ch]->setMute(false);
	}

	void AudioManager::ToggleMuteChannel(Channel channel)
	{
		s64 const ch{ static_cast<s64>(channel) };
		b8 isMuted{};
		channelGroup[ch]->getMute(&isMuted);
		channelGroup[ch]->setMute(!isMuted);
	}

	void AudioManager::SetChannelVolume(Channel channel, f32 volume)
	{
		s64 const ch{ static_cast<s64>(channel) };
		channelGroup[ch]->setVolume(volume);
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

	fmod::System* const& GetAudioSystem(void)
	{
		return audioManager->GetSystem();
	}

	void PlayAudio(Audio& audio)
	{
		audioManager->PlayAudio(audio);
	}

	void StopAudio(Audio const& audio)
	{
		if (!audio.ch)
			return;
		(*audio.ch)->stop();
	}

	void PauseAudio(Audio const& audio)
	{
		if (!audio.ch)
			return;
		(*audio.ch)->setPaused(true);
	}

	void UnpauseAudio(Audio const& audio)
	{
		if (!audio.ch)
			return;
		(*audio.ch)->setPaused(false);
	}

	void ToggleAudioPause(Audio const& audio)
	{
		if (!audio.ch)
			return;
		b8 isPlaying{}; (*audio.ch)->getPaused(&isPlaying);
		(*audio.ch)->setPaused(!isPlaying);
	}

	void MuteAudio(Audio const& audio)
	{
		if (!audio.ch)
			return;
		(*audio.ch)->setMute(true);
	}

	void UnmuteAudio(Audio const& audio)
	{
		if (!audio.ch)
			return;
		(*audio.ch)->setMute(true);
	}

	void ToggleMuteAudio(Audio& audio)
	{
		if (!audio.ch)
			return;
		b8 isMuted{}; 
		(*audio.ch)->getMute(&isMuted);
		(*audio.ch)->setMute(!isMuted);
	}

	void SetAudioVolume(Audio const& audio)
	{
		if (!audio.ch)
			return;
		(*audio.ch)->setVolume(audio.volume);
	}

	void SetAudioLoop(Audio const& audio)
	{
		if (!audio.ch)
			return;
		u32 const LOOP = audio.loop ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF;
		(*audio.ch)->setMode(LOOP);
	}

	void StopChannel(Channel channel)
	{
		audioManager->StopChannel(channel);
	}

	void PauseChannel(Channel channel)
	{
		audioManager->PauseChannel(channel);
	}

	void UnpauseChannel(Channel channel)
	{
		audioManager->UnpauseChannel(channel);
	}

	void TogglePauseChannel(Channel channel)
	{
		audioManager->TogglePauseChannel(channel);
	}

	void MuteChannel(Channel channel)
	{
		audioManager->MuteChannel(channel);
	}

	void UnmuteChannel(Channel channel)
	{
		audioManager->UnmuteChannel(channel);
	}

	void ToggleMuteChannel(Channel channel)
	{
		audioManager->ToggleMuteChannel(channel);
	}

	void SetChannelVolume(Channel channel, f32 volume)
	{
		audioManager->SetChannelVolume(channel, volume);
	}
}