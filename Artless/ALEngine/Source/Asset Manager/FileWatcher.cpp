/*!
file: FileWatcher.cpp
author	 :	Chan Jie Ming Stanley	(90%)
co-author:	Wong Man Cong			(10%)
email:	c.jiemingstanley\@digipen.edu
		w.mancong\@digipen.edu
brief: This file contains function definition for FileWatcher. FileWatcher handles the
	   detection of changes in files using files' last write time.

All content :copyright: 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#include "pch.h"

namespace
{
	const std::filesystem::path basePath = "Assets"; //base file path
}

namespace ALEngine::Engine
{
	std::atomic<b8> FileWatcher::m_Pause{ false };

    FileWatcher::FileWatcher()
	{
		//creating a record of files from the base directory and their last modification time
		for (auto& file : std::filesystem::recursive_directory_iterator(basePath))
		{
			if (file.path().string().find("Dev") != std::string::npos || file.is_directory())
				continue;
			m_FilePaths[file.path().string()] = std::filesystem::last_write_time(file);
		}
	}

	void FileWatcher::Start()
	{
		while (GetAppStatus())
		{
			if (m_Pause)
				continue;

			b8 should_delay = false;

			auto tempIt = m_FilePaths.begin();

			while (tempIt != m_FilePaths.end())
			{
				//check if file path still exists, if not remove from unordered map of filePaths
				if (!std::filesystem::exists(tempIt->first))
				{
					//need to define a function to alert assetmanager
					if(tempIt->first.find(".meta") == std::string::npos)
						AssetManager::Instance()->Alert(tempIt->first, FileStatus::Erased);

					should_delay = true;

					//remove from unordered map of filePaths
					tempIt = m_FilePaths.erase(tempIt);
				}
				else
				{
					tempIt++;
				}
			}

			//check if a file was created or modified
			for (auto const& file : std::filesystem::recursive_directory_iterator(basePath))
			{
				std::string const& filePath = file.path().string();
				if (filePath.find("Dev") != std::string::npos || file.is_directory())
					continue;
				try
				{
					auto currentfileLastwritetime = std::filesystem::last_write_time(file);

					//if detect new file creation
					if (!contains(filePath))
					{
						m_FilePaths[filePath] = currentfileLastwritetime;

						//need to define a function to alert assetmanager
						if (filePath.find(".meta") == std::string::npos)
							AssetManager::Instance()->Alert(file.path().string(), FileStatus::Created);
					}
					else //detect file modifications or changes
					{
						if (m_FilePaths[filePath] != currentfileLastwritetime)
						{
							m_FilePaths[filePath] = currentfileLastwritetime;

							//need to define a function to alert assetmanager
							if (filePath.find(".meta") == std::string::npos)
								AssetManager::Instance()->Alert(file.path().string(), FileStatus::Modified);
						}
					}
				}
				catch ([[maybe_unused]] std::exception const& e)
				{
					AL_CORE_WARN(e.what());
				}
			}
		}
	}

	void FileWatcher::SetPause(b8 pause)
	{
		m_Pause = pause;
	}

	bool FileWatcher::contains(const std::string& key)
	{
		auto temp = m_FilePaths.find(key);
		return temp != m_FilePaths.end();
	}

	namespace
	{
		void FileWatcherWorkerThread(void)
		{
			FileWatcher watcher{};
			watcher.Start();
		}
	}

	void RunFileWatcherThread(void)
	{
		std::thread worker{ FileWatcherWorkerThread };
		worker.detach();
	}
}

