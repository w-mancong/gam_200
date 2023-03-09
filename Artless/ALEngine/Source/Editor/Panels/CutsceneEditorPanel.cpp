/*!
file:	CutsceneEditorPanel.cpp
author: Lucas Nguyen
email:	l.nguyen\@digipen.edu
brief:	This file contains function definitions for the CutsceneEditorPanel class.
		The CutsceneEditorPanel class contains a Cutscene Editor that edits cutscenes
		and cutscene sequences.

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#include <pch.h>

#if EDITOR
#include "imgui/cpp/imgui_stdlib.h"

namespace ALEngine::Editor
{
	CutsceneEditorPanel::CutsceneEditorPanel(void)
	{
	}
	
	CutsceneEditorPanel::~CutsceneEditorPanel(void)
	{
	}
	
	void CutsceneEditorPanel::OnImGuiRender(void)
	{
		if (m_PanelIsOpen == false)
			return;

		if (!ImGui::Begin("Cutscene Editor", &m_PanelIsOpen))
		{
			ImGui::End();
			return;
		}

		Update();

		ImGui::End();
	}

	void CutsceneEditorPanel::Update(void)
	{
		using namespace Engine::Scene;

		// Get style
		ImGuiStyle& style = ImGui::GetStyle();

		// Exit if no cutscenes
		if (!CutsceneManager::Instance()->HasCutscene())
		{
			static std::string newSequenceName{};
			f32 text_width = ImGui::GetContentRegionAvail().x * 0.6f;
			f32 text_height = ImGui::GetContentRegionAvail().y * 0.2f;
			f32 text_size = text_width + style.FramePadding.x * 2.f;
			f32 align = (ImGui::GetWindowSize().x - text_size) * 0.5f;

			ImGui::Text("You have no Sequences at the moment!");

			ImGui::NewLine(); ImGui::NewLine(); ImGui::SameLine(align);
			ImGui::InputTextWithHint("##New Sequence Name", "Sequence Name", &newSequenceName);

			f32 btn_width = text_width * 0.5f;
			f32 btn_height = text_height;
			f32 btn_size = btn_width + style.FramePadding.x * 2.f;
			align = (ImGui::GetWindowSize().x - btn_size) * 0.5f;

			ImGui::NewLine(); ImGui::SameLine(align);
			if (ImGui::Button("Add Sequence", { btn_size, 0 }) && newSequenceName != "")
			{
				CutsceneManager::Instance()->AddSequence(newSequenceName);
				newSequenceName = "";
			}
			return;
		}

		ImGui::BeginChild("##CutsceneEditorChild", { ImGui::GetContentRegionMax().x * 0.4f, 0.f });
		ImVec2 borderSize{ ImGui::GetContentRegionAvail().x * 0.95f, ImGui::GetContentRegionAvail().y * 0.3f };

		// Sequence Stuff
		{
			// Set the selected sequence name
			if(m_SelectedSequence == "")
				m_SelectedSequence = CutsceneManager::Instance()->m_Sequences.begin()->first;

			ImGui::NewLine(); ImGui::SameLine((ImGui::GetWindowContentRegionMax().x * 0.5f) - (borderSize.x * 0.5f));
			// Combo
			if (ImGui::BeginCombo("Sequence##CutsceneEditor", m_SelectedSequence.c_str()))
			{
				for (auto i : CutsceneManager::Instance()->m_Sequences)
				{
					if (ImGui::Selectable(i.first.c_str(), m_SelectedSequence == i.first))
					{
						m_SelectedSequence = i.first;
						m_SelectedCutsceneIndex = 0;
					}
				}

				ImGui::EndCombo();
			}

			ImGui::NewLine(); ImGui::SameLine((ImGui::GetWindowContentRegionMax().x * 0.5f) - (borderSize.x * 0.5f));
			if (ImGui::Button("Add Sequence##CutsceneEditor"))
				ImGui::OpenPopup("Add Sequence##CutsceneEditorPopup");

			f32 popup_len{ ALEditor::Instance()->m_MenuSize.x };
			ImGui::SetNextWindowSize({ popup_len, 0.f });
			if (ImGui::BeginPopup("Add Sequence##CutsceneEditorPopup"))
			{
				static std::string newName{};
				ImGui::InputTextWithHint("##AddSequenceName", "New Sequence Name", &newName);
				f32 align{ popup_len * 0.4f };
				ImGui::NewLine(); ImGui::SameLine(align);
				if (ImGui::Button("Add##Sequence To Be Added", { popup_len * 0.2f, 0.f }))
				{
					CutsceneManager::Instance()->AddSequence(newName);
					newName = "";
					ImGui::CloseCurrentPopup();
				}

				ImGui::EndPopup();
			}
		}
		
		// Cutscene Stuff
		if(m_SelectedSequence != "")
		{
			ImGui::NewLine();
			ImGui::SameLine((ImGui::GetWindowContentRegionMax().x * 0.5f) - (borderSize.x * 0.5f));
			ImGui::BeginChild("Cutscenes##PanelChild", { borderSize.x, 0.f }, true);
			{
				// Sequence not empty
				if (!CutsceneManager::Instance()->m_Sequences[m_SelectedSequence].empty())
				{
					for (auto i : CutsceneManager::Instance()->m_Sequences[m_SelectedSequence])
					{
						if (ImGui::Selectable(i.m_CutsceneName.c_str(), i.m_OrderIndex == m_SelectedCutsceneIndex))
						{
							m_SelectedCutsceneIndex = i.m_OrderIndex;
						}
					}
				}				

				f32 btn_len{ ImGui::GetContentRegionAvail().x * 0.8f };
				ImGui::NewLine(); ImGui::SameLine(ImGui::GetContentRegionAvail().x * 0.1f);
				if (ImGui::Button("Add Cutscene", { btn_len, 0.f }))
				{	// Adds empty cutscene
					Cutscene newCutscene{};
					newCutscene.m_OrderIndex = CutsceneManager::Instance()->m_Sequences[m_SelectedSequence].size();
					newCutscene.m_CutsceneName = m_SelectedSequence + ": Cutscene #" + std::to_string(newCutscene.m_OrderIndex);
					CutsceneManager::Instance()->AddCutscene(m_SelectedSequence, newCutscene);
				}
			}
			ImGui::EndChild();
			ImGui::EndChild();

			// Cutscene Not Empty
			if (!CutsceneManager::Instance()->m_Sequences[m_SelectedSequence].empty())
			{
				ImGui::SameLine();
				ImGuiWindowFlags flag{};
				ImGui::BeginChild("##SecondChildCutsceneEditor", { 0, 0 }, false, flag);
				{
					borderSize = { ImGui::GetContentRegionAvail().x * 0.95f, 0.f };
					ImGui::NewLine(); ImGui::SameLine((ImGui::GetContentRegionAvail().x * 0.5f) - (borderSize.x * 0.5f));
					ImGui::Text("Selected Cutscene");

					borderSize.y = ImGui::GetContentRegionAvail().y;
					ImGui::NewLine(); ImGui::SameLine((ImGui::GetWindowContentRegionMax().x * 0.5f) - (borderSize.x * 0.5f));
					ImGui::BeginChild("Selected Cutscene##PanelChild", borderSize, true);
					{
						Cutscene& selected = CutsceneManager::Instance()->m_Sequences[m_SelectedSequence][m_SelectedCutsceneIndex];
						ImGui::InputText("Name##CutsceneEditorCutsceneName", &selected.m_CutsceneName);

						s32 index{ static_cast<s32>(selected.m_OrderIndex) };
						ImGui::InputInt("Index##CutsceneEditorCutsceneName", &index, 1, 1, ImGuiInputTextFlags_ReadOnly);

						ImGui::InputFloat("Time##CutsceneEditorCutsceneName", &selected.m_CutsceneTime);

						ImGui::Checkbox("Has Image", &selected.m_HasImage);
						ImGui::SameLine();	ImGui::Checkbox("Has Text", &selected.m_HasText);

						ImGui::Checkbox("Has Timer", &selected.m_HasTimer);
						ImGui::SameLine();	 ImGui::Checkbox("Wait For Input", &selected.m_WaitForInput);
					}
					ImGui::EndChild();
				}
				ImGui::EndChild();
			}
		}
		else
		{
			ImGui::EndChild();
		}
	}
	
	void CutsceneEditorPanel::SetPanelIsOpen(b8 isOpen)
	{
		m_PanelIsOpen = isOpen;
	}
	
	b8 CutsceneEditorPanel::GetPanelIsOpen(void)
	{
		return m_PanelIsOpen;
	}
}

#endif