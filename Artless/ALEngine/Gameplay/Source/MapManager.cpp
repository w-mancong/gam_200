/*!
file:	MapManager.cpp
author: Lucas Nguyen
email:	l.nguyen\@digipen.edu
brief:	This file contains function definitions for the MapManager class.
		The MapManager keeps track of the current Map that is being selected.

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#include <pch.h>

namespace Gameplay
{
	MapManager::MapManager(void) : m_MapPath(""), m_Width(0), m_Height(0), m_Map(nullptr)
	{
	}

	MapManager::~MapManager(void)
	{
		if (m_Map != nullptr)
			ALEngine::Memory::DynamicMemory::Delete(m_Map);
	}

	void MapManager::SetMapPath(std::string map_path)
	{
		m_MapPath = map_path;
		
		// Deserialize the Map
		DeserializeMap(map_path);
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

		// Make sure width or height are not 0
		assert((m_Width != 0) || (m_Height != 0));

		// Make sure has TileMap
		assert(val.HasMember("Map"));

		// Check if m_Map is already populated
		if (m_Map != nullptr)
		{
			Memory::DynamicMemory::Delete(m_Map);
			m_Map = nullptr;
		}

		// Set new dimensions for m_Map
		u64 mapSize = static_cast<u64>(m_Width) * static_cast<u64>(m_Height);
		m_Map = Memory::DynamicMemory::New<std::string>(mapSize);

		// Iterate to get all the map info for each tile
		for (s32 i{ 0 }; i < m_Height; ++i)
		{
			Value const& row_val = val["Map"][i];

			for (s32 j{ 0 }; j < m_Width; ++j)
				m_Map[i * m_Width + j] = row_val[j].GetString();
		}

		std::cout << "Hello" << std::endl;
	}

	u32 MapManager::GetWidth(void)
	{
		return m_Width;
	}

	u32 MapManager::GetHeight(void)
	{
		return m_Height;
	}

	std::string* MapManager::GetMap(void)
	{
		return m_Map;
	}
}