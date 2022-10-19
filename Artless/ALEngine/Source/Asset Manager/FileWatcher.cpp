/*!
file: FileWatcher.cpp
author: Chan Jie Ming Stanley
email: c.jiemingstanley\@digipen.edu
brief: This file contains function definition for FileWatcher. FileWatcher handles the
	   detection of changes in files using files' last write time.

All content :copyright: 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#include "pch.h"

namespace
{
	const std::filesystem::path basePath = "Assets"; //base file path
		// Container used to check for orphan meta files
	std::vector<std::string> metaFiles, assetFiles;
	u64 constexpr RESERVE_SIZE{ 100 };
	// Function to track meta files, if got orphan .meta files must delete

	void TrackMetaFiles(void)
	{
		const std::filesystem::path basePath = "Assets"; //base file path

		for (auto& file : std::filesystem::recursive_directory_iterator(basePath))
		{
			if (file.path().string().find("Dev") != std::string::npos || file.is_directory())
				continue;

			std::string const& filePath = file.path().string();
			if (filePath.find(".meta") != std::string::npos)
				metaFiles.push_back(filePath);
			else
				assetFiles.push_back(filePath);
		}

		for (auto it1 = assetFiles.begin(); it1 != assetFiles.end(); ++it1)
		{
			std::string meta = *it1 + ".meta";
			auto it2 = std::find(metaFiles.begin(), metaFiles.end(), meta);

			if (it2 != metaFiles.end())
				metaFiles.erase(it2);
		}

		//if orphan meta file, then delete
		for (auto it = metaFiles.begin(); it != metaFiles.end(); ++it)
		{
			if (remove(it->c_str()))
			{
				char error[1024];
				strerror_s(error, errno); strcat_s(error, "\n");
				std::cerr << "Error: " << error;
			}
		}
		assetFiles.clear();
		metaFiles.clear();
	}
}

namespace ALEngine::Engine
{
    FileWatcher::FileWatcher(std::chrono::duration<int, std::milli> delay) :
	m_Delay{ delay }
	{
		metaFiles.reserve(RESERVE_SIZE); assetFiles.reserve(RESERVE_SIZE);
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
		//while (m_Running && GetApplicationStatus())
		//{
			//wait for the milliseconds which is the "delay" time
			std::this_thread::sleep_for(m_Delay);

			auto tempIt = m_FilePaths.begin();

			while (tempIt != m_FilePaths.end())
			{
				//check if file path still exists, if not remove from unordered map of filePaths
				if (!std::filesystem::exists(tempIt->first))
				{
					//need to define a function to alert assetmanager
					if(tempIt->first.find(".meta") == std::string::npos)
						AssetManager::Instance()->Alert(tempIt->first, FileStatus::Erased);

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

				auto currentfileLastwritetime = std::filesystem::last_write_time(file);

				//if detect new file creation
				if (!contains(filePath))
				{
					m_FilePaths[filePath] = currentfileLastwritetime;

					//need to define a function to alert assetmanager
					if(filePath.find(".meta") == std::string::npos)
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

			//respond to file watcher alerts of any changes to file
			TrackMetaFiles();
		//}
	}

	bool FileWatcher::contains(const std::string& key)
	{
		auto temp = m_FilePaths.find(key);
		return temp != m_FilePaths.end();
	}

	namespace
	{
		//void FileWatcherWorkerThread(void)
		//{
		//	FileWatcher watcher{ std::chrono::milliseconds(0) };
		//	watcher.Start();
		//}
		//std::thread worker{ FileWatcherWorkerThread };
	}

	void RunFileWatcher(void)
	{
		//worker.join();
	}
}

