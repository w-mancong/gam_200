/*!
file:	AudioManager.cpp
author:	Wong Man Cong
email:	w.mancong\@digipen.edu
brief:	Function declaration for interacting with AudioManager

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
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
		void StopChannel(Channel m_Channel);
		void PauseChannel(Channel m_Channel);
		void UnpauseChannel(Channel m_Channel);
		void TogglePauseChannel(Channel m_Channel);
		void MuteChannel(Channel m_Channel);
		void UnmuteChannel(Channel m_Channel);
		void ToggleMuteChannel(Channel m_Channel);
		void SetChannelVolume(Channel m_Channel, f32 m_Volume);

		bool IsChannelPlaying(Channel m_Channel);

		f32 GetChannelVolume(Channel m_Channel) const;

		fmod::System* const& GetSystem(void) const;

	private:
		void PlaySfx(Audio& audio);
		void PlayBgm(Audio& audio);

		fmod::System* system{ nullptr };
		fmod::ChannelGroup* channelGroup[static_cast<s64>(Channel::Total)]{};
		f32 volumes[static_cast<u64>(Channel::Total)]{ 100.0f, 100.0f, 100.0f };

		c8 const *AUDIO_SETTING_FILE_PATH = "Assets\\Dev\\Objects\\audio_settings.json";

		static u64 constexpr MASTER = static_cast<u64>(Engine::Channel::Master),
							 SFX    = static_cast<u64>(Engine::Channel::SFX),
							 BGM    = static_cast<u64>(Engine::Channel::BGM);

		// My own m_Channel info which will be used to check if m_Channel is for bgm/sfx
		struct ChannelInfo
		{
			fmod::Channel* m_Ch{ nullptr };
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
		Channel& operator++(Channel& m_Ch)
		{
			/*
				Explicitly convert m_Channel into an integral type,
				then incrementing it by 1
			*/
			s64 res = static_cast<s64>(m_Ch); ++res;
			// To wrap the value of m_Ch
			if (res >= static_cast<s64>(Channel::Total))
				res = static_cast<s64>(Channel::Invalid) + 1;
			// converting m_Ch to the next value then returning it
			return (m_Ch = static_cast<Channel>(res));
		}

		Channel operator++(Channel& m_Ch, int)
		{
			/*
				Explicitly convert m_Channel into an integral type,
				then incrementing it by 1
			*/
			s64 res = static_cast<s64>(m_Ch); ++res;
			// To wrap the value of m_Ch
			if (res >= static_cast<s64>(Channel::Total))
				res = static_cast<s64>(Channel::Invalid) + 1;
			Channel temp = m_Ch;
			// converting m_Ch to the next value
			m_Ch = static_cast<Channel>(res);
			return temp;
		}

		Channel operator--(Channel m_Ch)
		{
			/*
				Explicitly convert m_Channel into an integral type,
				then decrementing it by 1
			*/
			s64 res = static_cast<s64>(m_Ch); --res;
			// To wrap the value of m_Ch
			if (res <= static_cast<s64>(Channel::Invalid))
				res = static_cast<s64>(Channel::Total) - 1;
			// converting m_Ch to the previous value then returning it
			return (m_Ch = static_cast<Channel>(res));
		}

		Channel operator--(Channel m_Ch, int)
		{
			/*
				Explicitly convert m_Channel into an integral type,
				then decrementing it by 1
			*/
			s64 res = static_cast<s64>(m_Ch); ++res;
			// To wrap the value of m_Ch
			if (res >= static_cast<s64>(Channel::Invalid))
				res = static_cast<s64>(Channel::Total) - 1;
			Channel temp = m_Ch;
			// converting m_Ch to the next value
			m_Ch = static_cast<Channel>(res);
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

		// Create m_Channel groups
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

		// adding master m_Channel as an input group to bgm and sfx
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

		// Load audio volume from .json file
		namespace rjs = rapidjson;
		rjs::Document doc;

		{
			c8* buffer = utils::ReadBytes(AUDIO_SETTING_FILE_PATH);

			if (!buffer)
			{
				AL_CORE_CRITICAL("Error: Unable to load scene: {}", AUDIO_SETTING_FILE_PATH);
				return;
			}

			doc.Parse(buffer);
			Memory::DynamicMemory::Delete(buffer);
		}

		rjs::Value::ValueIterator it = doc.Begin();
		volumes[SFX] = (*it)["sfx"].GetFloat();
		volumes[BGM] = (*it)["bgm"].GetFloat();
		volumes[MASTER] = (*it)["master"].GetFloat();

		sfx->setVolume(volumes[SFX]);
		bgm->setVolume(volumes[BGM]);
		master->setVolume(volumes[MASTER]);
	}

	void AudioManager::Update(void)
	{
		// Required by fmod
		system->update();

		// To remove any used channels and add them into the appropriate queue
		for (auto it{ usedChannels.begin() }; it != usedChannels.end(); ++it)
		{
			fmod::Channel* const& m_Ch = it->m_Ch;
			b8 isPlaying{ true }; // Assume that all audio is playing
			m_Ch->isPlaying(&isPlaying);
			if (isPlaying)
				continue;
			// If audio is no longer playing, add it to the appropriate queue
			switch (it->audioChannel)
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
		// save audio volume into .json file
		namespace rjs = rapidjson;
		using TWriter = rjs::PrettyWriter<rjs::StringBuffer>;

		rjs::StringBuffer sb{};
		TWriter writer{ sb };

		writer.StartArray();
		writer.StartObject();

		writer.Key("master");
		writer.Double( static_cast<f64>( volumes[MASTER] ) );

		writer.Key("sfx");
		writer.Double( static_cast<f64>(volumes[SFX]) );

		writer.Key("bgm");
		writer.Double( static_cast<f64>( volumes[BGM] ) );

		writer.EndObject();
		writer.EndArray();

		std::ofstream ofs{ AUDIO_SETTING_FILE_PATH };
		if (!ofs)
		{
			AL_CORE_WARN("Unable to save into audio_settings.json!");
			return;
		}
		ofs.write(sb.GetString(), sb.GetLength());
	}

	fmod::System* const& AudioManager::GetSystem(void) const
	{
		return system;
	}

	void AudioManager::PlaySfx(Audio& audio)
	{
		// if m_Channel size == 0, means no avaliable m_Channel to play audio
		if (!sfxChannels.size())
			return;
		// Get the first avaliable m_Channel, then remove it from the queue
		ChannelInfo& channelInfo = sfxChannels.front(); sfxChannels.pop();
		fmod::Channel*& m_Ch = channelInfo.m_Ch;
		system->playSound(audio.m_Sound, channelGroup[static_cast<s64>(Channel::SFX)], false, &m_Ch);
		// Set audio's m_Channel
		audio.m_Ch = &m_Ch;
		m_Ch->setVolume(audio.m_Volume);
		u32 const LOOP = audio.m_Loop ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF;
		m_Ch->setMode(LOOP);
		m_Ch->setChannelGroup(channelGroup[static_cast<s64>(Channel::SFX)]);
		m_Ch->setMute(audio.m_Mute);
		usedChannels.push_back(channelInfo);
	}

	void AudioManager::PlayBgm(Audio& audio)
	{
		// if m_Channel size == 0, means no avaliable m_Channel to play audio
		if (!bgmChannels.size())
			return;
		// Get the first avaliable m_Channel, then remove it from the queue
		ChannelInfo& channelInfo = bgmChannels.front(); bgmChannels.pop();
		fmod::Channel*& m_Ch = channelInfo.m_Ch;
		system->playSound(audio.m_Sound, channelGroup[static_cast<s64>(Channel::BGM)], false, &m_Ch);
		// Set audio's m_Channel
		audio.m_Ch = &m_Ch;
		m_Ch->setVolume(audio.m_Volume);
		u32 const LOOP = audio.m_Loop ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF;
		m_Ch->setMode(LOOP);
		m_Ch->setChannelGroup(channelGroup[static_cast<s64>(Channel::BGM)]);
		m_Ch->setMute(audio.m_Mute);
		usedChannels.push_back(channelInfo);
	}

	void AudioManager::PlayAudio(Audio& audio)
	{
		switch (audio.m_Channel)
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

	void AudioManager::StopChannel(Channel m_Channel)
	{
		s64 const m_Ch{ static_cast<s64>(m_Channel) };
		channelGroup[m_Ch]->stop();
	}

	void AudioManager::PauseChannel(Channel m_Channel)
	{
		s64 const m_Ch{ static_cast<s64>(m_Channel) };
		channelGroup[m_Ch]->setPaused(true);
	}

	void AudioManager::UnpauseChannel(Channel m_Channel)
	{
		s64 const m_Ch{ static_cast<s64>(m_Channel) };
		channelGroup[m_Ch]->setPaused(false);
	}

	void AudioManager::TogglePauseChannel(Channel m_Channel)
	{
		s64 const m_Ch{ static_cast<s64>(m_Channel) };
		b8 isPaused{};
		channelGroup[m_Ch]->getPaused(&isPaused);
		channelGroup[m_Ch]->setPaused(!isPaused);
	}

	void AudioManager::MuteChannel(Channel m_Channel)
	{
		s64 const m_Ch{ static_cast<s64>(m_Channel) };
		channelGroup[m_Ch]->setVolume(true);
	}

	void AudioManager::UnmuteChannel(Channel m_Channel)
	{
		s64 const m_Ch{ static_cast<s64>(m_Channel) };
		channelGroup[m_Ch]->setMute(false);
	}

	void AudioManager::ToggleMuteChannel(Channel m_Channel)
	{
		s64 const m_Ch{ static_cast<s64>(m_Channel) };
		b8 isMuted{};
		channelGroup[m_Ch]->getMute(&isMuted);
		channelGroup[m_Ch]->setMute(!isMuted);
	}

	void AudioManager::SetChannelVolume(Channel m_Channel, f32 m_Volume)
	{
		s64 const m_Ch{ static_cast<s64>(m_Channel) };
		channelGroup[m_Ch]->setVolume(m_Volume);
		volumes[m_Ch] = m_Volume;
	}

	bool AudioManager::IsChannelPlaying(Channel m_Channel)
	{
		b8 isPlaying{ false };
		s64 const m_Ch{ static_cast<s64>(m_Channel) };
		channelGroup[m_Ch]->isPlaying(&isPlaying);
		return isPlaying;
	}

	f32 AudioManager::GetChannelVolume(Channel m_Channel) const
	{
		s64 const m_Ch{ static_cast<s64>(m_Channel) };
		return volumes[m_Ch];
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
		if (!audio.m_Ch)
			return;
		(*audio.m_Ch)->stop();
	}

	void PauseAudio(Audio const& audio)
	{
		if (!audio.m_Ch)
			return;
		(*audio.m_Ch)->setPaused(true);
	}

	void UnpauseAudio(Audio const& audio)
	{
		if (!audio.m_Ch)
			return;
		(*audio.m_Ch)->setPaused(false);
	}

	void ToggleAudioPause(Audio const& audio)
	{
		if (!audio.m_Ch)
			return;
		b8 isPlaying{}; (*audio.m_Ch)->getPaused(&isPlaying);
		(*audio.m_Ch)->setPaused(!isPlaying);
	}

	void MuteAudio(Audio const& audio)
	{
		if (!audio.m_Ch)
			return;
		(*audio.m_Ch)->setMute(true);
	}

	void UnmuteAudio(Audio const& audio)
	{
		if (!audio.m_Ch)
			return;
		(*audio.m_Ch)->setMute(false);
	}

	void ToggleMuteAudio(Audio& audio)
	{
		if (!audio.m_Ch)
			return;
		b8 isMuted{}; 
		(*audio.m_Ch)->getMute(&isMuted);
		(*audio.m_Ch)->setMute(!isMuted);
	}

	void SetAudioVolume(Audio const& audio)
	{
		if (!audio.m_Ch)
			return;
		(*audio.m_Ch)->setVolume(audio.m_Volume);
	}

	void SetAudioLoop(Audio const& audio)
	{
		if (!audio.m_Ch)
			return;
		u32 const LOOP = audio.m_Loop ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF;
		(*audio.m_Ch)->setMode(LOOP);
	}

	b8 IsAudioPlaying(Audio const& audio)
	{
		if (!audio.m_Ch)
			return false;
		b8 isPlaying{ false };
		(*audio.m_Ch)->isPlaying(&isPlaying);
		return isPlaying;
	}

	void StopChannel(Channel m_Channel)
	{
		audioManager->StopChannel(m_Channel);
	}

	void PauseChannel(Channel m_Channel)
	{
		audioManager->PauseChannel(m_Channel);
	}

	void UnpauseChannel(Channel m_Channel)
	{
		audioManager->UnpauseChannel(m_Channel);
	}

	void TogglePauseChannel(Channel m_Channel)
	{
		audioManager->TogglePauseChannel(m_Channel);
	}

	void MuteChannel(Channel m_Channel)
	{
		audioManager->MuteChannel(m_Channel);
	}

	void UnmuteChannel(Channel m_Channel)
	{
		audioManager->UnmuteChannel(m_Channel);
	}

	void ToggleMuteChannel(Channel m_Channel)
	{
		audioManager->ToggleMuteChannel(m_Channel);
	}

	void SetChannelVolume(Channel m_Channel, f32 m_Volume)
	{
		audioManager->SetChannelVolume(m_Channel, m_Volume);
	}

	bool IsChannelPlaying(Channel m_Channel)
	{
		return audioManager->IsChannelPlaying(m_Channel);
	}

	f32 GetChannelVolume(Channel m_Channel)
	{
		return audioManager->GetChannelVolume(m_Channel);
	}
}