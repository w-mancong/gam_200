#include "pch.h"

/*!
file: FileWatcher.cpp
author: Chan Jie Ming Stanley
email: c.jiemingstanley\@digipen.edu
brief: This file contains function definition for FileWatcher. FileWatcher handles the
       detection of changes in files using files' last write time.

All content :copyright: 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/



ALEngine::Engine::FileWatcher::FileWatcher(std::string pathtoWatch, std::chrono::duration<int, std::milli> delay) :
pathtoWatch{pathtoWatch},
delay{delay}
{
	//creating a record of files from the base directory and their last modification time
	for (auto& file : std::filesystem::recursive_directory_iterator(pathtoWatch))
	{
		filePaths[file.path().string()] = std::filesystem::last_write_time(file);
	}
}

void ALEngine::Engine::FileWatcher::Start()
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
				//need to define a function to alert assetmanager
			    //AlertAssetManager(tempIt->first, FileStatus::Erased);

				//remove from unordered map of filePaths
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

				//need to define a function to alert assetmanager
				//AlertAssetManager(file.path().string(), FileStatus::Created);

			}
			else //detect file modifications or changes
			{
				if (filePaths[file.path().string()] != currentfileLastwritetime)
				{
					filePaths[file.path().string()] = currentfileLastwritetime;

					//need to define a function to alert assetmanager
					//AlertAssetManager(file.path().string(), FileStatus::Modified);
				}
			}
		}

	}
}

void ALEngine::Engine::FileWatcher::SetWatchPath(std::string watchFilepath)
{
	pathtoWatch = watchFilepath;
}

bool ALEngine::Engine::FileWatcher::contains(const std::string& key)
{
	auto temp = filePaths.find(key);
	return temp != filePaths.end();
}
