#include "pch.h"


namespace ALEngine::ECS::Component
{
	RoomBuilder::RoomBuilder(): m_RoomSize(10, 10),
	m_CellSize( 1.f,1.f ),
	m_Instance(this)
	{
	}

	RoomBuilder::RoomBuilder(s32 roomSizeX, s32 roomSizeY, f32 cellSizeX, f32 cellSizeY):m_RoomSize(roomSizeX, roomSizeY),
	m_CellSize(cellSizeX, cellSizeY),
	m_Instance(this)
	{
	}

	RoomBuilder::~RoomBuilder()
	{
	}

	bool RoomBuilder::IsGridInside(ALEngine::Math::Vector2Int grid)
	{
		//return grid.x >= 0 && grid.x < m_Instance.m_RoomSize.x&& grid.y >= 0 && grid.y < m_Instance.m_RoomSize.y;
		return false;
	}

	Cell RoomBuilder::GetCell(ALEngine::Math::Vector2Int cellPos)
	{
		s32 cellposition = cellPos.x * m_RoomSize.y + cellPos.y;
		return m_CellMap[cellposition];
	}

	void RoomBuilder::Awake()
	{
		m_Instance = this;
	}

	void RoomBuilder::Start()
	{
		if (m_RoomToLoad)
		{

		}
		else
		{
			
		}

	}

	void RoomBuilder::BuildRoom(Cell cellprefab, Transform room, ALEngine::Math::Vector2Int currentSize, ALEngine::Math::Vector2Int cellsize)
	{
		s32 roomSize = m_RoomSize.x * m_RoomSize.y;
		m_CellMap[roomSize];

		for (int i = 0; i < currentSize.x; ++i)
		{
			for (int j = 0; j < currentSize.y; ++j)
			{

			}
		}
	}
}