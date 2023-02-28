/*!
file:	GameplayCamera.cpp
author:	Wong Man Cong
email:	w.mancong\@digipen.edu
brief:	This file contain function definition for a gameplay camera

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#include <pch.h>
#include <GameplayCamera.h>

namespace ALEngine::Script
{	
	namespace
	{
		f32 constexpr const PADDING_PERCENTAGE = 0.1f, CAMERA_SPEED = 20.0f;
	}

	void GameplayCamera::Update([[maybe_unused]] ECS::Entity en)
	{
		Engine::Camera& camera = ECS::GetCamera();

		f32 const xScreen = static_cast<f32>( Input::GetScreenResX() ),
				  yScreen = static_cast<f32>( Input::GetScreenResY()) ;

		f32 const xPadding = xScreen * PADDING_PERCENTAGE,
				  yPadding = yScreen * PADDING_PERCENTAGE;

		f32 const xMouse = static_cast<f32>( Input::GetMousePosX() ), 
				  yMouse = static_cast<f32>( Input::GetMousePosY() );

		// Move camera to the left
		if (xMouse <= xPadding)
		{
			camera.Position().x -= CAMERA_SPEED * Time::m_DeltaTime;
		}
		// Move camera to the right
		else if (xMouse >= (xScreen - xPadding))
		{
			camera.Position().x += CAMERA_SPEED * Time::m_DeltaTime;
		}
		
		// Move camera down
		if (yMouse <= yPadding)
		{
			camera.Position().y -= CAMERA_SPEED * Time::m_DeltaTime;
		}
		// Move camera up
		else if (yMouse >= (yScreen - yPadding))
		{
			camera.Position().y += CAMERA_SPEED * Time::m_DeltaTime;
		}
	}
}