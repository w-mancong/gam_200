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

	class FileWatcher : public Templates::Singleton<ALEditor>
	{
	public:

		std::string pathtoWatch;

		//the time interval at which to check the base folder for changes
		std::chrono::duration<int, std::milli> delay;






	private:
		std::unordered_map<std::string, std::filesystem::file_time_type> filepaths;
		bool running = true;



	};

}



#endif // !FILE_WATCHER_H
