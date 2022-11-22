#pragma once
#ifndef FILE_WATCHER_H
#define FILE_WATCHER_H

/*!
file: FileWatcher.h
author	 :	Chan Jie Ming Stanley
co-author:	Wong Man Cong
email:	c.jiemingstanley\@digipen.edu
		w.mancong\@digipen.edu
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
		/*!*********************************************************************************
	    \brief
	    constructor for creating a record of files from the base directory and their last 
		modification time
		\param [in] delay:
		time interval before needing to check files for any changes again
	    ***********************************************************************************/
		FileWatcher();

		~FileWatcher(void) = default;

		/*!*********************************************************************************
	    \brief
	    Monitor the "pathtoWatch" for any changes and if there is a case of a change execute
		the user supplied "action" function
	    ***********************************************************************************/
		void Start();

		/*!*********************************************************************************
		\brief If true, file watcher will not be updated
		***********************************************************************************/
		static void SetPause(b8 pause);

	private:
		std::unordered_map<std::string, std::filesystem::file_time_type> m_FilePaths;
		bool m_Running = true;
		static std::atomic<b8> m_Pause;

		/*!*********************************************************************************
		\brief
	    Check if the unordered map "filePaths" contains a given key
		\param [in] key:
		the key for filePaths to check if it has the key
		***********************************************************************************/
		bool contains(const std::string& key);
	};
	
	void RunFileWatcherThread(void);
}
#endif // !FILE_WATCHER_H
