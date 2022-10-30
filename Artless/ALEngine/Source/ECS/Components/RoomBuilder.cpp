#include "pch.h"


namespace ALEngine::ECS::Component
{
	RoomBuilder::RoomBuilder()
	{
	}

	RoomBuilder::~RoomBuilder()
	{
	}

	bool RoomBuilder::isGridInside(ALEngine::Math::Vector2 grid)
	{
		return grid.x >= 0 && grid.x < m_Instance.m_RoomSize.x&& grid.y >= 0 && grid.y < m_Instance.m_RoomSize.y;
	}

	void RoomBuilder::Awake()
	{
		m_Instance = *this;
	}

	void RoomBuilder::Start()
	{

	}

	void RoomBuilder::BuildRoom(Cell cellprefab, Transform room, ALEngine::Math::Vector2 roomsize, ALEngine::Math::Vector2 cellsize)
	{
	}

}