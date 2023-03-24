/*!
file:	TutorialCamera.cpp
author:	Lucas Nguyen
email:	l.nguyen\@digipen.edu
brief:	This file contain function definition for a tutorial camera

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#include <pch.h>
#include <TutorialCamera.h>

namespace ALEngine::Script
{
	namespace
	{
		f32 constexpr const PADDING_PERCENTAGE = 0.01f, CAMERA_SPEED = 450.0f;
	}

	void TutorialCamera::Init([[maybe_unused]] ECS::Entity en)
	{
		AL_CORE_INFO("Hello World");
		Transform& xform = Coordinator::Instance()->GetComponent<Transform>(en);
		Engine::Camera& cam = ECS::GetCamera();

		f32 const xScreen = static_cast<f32>(Input::GetScreenResX()),
			yScreen = static_cast<f32>(Input::GetScreenResY());

		cam.Position().x = xform.localPosition.x - xScreen * 0.5f;
		cam.Position().y = xform.localPosition.y - yScreen;

		ECS::UpdateUIpositions();
	}

	void TutorialCamera::Update([[maybe_unused]] ECS::Entity en)
	{
		// Don't move camera if cutscene is playing
		if (Engine::Scene::CutsceneManager::Instance()->CutsceneIsPlaying())
			return;

		UpdateCameraMovement();

#if EDITOR
		// For debugging purposes, to go to the next state
		if (Input::KeyTriggered(KeyCode::N))
			Gameplay::TutorialManager::Instance()->NextState();
#endif
	}

	void TutorialCamera::UpdateCameraMovement(void)
	{
		Engine::Camera& camera = ECS::GetCamera();

		f32 const xScreen = static_cast<f32>(Input::GetScreenResX()),
			yScreen = static_cast<f32>(Input::GetScreenResY());

		f32 const xPadding = xScreen * PADDING_PERCENTAGE,
			yPadding = yScreen * PADDING_PERCENTAGE;

		f32 const xMouse = static_cast<f32>(Input::GetMousePosX()),
			yMouse = static_cast<f32>(Input::GetMousePosY());

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
		ECS::UpdateUIpositions();

	}
}