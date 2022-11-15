#ifndef	CPP_SCRIPT_INTERFACE_H
#define CPP_SCRIPT_INTERFACE_H

namespace ALEngine
{
	using EntityFunc = std::function<void(ECS::Entity)>;

	EntityFunc GetInitFunction(std::string const& funcName);
	EntityFunc GetUpdateFunction(std::string const& funcName);
	EntityFunc GetExitFunction(std::string const& funcName);

	void RegisterCppScripts(void);
}

#endif