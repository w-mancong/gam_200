#include <pch.h>

namespace ALEngine
{
	void EntityScript::AddInitFunction(std::string const& funcName, EntityFunc func)
	{
		Init[funcName] = func;
	}

	void EntityScript::AddUpdateFunction(std::string const& funcName, EntityFunc func)
	{
		Update[funcName] = func;
	}

	void EntityScript::AddExitFunction(std::string const& funcName, EntityFunc func)
	{
		Exit[funcName] = func;
	}

	void EntityScript::RemoveInitFunction(std::string const& funcName)
	{
		Init.erase(funcName);
	}

	void EntityScript::RemoveUpdateFunction(std::string const& funcName)
	{
		Update.erase(funcName);
	}

	void EntityScript::RemoveExitFunction(std::string const& funcName)
	{
		Exit.erase(funcName);
	}
}