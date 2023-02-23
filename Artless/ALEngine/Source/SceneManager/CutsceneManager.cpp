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
	void CutsceneManager::PlaySequence(std::string sequence)
	{
		// Check if sequence exists
		if (m_Cutscenes.find(sequence) == m_Cutscenes.end())
			return;

		m_CutsceneIsPlaying = true;
		m_SelectedSequence = sequence;
		m_CurrentCutscene = m_Cutscenes[sequence].begin();
	}

	void CutsceneManager::Update(void)
	{
		if (!m_CutsceneIsPlaying)
			return;

		if (!m_CurrentCutscene->UpdateTime())
		{
			std::next(m_CurrentCutscene);

			if (m_CurrentCutscene == m_Cutscenes[m_SelectedSequence].end())
				m_CutsceneIsPlaying = false;
		}
	}
	
	bool Cutscene::UpdateTime(void)
	{
		m_TimeTaken -= Time::m_DeltaTime;
		if (m_TimeTaken <= 0.f)
			return false;

		return true;
	}
}
