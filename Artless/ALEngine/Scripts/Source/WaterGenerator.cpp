/*!
file:	WaterGenerator.cpp
author:	Wong Man Cong
email:	w.mancong\@digipen.edu
brief:	This file contain function definition for generating and moving water background

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#include <pch.h>
#include <WaterGenerator.h>
#include <GameplaySystem.h>
#include <Engine/GSM/GameStateManager.h>

namespace ALEngine::Script
{
	namespace
	{
		using namespace ECS;
	}

	void WaterGenerator::Init(ECS::Entity en)
	{
		// Retrieve the size of the map and the water entity
		auto const& map = Gameplay::MapManager::Instance()->GetMap();
		f32 const MAP_WIDTH = static_cast<f32>(map[0].size()) * TILE_SIZE, MAP_HEIGHT = static_cast<f32>(map.size()) * TILE_SIZE;
		
		Entity water_en = Instantiate("water");
		Math::vec2 const waterScale = Coordinator::Instance()->GetComponent<Transform>(water_en).scale;
		u64 const COL = static_cast<u64>(MAP_WIDTH  / waterScale.x) + 2,
				  ROW = static_cast<u64>(MAP_HEIGHT / waterScale.y) + 2;
		Coordinator::Instance()->DestroyEntity(water_en);

		for (u64 i{}; i < ROW; ++i)
		{
			for (u64 j{}; j < COL; ++j)
			{
				water_en = Instantiate("water");
				//Math::vec3& pos = Coordinator::Instance()->GetComponent<Transform>(water_en).localPosition;
				Math::vec3 pos = { (f32)j * waterScale.x, (f32)i * waterScale.y, 0.0f };

				GetSceneGraph().FindImmediateChildren( static_cast<s32>(water_en) );
				std::vector<s32> const& children{ GetSceneGraph().GetChildren() };

				Water w{};
				for (s32 child : children)
				{
					EntityData const& ed = Coordinator::Instance()->GetComponent<EntityData>( static_cast<Entity>(child) );
					Coordinator::Instance()->GetComponent<Transform>(static_cast<Entity>(child)).position = pos;
					if (ed.tag == "second_layer")
						w.second_layer = static_cast<Entity>(child);
					else if (ed.tag == "first_layer")
						w.first_layer = static_cast<Entity>(child);
				}
				w.parent = water_en;
				water.emplace_back(w);
			}
		}

		L_Bound = -waterScale.x, R_Bound = waterScale.x * static_cast<f32>(COL - 1);
		B_Bound = -waterScale.y, T_Bound = waterScale.y * static_cast<f32>(ROW - 1);
	}

	void WaterGenerator::Update(ECS::Entity en)
	{
		for (Water const& w : water)
		{
			{	// Move first layer
				Transform& trans = Coordinator::Instance()->GetComponent<Transform>(w.first_layer);
				trans.position.x -= WATER_SPEED[0] * Time::m_DeltaTime;
				if (trans.position.x <= L_Bound)
					trans.position.x = R_Bound;

				trans.position.y -= WATER_SPEED[0] * Time::m_DeltaTime;
				if (trans.position.y <= B_Bound)
					trans.position.y = T_Bound;
			}

			{	// Move second layer
				Transform& trans = Coordinator::Instance()->GetComponent<Transform>(w.second_layer);
				trans.position.x += WATER_SPEED[1] * Time::m_DeltaTime;
				if (trans.position.x >= R_Bound)
					trans.position.x = L_Bound;

				trans.position.y += WATER_SPEED[1] * Time::m_DeltaTime;
				if (trans.position.y >= T_Bound)
					trans.position.y = B_Bound;
			}
		}
	}

	void WaterGenerator::Free(ECS::Entity en)
	{

	}
}