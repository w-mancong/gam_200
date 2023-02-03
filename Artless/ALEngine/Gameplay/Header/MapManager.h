/*!
file:	MapManager.h
author: Lucas Nguyen
email:	l.nguyen\@digipen.edu
brief:	This file contains function declarations for the MapManager class.
		The MapManager keeps track of the current Map that is being selected.

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#ifndef MAP_MANAGER_H
#define MAP_MANAGER_H

namespace Gameplay
{
	/*!*********************************************************************************
		\brief
		Map Manager that keeps track of the current map for the scene/level.
		Singleton.
	***********************************************************************************/
	class MapManager : public ALEngine::Templates::Singleton<MapManager>
	{
	public:
		/*!*********************************************************************************
			\brief
			Sets the map path for the map
			\param map_path
			File Path of the Map
		***********************************************************************************/
		void SetMapPath(std::string map_path);

		/*!*********************************************************************************
			\brief
			Deserializes and assigns the specified Map to be the current Map
			\param map_path
			File Path of the Map
		***********************************************************************************/
		void DeserializeMap(std::string map_path);

		/*!*********************************************************************************
			\brief
			Gets the Width of the Map
			\return
			Returns Width of the Map
		***********************************************************************************/
		u32 GetWidth(void);

		/*!*********************************************************************************
			\brief
			Gets the Height of the Map
			\return
			Returns Height of the Map
		***********************************************************************************/
		u32 GetHeight(void);

		/*!*********************************************************************************
			\brief
			Gets the Map which is an array of strings.
			Each string specifies what each tile contains.
			i.e. "Empty", "Player", etc.
			\return
			Returns a string pointer to the Map array
		***********************************************************************************/
		std::string* GetMap(void);

	private:
		/*!*********************************************************************************
			\brief
			Default Constructor for the MapManager
		***********************************************************************************/
		MapManager(void);

		/*!*********************************************************************************
			\brief
			Default Destructor for the MapManager
		***********************************************************************************/
		~MapManager(void);

		std::string m_MapPath{ "" };				// File Path to the Current Map
		u32 m_Width{ 0 }, m_Height{ 0 };			// Width and Height of the Map
		std::string* m_Map{ nullptr };				// Keeps track of the items found within this map

		// Required for Singleton to function
		friend class ALEngine::Templates::Singleton<MapManager>;
		friend class ALEngine::Memory::StaticMemory;
	};
}

#endif