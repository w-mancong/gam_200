#include <pch.h>

namespace ALEngine
{
	void EntityScript::AddLoadFunction(std::string const& funcName)
	{
		Load[funcName] = GetLoadFunction(funcName);
	}

	void EntityScript::AddInitFunction(std::string const& funcName)
	{
		Init[funcName] = GetInitFunction(funcName);
	}

	void EntityScript::AddUpdateFunction(std::string const& funcName)
	{
		Update[funcName] = GetUpdateFunction(funcName);
	}

	void EntityScript::AddFreeFunction(std::string const& funcName)
	{
		Free[funcName] = GetFreeFunction(funcName);
	}

	void EntityScript::AddUnloadFunction(std::string const& funcName)
	{
		Unload[funcName] = GetUnloadFunction(funcName);
	}

	void EntityScript::RemoveLoadFunction(std::string const& funcName)
	{
		Load.erase(funcName);
	}

	void EntityScript::RemoveInitFunction(std::string const& funcName)
	{
		Init.erase(funcName);
	}

	void EntityScript::RemoveUpdateFunction(std::string const& funcName)
	{
		Update.erase(funcName);
	}

	void EntityScript::RemoveFreeFunction(std::string const& funcName)
	{
		Free.erase(funcName);
	}

	void EntityScript::RemoveUnloadFunction(std::string const& funcName)
	{
		Unload.erase(funcName);
	}
}