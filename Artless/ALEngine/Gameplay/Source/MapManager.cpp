/*!
file:	MapManager.cpp
author: Lucas Nguyen
email:	l.nguyen\@digipen.edu
brief:	This file contains function definitions for the MapManager class.
		The MapManager keeps track of the current Map that is being selected.

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#include <pch.h>

#define TILE_DATA_PATH "Assets/Dev/Objects/TileEditorData.json"

namespace Gameplay
{
	MapManager::MapManager(void) : m_MapPath(""), m_Width(0), m_Height(0)
	{
		ReadTileData();
	}

	MapManager::~MapManager(void)
	{
	}

	std::string MapManager::GetMapPath(void)
	{
		return m_MapPath;
	}

	void MapManager::SetMapPath(std::string map_path)
	{
		m_MapPath = map_path;
	}

	void MapManager::DeserializeMap(std::string map_path)
	{
		using namespace ALEngine;
		using namespace rapidjson;

		c8* buffer = Utility::ReadBytes(map_path.c_str());
		assert(buffer != nullptr);

		// Set the map path
		m_MapPath = map_path;

		Document doc;
		doc.Parse(buffer);

		Memory::DynamicMemory::Delete(buffer);

		Value const& val{ *doc.Begin() };

		// Get Width
		if (val.HasMember("Width"))
			m_Width = val["Width"].GetInt();

		// Get Height
		if (val.HasMember("Height"))
			m_Height = val["Height"].GetInt();

		// Make sure width and height are not 0
		assert((m_Width != 0) && (m_Height != 0));

		// Make sure has TileMap
		assert(val.HasMember("Map"));

		// Check if m_Map is already populated
		if (m_Map.empty() == false)
			m_Map.clear();

		// Iterate to get all the map info for each tile
		for (u32 i{ 0 }; i < m_Height; ++i)
		{
			Value const& row_val = val["Map"][i];
			std::vector<std::string> row_tiles{};

			for (u32 j{ 0 }; j < m_Width; ++j)
				row_tiles.emplace_back(row_val[j].GetString());

			m_Map.emplace_back(row_tiles);
		}
	}

	void MapManager::ReadTileData(void)
	{
		using namespace rapidjson;

		std::string filePath_str{ TILE_DATA_PATH };
		c8* buffer = ALEngine::Utility::ReadBytes(filePath_str.c_str());

		assert(buffer);

		Document doc;
		doc.Parse(buffer);

		ALEngine::Memory::DynamicMemory::Delete(buffer);

		Value const& val{ *doc.Begin() };

		// Iterate all  values
		for (Value::ConstMemberIterator it = val.MemberBegin(); it != val.MemberEnd(); ++it)
			m_ImageMap[it->name.GetString()] = it->value.GetString();
	}

	u32 MapManager::GetWidth(void)
	{
		return m_Width;
	}

	u32 MapManager::GetHeight(void)
	{
		return m_Height;
	}

	std::vector<std::vector<std::string>> MapManager::GetMap(void)
	{
		return m_Map;
	}

	std::string MapManager::GetTileImage(std::string tileName)
	{
		auto it = m_ImageMap.find(tileName);
		if (it != m_ImageMap.end())
			return it->second;

		return "";
	}

	Guid MapManager::GetTileImageGuid(std::string tileName)
	{
		auto it = m_ImageGuidMap.find(tileName);
		if (it != m_ImageGuidMap.end())
			return it->second;

		return 0;
	}
}