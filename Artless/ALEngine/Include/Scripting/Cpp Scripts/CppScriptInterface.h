/*!
file:	CppScriptInterface.h
author:	Wong Man Cong
email:	w.mancong\@digipen.edu
brief:	This file provides an interface for creating cpp scripts
		(Not really a scripting component as we still need to recompile the code)

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#ifndef	CPP_SCRIPT_INTERFACE_H
#define CPP_SCRIPT_INTERFACE_H

namespace ALEngine
{
	/*!*********************************************************************************
		\brief A name alias to be used for running scripts
	***********************************************************************************/
	using EntityFunc = std::function<void(ECS::Entity)>;

	/*!*********************************************************************************
		\brief Get a function pointer to a load function

		\param [in] funcName: Name of the function
	***********************************************************************************/
	EntityFunc GetLoadFunction(std::string const& funcName);

	/*!*********************************************************************************
		\brief Get a function pointer to an init function

		\param [in] funcName: Name of the function
	***********************************************************************************/
	EntityFunc GetInitFunction(std::string const& funcName);

	/*!*********************************************************************************
		\brief Get a function pointer to an update function

		\param [in] funcName: Name of the function
	***********************************************************************************/
	EntityFunc GetUpdateFunction(std::string const& funcName);

	/*!*********************************************************************************
		\brief	Get a function pointer to a free function

		\param [in] funcName: Name of the function
	***********************************************************************************/
	EntityFunc GetFreeFunction(std::string const& funcName);

	/*!*********************************************************************************
		\brief Get a function pointer to an unload function

		\param [in] funcName: Name of the function
	***********************************************************************************/
	EntityFunc GetUnloadFunction(std::string const& funcName);

	/*!*********************************************************************************
		\brief To register all the functions that will be included inside the cpp script systems
	***********************************************************************************/
	void RegisterCppScripts(void);
}

#endif