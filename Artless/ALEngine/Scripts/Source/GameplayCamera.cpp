/*!
file:	GameplayCamera.cpp
author:	Wong Man Cong
email:	w.mancong\@digipen.edu
brief:	This file contain function definition for a gameplay camera

		All content � 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#include <pch.h>
#include <GameplayCamera.h>
#include <GameplaySystem.h>

namespace ALEngine::Script
{	
	namespace
	{
		using namespace ECS;
	}

	f32 GameplayCamera::L_Boundary{ std::numeric_limits<f32>::min() },
		GameplayCamera::R_Boundary{ std::numeric_limits<f32>::max() },
		GameplayCamera::B_Boundary{ std::numeric_limits<f32>::min() },
		GameplayCamera::T_Boundary{ std::numeric_limits<f32>::max() };

	void GameplayCamera::Init([[maybe_unused]] ECS::Entity en)
	{
		auto const& map = Gameplay::MapManager::Instance()->GetMap();
		u64 const W = map[0].size(), H = map.size();

		L_Boundary = B_Boundary = 0.0f - TILE_SIZE;
		R_Boundary = static_cast<f32>(TILE_SIZE) * static_cast<f32>(W);
		T_Boundary = static_cast<f32>(TILE_SIZE) * static_cast<f32>(H);

		WIDTH = GetCamera().Width(), HEIGHT = GetCamera().Height();
	}

	void GameplayCamera::Update([[maybe_unused]] ECS::Entity en)
	{
		Math::vec3& pos = GetCamera().Position();

		f32 const xScreen = static_cast<f32>( Input::GetScreenResX() ),
				  yScreen = static_cast<f32>( Input::GetScreenResY()) ;

		f32 const xPadding = xScreen * PADDING_PERCENTAGE,
				  yPadding = yScreen * PADDING_PERCENTAGE;

		f32 const xMouse = static_cast<f32>( Input::GetMousePosX() ), 
				  yMouse = static_cast<f32>( Input::GetMousePosY() );

		// Move camera to the left
		if (xMouse <= xPadding)
			pos.x -= CAMERA_SPEED * Time::m_DeltaTime;
		// Move camera to the right
		else if (xMouse >= (xScreen - xPadding))
			pos.x += CAMERA_SPEED * Time::m_DeltaTime;

		// Move camera down
		if (yMouse <= yPadding)
			pos.y -= CAMERA_SPEED * Time::m_DeltaTime;
		// Move camera up
		else if (yMouse >= (yScreen - yPadding))
			pos.y += CAMERA_SPEED * Time::m_DeltaTime;

		ConfinePosition(pos);
		ECS::UpdateUIpositions();
	}

	void GameplayCamera::Free(ECS::Entity en)
	{
		L_Boundary = B_Boundary = std::numeric_limits<f32>::min();
		R_Boundary = T_Boundary = std::numeric_limits<f32>::max();
	}

	void GameplayCamera::SetBoundary(f32 L, f32 R, f32 T, f32 B)
	{
		L_Boundary = L;
		R_Boundary = R;
		T_Boundary = T;
		B_Boundary = B;
	}

	void GameplayCamera::ConfinePosition(Math::vec3& pos) const
	{
		if (pos.x < L_Boundary)
			pos.x = L_Boundary;
		else if (pos.x > R_Boundary - WIDTH)
			pos.x = R_Boundary - WIDTH;

		if (pos.y < B_Boundary)
			pos.y = B_Boundary;
		else if (pos.y > T_Boundary - HEIGHT)
			pos.y = T_Boundary - HEIGHT;
	}
}