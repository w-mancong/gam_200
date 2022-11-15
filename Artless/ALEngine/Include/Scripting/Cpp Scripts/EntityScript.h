// File to be deleted after m3 
#ifndef	ENTITY_SCRIPT_H
#define ENTITY_SCRIPT_H

namespace ALEngine
{
	struct EntityScript
	{
		std::unordered_map<std::string, EntityFunc> Init{};
		std::unordered_map<std::string, EntityFunc> Update{};
		std::unordered_map<std::string, EntityFunc> Exit{};

		void AddInitFunction(std::string const& funcName, EntityFunc func);
		void AddUpdateFunction(std::string const& funcName, EntityFunc func);
		void AddExitFunction(std::string const& funcName, EntityFunc func);

		void RemoveInitFunction(std::string const& funcName);
		void RemoveUpdateFunction(std::string const& funcName);
		void RemoveExitFunction(std::string const& funcName);
	};
}

#endif