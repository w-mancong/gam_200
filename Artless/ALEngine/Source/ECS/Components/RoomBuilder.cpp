#include "pch.h"


namespace ALEngine::ECS::Component
{
	RoomBuilder::RoomBuilder()
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

	void RoomBuilder::Awake()
	{
		//m_Instance = *this;
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

	
}