/*!
file:	TileEditorPanel.cpp
author:	Lucas Nguyen
email:	l.nguyen@digipen.edu
brief:	This file contains the function definitions for the TileEditorPanel class.
		The Tile Editor Panel creates or loads previous Tile Levels created.
		The TileEditorPanel class contains information and functions necessary for
		the Tile Editor Panel of the editor to be displayed.

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#include "pch.h"

namespace ALEngine::Editor
{
	TileEditorPanel::TileEditorPanel(void)
	{
		m_HasMapLoaded = false;
		m_CurrentLoadStage = LoadStage::CreateOrLoadSelection;
	}

	TileEditorPanel::~TileEditorPanel(void)
	{
	}

	void TileEditorPanel::OnImGuiRender(void)
	{
		if (m_PanelIsOpen == false)
			return;

		// Set constraints
		ImGui::SetNextWindowSizeConstraints(m_PanelMin, ImGui::GetMainViewport()->WorkSize);

		if (!ImGui::Begin("Tile Editor"), &m_PanelIsOpen, 0)
		{
			ImGui::End();
			return;
		}

		// No level loaded, so select 
		if (m_HasMapLoaded == false)
			SelectMap();
		else 
			Update();

		ImGui::End();
	}

	void TileEditorPanel::SetPanelMin(Math::Vec2 min)
	{
		m_PanelMin = ImVec2(min.x, min.y);
	}

	void TileEditorPanel::SetPanelIsOpen(b8 isOpen)
	{
		m_PanelIsOpen = isOpen;
	}

	b8 TileEditorPanel::GetPanelIsOpen(void)
	{
		return m_PanelIsOpen;
	}

	void TileEditorPanel::CreateNewMap(void)
	{
		
	}

	void TileEditorPanel::SelectMap(void)
	{
		if(m_CurrentLoadStage == LoadStage::CreateOrLoadSelection)
		{
			ImGuiStyle &style = ImGui::GetStyle();

			f32 btn_width = ImGui::GetContentRegionAvail().x * 0.75f;
			f32 btn_size = btn_width + style.FramePadding.x * 2.f;
			f32 align = (ImGui::GetWindowSize().x - btn_size) * 0.5f;

			ImGui::SameLine(align);
			if (ImGui::Button("Create New Map##TileEditor", ImVec2(btn_width, 0.f)))
			{
				m_CurrentLoadStage = LoadStage::CreateMap;
				return;
			}

			ImGui::NewLine();
			ImGui::SameLine(align);
			if (ImGui::Button("Load Map##TileEditor", ImVec2(btn_width, 0.f)))
			{
				m_CurrentLoadStage = LoadStage::LoadMap;
				return;
			}
		}
		else if (m_CurrentLoadStage == LoadStage::LoadMap)
		{

		}
		else if (m_CurrentLoadStage == LoadStage::CreateMap)
		{
			CreateNewMap();
		}
	}

	void TileEditorPanel::Update(void)
	{
	}

}
