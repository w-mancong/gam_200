#pragma once
#ifndef FILE_WATCHER_H
#define FILE_WATCHER_H

/*!
file: FileWatcher.h
author: Chan Jie Ming Stanley
email: c.jiemingstanley\@digipen.edu
brief: This file contains function declaration for FileWatcher. FileWatcher handles the
       detection of changes in files using files' last write time.

		All content :copyright: 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/



namespace ALEngine::Engine
{
	enum class FileStatus
	{
		Created,
		Modified,
		Erased
	};

	class FileWatcher
	{
	public:
		std::string m_PathToWatch;

		//the time interval at which to check the base folder for changes
		std::chrono::duration<int, std::milli> m_Delay;

		/*!*********************************************************************************
	    \brief
	    constructor for creating a record of files from the base directory and their last 
		modification time
		\param [in] pathtoWatch:
		Path to the where files need to be watch
		\param [in] delay:
		time interval before needing to check files for any changes again
	    ***********************************************************************************/
		FileWatcher(std::string pathToWatch, std::chrono::duration<int, std::milli> delay);

		virtual ~FileWatcher(void) = default;

		/*!*********************************************************************************
	    \brief
	    Monitor the "pathtoWatch" for any changes and if there is a case of a change execute
		the user supplied "action" function
	    ***********************************************************************************/
		void Start();

		/*!*********************************************************************************
	    \brief
		Setter function to set the file path to watch
		\param [in] watchFilepath:
		The new file Path to set for file watching
	    ***********************************************************************************/
		void SetWatchPath(std::string watchFilepath);

	private:
		std::unordered_map<std::string, std::filesystem::file_time_type> m_FilePaths;
		bool m_Running = true;

		/*!*********************************************************************************
		\brief
	    Check if the unordered map "filePaths" contains a given key
		\param [in] key:
		the key for filePaths to check if it has the key
		***********************************************************************************/
		bool contains(const std::string& key);
	};
}
#endif // !FILE_WATCHER_H
