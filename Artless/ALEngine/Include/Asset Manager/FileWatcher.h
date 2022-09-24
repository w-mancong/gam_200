#pragma once
#ifndef FILE_WATCHER_H
#define FILE_WATCHER_H

/*!
file: FileWatcher.h
author: Chan Jie Ming Stanley
email: c.jiemingstanley\@digipen.edu
brief:

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

		std::string pathtoWatch;

		//the time interval at which to check the base folder for changes
		std::chrono::duration<int, std::milli> delay;


		/*!*********************************************************************************
	    \brief
	    function for keeping a record of files from the base directory and their last 
		modification time
	    ***********************************************************************************/
		FileWatcher(std::string pathtoWatch, std::chrono::duration<int, std::milli> delay);

		/*!*********************************************************************************
	    \brief
	    Monitor the "pathtoWatch" for any changes and if there is a case of a change execute
		the user supplied "action" function
	    ***********************************************************************************/
		void start(const std::function<void(std::string, FileStatus)>& action);


	private:
		std::unordered_map<std::string, std::filesystem::file_time_type> filePaths;
		bool running = true;


		/*!*********************************************************************************
		\brief
	    Check if "filePaths" contains a given key
		***********************************************************************************/
		bool contains(const std::string& key);

	};

}



#endif // !FILE_WATCHER_H
