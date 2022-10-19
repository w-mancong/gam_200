#include "pch.h"

/*!
file: FileWatcher.cpp
author: Chan Jie Ming Stanley
email: c.jiemingstanley\@digipen.edu
brief: This file contains function definition for FileWatcher. FileWatcher handles the
       detection of changes in files using files' last write time.

All content :copyright: 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
namespace ALEngine::Engine
{
    FileWatcher::FileWatcher(std::string pathToWatch, std::chrono::duration<int, std::milli> delay) :
	m_PathToWatch{ pathToWatch },
	m_Delay{ delay }
	{
		//creating a record of files from the base directory and their last modification time
		for (auto& file : std::filesystem::recursive_directory_iterator(m_PathToWatch))
		{
			m_FilePaths[file.path().string()] = std::filesystem::last_write_time(file);
		}
	}

	void FileWatcher::Start()
	{
		while (m_Running)
		{
			//wait for the milliseconds which is the "delay" time
			std::this_thread::sleep_for(m_Delay);

			auto tempIt = m_FilePaths.begin();

			while (tempIt != m_FilePaths.end())
			{
				//check if still file path exists, if not remove from unordered map of filePaths
				if (!std::filesystem::exists(tempIt->first))
				{
					//need to define a function to alert assetmanager
					//AlertAssetManager(tempIt->first, FileStatus::Erased);

					//remove from unordered map of filePaths
					tempIt = m_FilePaths.erase(tempIt);
				}
				else
				{
					tempIt++;
				}
			}

			//check if a file was created or modified
			for (auto const& file : std::filesystem::recursive_directory_iterator(m_PathToWatch))
			{
				auto currentfileLastwritetime = std::filesystem::last_write_time(file);

				//if detect new file creation
				if (!contains(file.path().string()))
				{
					m_FilePaths[file.path().string()] = currentfileLastwritetime;

					//need to define a function to alert assetmanager
					//AlertAssetManager(file.path().string(), FileStatus::Created);

				}
				else //detect file modifications or changes
				{
					if (m_FilePaths[file.path().string()] != currentfileLastwritetime)
					{
						m_FilePaths[file.path().string()] = currentfileLastwritetime;

						//need to define a function to alert assetmanager
						//AlertAssetManager(file.path().string(), FileStatus::Modified);
					}
				}
			}

		}
	}

	void FileWatcher::SetWatchPath(std::string watchFilepath)
	{
		m_PathToWatch = watchFilepath;
	}

	bool FileWatcher::contains(const std::string& key)
	{
		auto temp = m_FilePaths.find(key);
		return temp != m_FilePaths.end();
	}
}

