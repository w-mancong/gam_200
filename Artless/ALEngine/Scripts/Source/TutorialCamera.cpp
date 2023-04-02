/*!
file:	TutorialCamera.cpp
author:	Lucas Nguyen
email:	l.nguyen\@digipen.edu
brief:	This file contain function definition for a tutorial camera

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#include <pch.h>
#include <TutorialCamera.h>
#include <GameplaySystem.h>

namespace ALEngine::Script
{
	namespace
	{
		f32 constexpr const PADDING_PERCENTAGE = 0.01f, CAMERA_SPEED = 450.0f;
	}

	void TutorialCamera::Init([[maybe_unused]] ECS::Entity en)
	{
		auto const& map = Gameplay::MapManager::Instance()->GetMap();
		u64 const W = map[0].size(), H = map.size();

		m_LBound = m_BBound = 0.0f - TILE_SIZE;
		m_RBound = static_cast<f32>(TILE_SIZE) * static_cast<f32>(W);
		m_TBound = static_cast<f32>(TILE_SIZE) * static_cast<f32>(H);

		m_Width = ECS::GetCamera().Width(), m_Height = ECS::GetCamera().Height();

		Engine::Camera& camera = ECS::GetCamera();
		camera.Position().y = 0;

		if (camera.Position().x < m_LBound)
			camera.Position().x = m_LBound;
		else if (camera.Position().x > m_RBound - m_Width)
			camera.Position().x = m_RBound - m_Width;

		if (camera.Position().y < m_BBound)
			camera.Position().y = m_BBound;
		else if (camera.Position().y > m_TBound - m_Height)
			camera.Position().y = m_TBound - m_Height;

		Gameplay::TutorialManager::Instance()->SetCameraOriginalPos(camera.Position());
		ECS::UpdateUIpositions();
	}

	void TutorialCamera::Update([[maybe_unused]] ECS::Entity en)
	{
		// Don't move camera if cutscene is playing
		if (Engine::Scene::CutsceneManager::Instance()->CutsceneIsPlaying())
			return;
		
		switch (Gameplay::TutorialManager::Instance()->GetState())
		{
		case Gameplay::TutorialState::TUTORIAL_MOVE_CAMERA_1:
			MoveCameraToTileDestroyer();
			break;
		case Gameplay::TutorialState::TUTORIAL_MOVE_CAMERA_2:
			MoveCameraToSummoner();
			break;
		case Gameplay::TutorialState::TUTORIAL_FINAL_FIGHT:
			UpdateCameraMovement();
			break;
		default:
			break;
		}
		//UpdateCameraMovement();

#if EDITOR
		// For debugging purposes, to go to the next state
		if (Input::KeyTriggered(KeyCode::N))
			Gameplay::TutorialManager::Instance()->NextState();
#endif
	}

	void TutorialCamera::MoveCameraToTileDestroyer(void)
	{
		constexpr f32 targetX{ 400.f };
		
		Engine::Camera& camera = ECS::GetCamera();
		camera.Position().x += CAMERA_SPEED * Time::m_DeltaTime * 0.7f;

		if(camera.Position().x >= targetX)
		{
			camera.Position().x = targetX;
			Gameplay::TutorialManager::Instance()->NextState();

			// Make the first one an | and second one a Z
			Gameplay::TutorialManager::Instance()->GetGameplaySystem()->pattern_List.insert(Gameplay::TutorialManager::Instance()->GetGameplaySystem()->pattern_List.begin(),
				Gameplay::TutorialManager::Instance()->GetGameplaySystem()->pattern_Default[5]);
			Gameplay::TutorialManager::Instance()->GetGameplaySystem()->pattern_List.insert(Gameplay::TutorialManager::Instance()->GetGameplaySystem()->pattern_List.begin(),
				Gameplay::TutorialManager::Instance()->GetGameplaySystem()->pattern_Default[2]);

			// Set sprites for the Patterns
			for (u32 i{ 1 }; i <= 4; ++i)
			{
				std::string tile_icon = "next_tile_icon" + std::to_string(i);

				ECS::Entity tileEtt = Coordinator::Instance()->GetEntityByTag(tile_icon);

				Sprite& sprite = Coordinator::Instance()->GetComponent<Sprite>(tileEtt);
				sprite.id = Engine::AssetManager::Instance()->GetGuid(Gameplay::TutorialManager::Instance()->GetGameplaySystem()->pattern_List[i - 1].file_path);
			}
		}

		ECS::UpdateUIpositions();
	}

	void TutorialCamera::MoveCameraToSummoner(void)
	{
		constexpr f32 targetX{ 1300.f };

		Engine::Camera& camera = ECS::GetCamera();
		camera.Position().x += CAMERA_SPEED * Time::m_DeltaTime * 0.7f;

		if (camera.Position().x >= targetX)
		{
			camera.Position().x = targetX;
			Gameplay::TutorialManager::Instance()->SetAllAbilitiesOn();
			Gameplay::TutorialManager::Instance()->NextState();
		}

		ECS::UpdateUIpositions();
	}

	void TutorialCamera::UpdateCameraMovement(void)
	{
		Engine::Camera& camera = ECS::GetCamera();

		f32 const	xScreen = static_cast<f32>(Input::GetScreenResX()),
					yScreen = static_cast<f32>(Input::GetScreenResY());

		f32 const	xPadding = xScreen * PADDING_PERCENTAGE,
					yPadding = yScreen * PADDING_PERCENTAGE;

		f32 const	xMouse = static_cast<f32>(Input::GetMousePosX()),
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

		if (camera.Position().x < m_LBound)
			camera.Position().x = m_LBound;
		else if (camera.Position().x > m_RBound - m_Width)
			camera.Position().x = m_RBound - m_Width;

		if (camera.Position().y < m_BBound)
			camera.Position().y = m_BBound;
		else if (camera.Position().y > m_TBound - m_Height)
			camera.Position().y = m_TBound - m_Height;

		ECS::UpdateUIpositions();
	}
}