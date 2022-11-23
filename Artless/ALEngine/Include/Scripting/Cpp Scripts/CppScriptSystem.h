#ifndef	CPP_SCRIPT_SYSTEM_H
#define CPP_SCRIPT_SYSTEM_H

namespace ALEngine
{
	void RegisterCppScriptSystem(void);

	void LoadCppScripts(void);
	void InitCppScripts(void);
	void UpdateCppScripts(void);
	void FreeCppScripts(void);
	void UnloadCppScripts(void);
}

#endif