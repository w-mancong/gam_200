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
		return grid.x >= 0 && grid.x < instance.m_RoomSize.x&& grid.y >= 0 && grid.y < instance.m_RoomSize.y;
	}

	void RoomBuilder::Awake()
	{
		instance = *this;
	}

	void RoomBuilder::Start()
	{
	}

	void RoomBuilder::BuildRoom(Cell cellprefab, Transform room, ALEngine::Math::Vector2 roomsize, ALEngine::Math::Vector2 cellsize)
	{
	}

}