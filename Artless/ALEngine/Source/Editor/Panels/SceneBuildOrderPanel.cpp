/*!
file:	SceneBuildOrderPanel.cpp
author: Lucas Nguyen
email:	l.nguyen\@digipen.edu
brief:	This file contains function definitions for the SceneBuildOrderPanel class.
		The InspectorPanel class contains information and functions necessary for
		the editing the Build Order of scenes.

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#include "pch.h"

#if EDITOR

namespace ALEngine::Editor
{
	SceneBuildOrderPanel::SceneBuildOrderPanel(void)
	{
	}
	
	SceneBuildOrderPanel::~SceneBuildOrderPanel(void)
	{
	}
	
	void SceneBuildOrderPanel::OnImGuiRender(void)
	{
		// Begin ImGui
		if (!ImGui::Begin("Scene Build Order", &m_PanelIsOpen))
		{
			ImGui::End();
			return;
		}
		
		ImVec2 borderSize{ ImGui::GetContentRegionAvail().x * 0.8f, ImGui::GetContentRegionAvail().y * 0.8f };
		ImGui::SameLine((ImGui::GetWindowContentRegionMax().x * 0.5f) - (borderSize.x * 0.5f));
		ImGui::BeginChild("ContentBrowserPanel##PanelChild", borderSize, true);
		

		
		ImGui::EndChild();
		//Engine::Scene::

		ImGui::End();
	}
	
	void SceneBuildOrderPanel::SetPanelIsOpen(bool isOpen)
	{
		m_PanelIsOpen = isOpen;
	}
	
	bool SceneBuildOrderPanel::GetPanelIsOpen(void)
	{
		return m_PanelIsOpen;
	}
}

#endif