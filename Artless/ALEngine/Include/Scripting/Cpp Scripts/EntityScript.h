/*!
file:	EntityScript.h
author:	Wong Man Cong
email:	w.mancong\@digipen.edu
brief:	This file contain a structure with variables for EntityScript

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#ifndef	ENTITY_SCRIPT_H
#define ENTITY_SCRIPT_H

namespace ALEngine
{
	/*!*********************************************************************************
		\brief Structure that contains a list of functions that will be run accordingly
		as the game flow
	***********************************************************************************/
	struct EntityScript
	{
		std::unordered_map<std::string, EntityFunc> Load{};
		std::unordered_map<std::string, EntityFunc> Init{};
		std::unordered_map<std::string, EntityFunc> Update{};
		std::unordered_map<std::string, EntityFunc> Free{};
		std::unordered_map<std::string, EntityFunc> Unload{};

		/*!*********************************************************************************
			\brief Add a load function

			\param [in] funcName: Name of load function to add
		***********************************************************************************/
		void AddLoadFunction(std::string const& funcName);

		/*!*********************************************************************************
			\brief Add an init function

			\param [in] funcName: Name of init function to add
		***********************************************************************************/
		void AddInitFunction(std::string const& funcName);

		/*!*********************************************************************************
			\brief Add an update function

			\param [in] funcName: Name of update function to add
		***********************************************************************************/
		void AddUpdateFunction(std::string const& funcName);

		/*!*********************************************************************************
			\brief Add a free function

			\param [in] funcName: Name of free function to add
		***********************************************************************************/
		void AddFreeFunction(std::string const& funcName);

		/*!*********************************************************************************
			\brief Add an unload function

			\param [in] funcName: Name of unload function to add
		***********************************************************************************/
		void AddUnloadFunction(std::string const& funcName);

		/*!*********************************************************************************
			\brief Remove a load function

			\param [in] funcName: Name of load function to remove
		***********************************************************************************/
		void RemoveLoadFunction(std::string const& funcName);

		/*!*********************************************************************************
			\brief Remove an init function

			\param [in] funcName: Name of init function to remove
		***********************************************************************************/
		void RemoveInitFunction(std::string const& funcName);

		/*!*********************************************************************************
			\brief Remove an update function

			\param [in] funcName: Name of update function to remove
		***********************************************************************************/
		void RemoveUpdateFunction(std::string const& funcName);

		/*!*********************************************************************************
			\brief Remove an unload function

			\param [in] funcName: Name of unload function to remove
		***********************************************************************************/
		void RemoveUnloadFunction(std::string const& funcName);

		/*!*********************************************************************************
			\brief Remove a free function

			\param [in] funcName: Name of free function to remove
		***********************************************************************************/
		void RemoveFreeFunction(std::string const& funcName);
	};
}

#endif