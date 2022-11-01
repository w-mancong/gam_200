#ifndef	AUDIO_MANAGER_H
#define AUDIO_MANAGER_H

namespace ALEngine::Engine
{
	namespace fmod = FMOD;

	enum class AudioChannel : s64
	{
		Invalid = -1,
		BGM,
		SFX,
		Master,
		Total,
	};

	class AudioManager : Templates::Singleton<AudioManager>
	{
	public:
		void Init(void);
		void Update(void);
		void Exit(void);

		fmod::System*& GetSystem(void);

	private:
		fmod::System* system{ nullptr };
		fmod::ChannelGroup* channelGroup[static_cast<s64>(AudioChannel::Total)]{};

		// My own channel info which will be used to check if channel is for bgm/sfx
		struct ChannelInfo
		{
			fmod::Channel* ch{ nullptr };
			AudioChannel audioChannel{ AudioChannel::Invalid };
		};

		using ChannelQueue = std::queue<ChannelInfo>;
		using UsedChannels  = std::vector<ChannelInfo>;

		ChannelQueue sfxChannels{}, bgmChannels{};	// To store all the avaliable channels for sfx, bgm
		UsedChannels usedChannels{};				// To store all the channels that are currently in used

		AudioManager(void) = default;
		virtual ~AudioManager(void) = default;

		friend class Templates::Singleton<AudioManager>;
		friend class Memory::StaticMemory;
	};
}

#endif