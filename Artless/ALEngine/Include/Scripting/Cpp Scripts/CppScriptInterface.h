#ifndef	CPP_SCRIPT_INTERFACE_H
#define CPP_SCRIPT_INTERFACE_H

namespace ALEngine
{
	using EntityFunc = std::function<void(ECS::Entity)>;

	EntityFunc GetLoadFunction(std::string const& funcName);
	EntityFunc GetInitFunction(std::string const& funcName);
	EntityFunc GetUpdateFunction(std::string const& funcName);
	EntityFunc GetFreeFunction(std::string const& funcName);
	EntityFunc GetUnloadFunction(std::string const& funcName);

	void RegisterCppScripts(void);
}

#endif