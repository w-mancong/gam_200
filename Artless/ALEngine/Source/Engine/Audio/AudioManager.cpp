#include <pch.h>

namespace ALEngine::Engine
{
	void AudioManager::Init(void)
	{
		FMOD_RESULT res = System_Create(&system);
		assert(res == FMOD_RESULT::FMOD_OK && "Fmod system not created properly!");
		system->init(32, FMOD_INIT_NORMAL, reinterpret_cast<void*>(FMOD_OUTPUTTYPE::FMOD_OUTPUTTYPE_AUTODETECT));
	}

	void AudioManager::Update(void)
	{
		system->update();
	}

	void AudioManager::Exit(void) 
	{
		system->release();
	}
}