/*!
file:	CppScriptSystem.h
author:	Wong Man Cong
email:	w.mancong\@digipen.edu
brief:	This file provides an interface for a cpp scripting system

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#ifndef	CPP_SCRIPT_SYSTEM_H
#define CPP_SCRIPT_SYSTEM_H

namespace ALEngine
{
	/*!*********************************************************************************
		\brief Register the cpp scripting system
	***********************************************************************************/
	void RegisterCppScriptSystem(void);

	/*!*********************************************************************************
		\brief Interface to run all the Load functions entities has
	***********************************************************************************/
	void LoadCppScripts(void);

	/*!*********************************************************************************
		\brief Interface to run all the Init function entities has
	***********************************************************************************/
	void InitCppScripts(void);

	/*!*********************************************************************************
		\brief Interface to run all the Update function entities has
	***********************************************************************************/
	void UpdateCppScripts(void);

	/*!*********************************************************************************
		\brief Interface to run all the Free function entities has
	***********************************************************************************/
	void FreeCppScripts(void);

	/*!*********************************************************************************
		\brief Interface to run all the Unload function entities has
	***********************************************************************************/
	void UnloadCppScripts(void);
}

#endif