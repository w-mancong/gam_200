/*!
file:	CutsceneManager.cpp
author: Lucas Nguyen
email:	l.nguyen\@digipen.edu
brief:	This contains definitions for the Cutscene Manager class, which is in charge of
		cutscenes within the engine.

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#include <pch.h>

namespace ALEngine::Engine::Scene
{
	namespace
	{
		const f32 WAIT_TIME{ 1.f };										// Time to wait after player clicks to skip current cutscene/cutscene text
		const c8* TEXTBOX_PATH{ "Assets/Images/DialogueBox.png" };		// Path for the Dialogue Box images
	}

	CutsceneManager::CutsceneManager(void)
	{
	}

	void CutsceneManager::Init(void)
	{
		m_CutsceneObject = Instantiate("Cutscene Object");
		ECS::GetSceneGraph().FindImmediateChildren(m_CutsceneObject);
		std::vector<s32> const& children = ECS::GetSceneGraph().GetChildren();
		for (s32 child : children)
		{
			EntityData const& data = Coordinator::Instance()->GetComponent<EntityData>(child);

			if (data.tag == "Black Overlay")
				m_BlackOverlay = child;
			else if (data.tag == "Dialogue Box")
				m_DialogueBox = child;
		}
		std::cout << std::endl;
	}

	void CutsceneManager::PlaySequence(std::string sequence)
	{
		// Check if sequence exists
		if (m_Cutscenes.find(sequence) == m_Cutscenes.end())
			return;	// Exit since sequence does not exist

		m_CutsceneIsPlaying = true;
		m_SelectedSequence = sequence;
		m_CurrentCutscene = m_Cutscenes[sequence].begin();
		m_CurrentCutscene->m_CutsceneTimeCountdown = m_CurrentCutscene->m_CutsceneTime;

		// Make Obj Appear
		EntityData& objData = Coordinator::Instance()->GetComponent<EntityData>(m_CutsceneObject);
		objData.active = true;
	}

	void CutsceneManager::StopSequence(void)
	{
		m_CutsceneIsPlaying = false;

		// Make Obj Disappear
		EntityData& objData = Coordinator::Instance()->GetComponent<EntityData>(m_CutsceneObject);
		objData.active = false;
	}

	void CutsceneManager::Update(void)
	{
		if (!m_CutsceneIsPlaying)
			return;

		// This is so the player does not spam the cutscene skip or skip too fast by accident
		static f32 wait_timer{ 0.f };

		// Check if next cutsene, either based on timer or if player pressed key
		if (wait_timer <= 0.f &&
			(!m_CurrentCutscene->UpdateTime()
				|| Input::KeyTriggered(KeyCode::MouseLeftButton)
				|| Input::KeyTriggered(KeyCode::Enter)))
		{
			std::next(m_CurrentCutscene);
			m_CurrentCutscene->m_CutsceneTimeCountdown = m_CurrentCutscene->m_CutsceneTime;
			if (m_CurrentCutscene == m_Cutscenes[m_SelectedSequence].end())
				m_CutsceneIsPlaying = false;

			wait_timer = WAIT_TIME;
		}
		else if (wait_timer > 0.f)
			wait_timer -= Time::m_DeltaTime;
	}
	
	bool Cutscene::UpdateTime(void)
	{
		// If no timer, just exit
		if (!m_HasTimer)
			return true;

		m_CutsceneTimeCountdown -= Time::m_DeltaTime;
		if (m_CutsceneTimeCountdown <= 0.f)
			return false;

		return true;
	}
}
