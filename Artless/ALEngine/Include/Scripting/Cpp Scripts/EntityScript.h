// File to be deleted after m3 
#ifndef	ENTITY_SCRIPT_H
#define ENTITY_SCRIPT_H

namespace ALEngine
{
	struct EntityScript
	{
		std::unordered_map<std::string, EntityFunc> Load{};
		std::unordered_map<std::string, EntityFunc> Init{};
		std::unordered_map<std::string, EntityFunc> Update{};
		std::unordered_map<std::string, EntityFunc> Free{};
		std::unordered_map<std::string, EntityFunc> Unload{};

		void AddLoadFunction(std::string const& funcName);
		void AddInitFunction(std::string const& funcName);
		void AddUpdateFunction(std::string const& funcName);
		void AddFreeFunction(std::string const& funcName);
		void AddUnloadFunction(std::string const& funcName);

		void RemoveLoadFunction(std::string const& funcName);
		void RemoveInitFunction(std::string const& funcName);
		void RemoveUpdateFunction(std::string const& funcName);
		void RemoveUnloadFunction(std::string const& funcName);
		void RemoveFreeFunction(std::string const& funcName);
	};
}

#endif