#include "pch.h"

ALEngine::Engine::FileWatcher::FileWatcher(std::string pathtoWatch, std::chrono::duration<int, std::milli> delay) :
pathtoWatch{pathtoWatch},
delay{delay}
{
	for (auto& file : std::filesystem::recursive_directory_iterator(pathtoWatch))
	{
		filePaths[file.path().string()] = std::filesystem::last_write_time(file);
	}
}

void ALEngine::Engine::FileWatcher::start(const std::function<void(std::string, FileStatus)>& action)
{
	while (running)
	{
		//wait for the milliseconds which is the "delay" time
		std::this_thread::sleep_for(delay);

		auto tempIt = filePaths.begin();

		while (tempIt != filePaths.end())
		{
			//check if still file path exists, if not remove from unordered map of filePaths
			if (!std::filesystem::exists(tempIt->first))
			{
				action(tempIt->first, FileStatus::Erased);
				tempIt = filePaths.erase(tempIt);
			}
			else 
			{
				tempIt++;
			}
		}

		//check if a file was created or modified
		for (auto& file : std::filesystem::recursive_directory_iterator(pathtoWatch))
		{
			auto currentfileLastwritetime = std::filesystem::last_write_time(file);

			//if detect new file creation
			if(!contains(file.path().string()))
			{
				filePaths[file.path().string()] = currentfileLastwritetime;
				action(file.path().string(), FileStatus::Created);
			}
			else //detect file modifications or changes
			{
				if (filePaths[file.path().string()] != currentfileLastwritetime)
				{
					filePaths[file.path().string()] = currentfileLastwritetime;
					action(file.path().string(), FileStatus::Modified);
				}
			}
		}

	}
}

bool ALEngine::Engine::FileWatcher::contains(const std::string& key)
{
	auto temp = filePaths.find(key);
	return temp != filePaths.end();
}
