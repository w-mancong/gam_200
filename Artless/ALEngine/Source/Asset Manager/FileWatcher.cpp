#include "pch.h"

ALEngine::Engine::FileWatcher::FileWatcher(std::string pathtoWatch, std::chrono::duration<int, std::milli> delay) :
pathtoWatch{pathtoWatch},
delay{delay}
{
	for (auto& file : std::filesystem::recursive_directory_iterator(pathtoWatch))
	{
		
	}

}
