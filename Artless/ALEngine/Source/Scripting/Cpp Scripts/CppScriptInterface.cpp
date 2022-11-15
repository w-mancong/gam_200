#include <pch.h>

// to include all the cpp script headers here

namespace ALEngine
{
	namespace
	{
		std::unordered_map<std::string, EntityFunc> initList{};
		std::unordered_map<std::string, EntityFunc> updateList{};
		std::unordered_map<std::string, EntityFunc> exitList{};

		void AddIntoInitList(std::string const& funcName, EntityFunc func)
		{
			initList[funcName] = func;
		}

		void AddIntoUpdateList(std::string const& funcName, EntityFunc func)
		{
			updateList[funcName] = func;
		}

		void AddIntoExitList(std::string const& funcName, EntityFunc func)
		{
			exitList[funcName] = func;
		}

#define AL_ADD_INIT_FUNC(Name) AddIntoInitList(#Name, Name)
#define AL_ADD_UPDATE_FUNC(Name) AddIntoUpdateList(#Name, Name)
#define AL_ADD_EXIT_FUNC(Name) AddIntoExitList(#Name, Name)
	}

	EntityFunc GetInitFunction(std::string const& funcName)
	{
		return initList[funcName];
	}

	EntityFunc GetUpdateFunction(std::string const& funcName)
	{
		return updateList[funcName];
	}

	EntityFunc GetExitFunction(std::string const& funcName)
	{
		return exitList[funcName];
	}

	void RegisterCppScripts(void)
	{
#pragma region Init Functions
#pragma endregion

#pragma region Update Functions
#pragma endregion

#pragma region Exit Functions
#pragma endregion
	}
}