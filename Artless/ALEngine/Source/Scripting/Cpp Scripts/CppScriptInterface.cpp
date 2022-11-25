#include <pch.h>

// to include all the cpp script headers here
#include <Scripting/Cpp Scripts/Scripts/SkillHovering.h>
#include <Scripting/Cpp Scripts/Scripts/StatsHovering.h>

namespace ALEngine
{
	namespace
	{
		std::unordered_map<std::string, EntityFunc> loadList{};
		std::unordered_map<std::string, EntityFunc> initList{};
		std::unordered_map<std::string, EntityFunc> updateList{};
		std::unordered_map<std::string, EntityFunc> freeList{};
		std::unordered_map<std::string, EntityFunc> unloadList{};

		void AddIntoLoadList(std::string const& funcName, EntityFunc func)
		{
			loadList[funcName] = func;
		}

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
			freeList[funcName] = func;
		}

		void AddIntoUnloadList(std::string const& funcName, EntityFunc func)
		{
			unloadList[funcName] = func;
		}

#define AL_ADD_LOAD_FUNC(Name) AddIntoLoadList(#Name, Name)
#define AL_ADD_INIT_FUNC(Name) AddIntoInitList(#Name, Name)
#define AL_ADD_UPDATE_FUNC(Name) AddIntoUpdateList(#Name, Name)
#define AL_ADD_EXIT_FUNC(Name) AddIntoExitList(#Name, Name)
#define AL_ADD_UNLOAD_FUNC(Name) AddIntoUnloadList(#Name, Name)
	}

	EntityFunc GetLoadFunction(std::string const& funcName)
	{
		return loadList[funcName];
	}

	EntityFunc GetInitFunction(std::string const& funcName)
	{
		return initList[funcName];
	}

	EntityFunc GetUpdateFunction(std::string const& funcName)
	{
		return updateList[funcName];
	}

	EntityFunc GetFreeFunction(std::string const& funcName)
	{
		return freeList[funcName];
	}

	EntityFunc GetUnloadFunction(std::string const& funcName)
	{
		return unloadList[funcName];
	}

	void RegisterCppScripts(void)
	{
#pragma region Load Functions
#pragma endregion

#pragma region Init Functions	
		AL_ADD_INIT_FUNC(SkillInit);
		AL_ADD_INIT_FUNC(StatsInit);
#pragma endregion

#pragma region Update Functions
#pragma endregion

#pragma region Exit Functions
		AL_ADD_EXIT_FUNC(SkillReset);
		AL_ADD_EXIT_FUNC(StatsReset);
#pragma endregion

#pragma region Unload Functions
#pragma endregion
	}
}