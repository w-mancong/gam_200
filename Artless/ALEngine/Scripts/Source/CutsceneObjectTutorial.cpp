/*!
file:	CutsceneObjectTutorial.cpp
author:	Lucas Nguyen
email:	l.nguyen\@digipen.edu
brief:	This file contain function definitions for the cutscene object for the tutorial.

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#include <pch.h>
#include <CutsceneObjectTutorial.h>

namespace ALEngine::Script
{
	namespace
	{
		using namespace ECS;
	}

	void CutsceneObjectTutorial::Init(ECS::Entity en)
	{
		Engine::Scene::CutsceneManager::Instance()->Init(en);
		Engine::Scene::CutsceneManager::Instance()->PlaySequence("Tutorial Intro");
		Time::m_Scale = 0.f;

		m_EntityBoxy = Coordinator::Instance()->GetEntityByTag("Boxy");
		m_BoxyXform = &Coordinator::Instance()->GetComponent<Transform>(m_EntityBoxy);

		Gameplay::TutorialManager::Instance()->BeginTutorial();
	}

	void CutsceneObjectTutorial::Update([[maybe_unused]]ECS::Entity en)
	{
		using namespace Gameplay;
		Engine::Scene::CutsceneManager::Instance()->Update();

		if (Engine::Scene::CutsceneManager::Instance()->CutsceneIsPlaying() == false)
			TutorialManager::Instance()->NextState();
		else
		{
			ECS::SetActive(true, m_EntityBoxy);
			if (m_BoxyXform == nullptr)
				return;

			static const f32 topBound{ m_BoxyXform->localPosition.y + 15.f },
				botBound{ m_BoxyXform->localPosition.y - 5.f };
			const f32 boxySpeed{ 20.f };
			static b8 movingDown{ false };

			if (movingDown)
			{
				m_BoxyXform->localPosition.y -= (boxySpeed * Time::m_ActualDeltaTime);

				if (m_BoxyXform->localPosition.y <= botBound)
					movingDown = false;
			}
			else
			{
				m_BoxyXform->localPosition.y += (boxySpeed * Time::m_ActualDeltaTime);

				if (m_BoxyXform->localPosition.y >= topBound)
					movingDown = true;
			}
		}
	}

}