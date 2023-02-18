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
			Gets the map path
			\return
			Returns the File Path of the Map
		***********************************************************************************/
		std::string GetMapPath(void);

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
			Returns a vector of vector of string that is the Map array
		***********************************************************************************/
		std::vector<std::vector<std::string>> GetMap(void);

		/*!*********************************************************************************
			\brief
			Gets the filepath for the Tile Image
			\param tileName
			Name of the tile to get the image for
			\return
			Returns the filepath for the Tile Image.
			Empty string is returned if Image cannot be found.
		***********************************************************************************/
		std::string GetTileImage(std::string tileName);

		/*!*********************************************************************************
			\brief
			Gets the Guid for the Tile Image
			\param tileName
			Name of the tile to get the image for
			\return
			Returns the Guid for the Tile Image.
			0 is returned if Image cannot be found.
		***********************************************************************************/
		Guid GetTileImageGuid(std::string tileName);

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

		/*!*********************************************************************************
			\brief
			Reads the Saved Tile data, which is the data that links the Tiles to the Tile
			Images
		***********************************************************************************/
		void ReadTileData(void);

		std::string m_MapPath{ "" };						// File Path to the Current Map
		u32 m_Width{ 0 }, m_Height{ 0 };					// Width and Height of the Map
		std::vector<std::vector<std::string>> m_Map{};		// Keeps track of the items found within this map
		std::map<std::string, std::string> m_ImageMap{};	// Map of all the Tile Images
		std::map<std::string, Guid> m_ImageGuidMap{};		// Map of the Tile images Guid

		// Required for Singleton to function
		friend class ALEngine::Templates::Singleton<MapManager>;
		friend class ALEngine::Memory::StaticMemory;
	};
}

#endif