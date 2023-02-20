#if EDITOR

#ifndef	SCRIPT_MANAGER_H
#define SCRIPT_MANAGER_H

#include <vector>
#include <string>

namespace ALEngine::Script
{
	void InitScriptManager(void);
	std::vector<std::string> const& GetAllScripts(void);
}

#endif

#endif