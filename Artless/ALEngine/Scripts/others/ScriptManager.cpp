#include <pch.h>
#include "ScriptManager.h"

namespace ALEngine::Script
{
	namespace
	{
		std::vector <std::string> scriptNamesList;
	}

	void InitScriptManager(void)
	{
		const c8* path{ "..\\ALEngine\\Scripts\\Header" };
		auto dirIt = std::filesystem::directory_iterator(path);

		u64 fileCount = static_cast<u64>( std::count_if(
			begin(dirIt),
			end(dirIt),
			[](auto& entry) { return entry.is_regular_file(); }
		) );

		u64 it{};
		scriptNamesList.resize(fileCount);
		std::filesystem::path const scriptsPath{ path };
		for (const auto& entry : std::filesystem::directory_iterator(scriptsPath))
		{
			std::string const& fileName = std::filesystem::relative(entry.path(), scriptsPath).filename().string();
			scriptNamesList[it++] = fileName.substr(0, fileName.find_last_of('.'));
		}
	}

	std::vector<std::string> const& GetAllScripts(void)
	{
		return scriptNamesList;
	}
}