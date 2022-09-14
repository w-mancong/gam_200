#include "pch.h"

int main(void)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	/*TESTING FOR LOGGER*/
	ALEngine::Exceptions::Logger::Init();

	//ALEngine::Exceptions::Logger::GetCoreLogger()->set_level(spdlog::level::level_enum::warn);
	//AL_CORE_SET_LEVEL(spdlog::level::level_enum::err);
	//AL_CORE_SET_LEVEL(AL_CRITICAL);
	AL_CORE_TRACE("THIS IS A TRACE MESSAGE");
	AL_CORE_DEBUG("THIS IS A DEBUG MESSAGE");
	AL_CORE_INFO("THIS IS A INFO MESSAGE");
	AL_CORE_WARN("THIS IS A WARNING MESSAGE");
	//ALEngine::Exceptions::Logger::GetCoreLogger()->set_level(2);
	//AL_CORE_SET_LEVEL(spdlog::level::level_enum::info);
	AL_CORE_ERROR("THIS IS AN ERROR MESSAGE");
	AL_CORE_CRITICAL("THIS IS A CRITICAL MESSAGE");
	
	/* END OF LOGGER TESTING*/



	ALEngine::Engine::Run();
	ALEngine::Memory::FreeAll();
}