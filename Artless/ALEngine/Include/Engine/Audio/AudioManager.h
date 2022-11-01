#ifndef	AUDIO_MANAGER_H
#define AUDIO_MANAGER_H

namespace ALEngine::Engine
{
	namespace fmod = FMOD;

	class AudioManager : Templates::Singleton<AudioManager>
	{
	public:
		void Init(void);
		void Update(void);
		void Exit(void);

	private:
		fmod::System* system;

		AudioManager(void) = default;
		virtual ~AudioManager(void) = default;

		friend class Templates::Singleton<AudioManager>;
		friend class Memory::StaticMemory;
	};
}

#endif