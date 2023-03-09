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
#include "imgui/cpp/imgui_stdlib.h"

namespace ALEngine::Editor
{
	namespace 
	{
		// File buffer size
		const u32 FILE_BUFFER_SIZE{ 1024 };
	}
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
		
		f32 strLen{ ImGui::CalcTextSize("Scene Build Index").x };
		ImGui::SameLine(ImGui::GetContentRegionAvail().x * 0.5f - strLen * 0.5f);
		ImGui::Text("Scene Build Index");
		
		ImVec2 borderSize{ ImGui::GetContentRegionAvail().x * 0.8f, ImGui::GetContentRegionAvail().y * 0.8f };
		ImGui::NewLine();
		ImGui::SameLine((ImGui::GetWindowContentRegionMax().x * 0.5f) - (borderSize.x * 0.5f));
		ImGui::BeginChild("SceneBuildOrderPanel##PanelChild", borderSize, true);
		{
			std::vector<std::string> scenesList = Engine::Scene::GetScenesList();

			s32 to_remove{ -1 };
			for (u32 i = 0; i < scenesList.size(); ++i)
			{
				std::string text{ std::to_string(i) + " " + scenesList[i] };
				ImGui::Selectable(text.c_str());

				if (ImGui::BeginDragDropSource())
				{
					ImGui::SetDragDropPayload("HIERARCHY_ENTITY", &i, sizeof(ECS::Entity));
					ImGui::EndDragDropSource();
				}
				else if (ImGui::BeginDragDropTarget())
				{
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("HIERARCHY_ENTITY"))
					{
						u32 oldIdx = *(ECS::Entity*)payload->Data;
						Engine::Scene::InsertScene(i, oldIdx);
					}
					ImGui::EndDragDropTarget();
				}

				ImGui::SameLine();

				ImGui::SetItemAllowOverlap();
				ImGui::PushID(text.c_str());
				if (ImGui::SmallButton("X"))
				{
					to_remove = static_cast<s32>(i);
				}
				ImGui::PopID();
			}

			if (to_remove != -1)
				Engine::Scene::RemoveScene(scenesList[to_remove].c_str());
		}
		ImGui::EndChild();

		if (ImGui::BeginDragDropTarget())
		{
			// Payload flag
			ImGuiDragDropFlags payload_flag{ 0 };
			//payload_flag |= ImGuiDragDropFlags_AcceptNoDrawDefaultRect;

			// Get Drag and Drop Payload
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("SCENE_ITEM", payload_flag))
			{
				// Get filepath
				size_t fileLen;	c8 filePath[FILE_BUFFER_SIZE];
				wcstombs_s(&fileLen, filePath, FILE_BUFFER_SIZE, (const wchar_t*)payload->Data, payload->DataSize);

				Engine::Scene::AddScene(filePath);
			}
		}

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