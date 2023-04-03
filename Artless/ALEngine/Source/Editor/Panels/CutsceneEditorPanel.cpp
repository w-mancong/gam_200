/*!
file:	CutsceneEditorPanel.cpp
author: Lucas Nguyen
email:	l.nguyen\@digipen.edu
brief:	This file contains function definitions for the CutsceneEditorPanel class.
		The CutsceneEditorPanel class contains a Cutscene Editor that edits cutscenes
		and cutscene sequences.

		All content � 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#include <pch.h>

#if EDITOR
#include "imgui/cpp/imgui_stdlib.h"

namespace ALEngine::Editor
{
	namespace
	{
		const f32 ASPECT_RATIO{ 9.f / 16.f };

		// File buffer size
		const u32 FILE_BUFFER_SIZE{ 1024 };
	}

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

		ImGuiWindowFlags winFlags = ImGuiWindowFlags_MenuBar;

		if (!ImGui::Begin("Cutscene Editor", &m_PanelIsOpen, winFlags))
		{
			ImGui::End();
			return;
		}

		UpdateMenuBar();
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
			//f32 text_height = ImGui::GetContentRegionAvail().y * 0.2f;
			f32 text_size = text_width + style.FramePadding.x * 2.f;
			f32 align = (ImGui::GetWindowSize().x - text_size) * 0.5f;

			ImGui::Text("You have no Sequences at the moment!");

			ImGui::NewLine(); ImGui::NewLine(); ImGui::SameLine(align);
			ImGui::InputTextWithHint("##New Sequence Name", "Sequence Name", &newSequenceName);

			f32 btn_width = text_width * 0.5f;
			//f32 btn_height = text_height;
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
			ImGui::SameLine();
			if (ImGui::Button("Delete Sequence##CutsceneEditor"))
				ImGui::OpenPopup("Delete Sequence##CutsceneEditorPopup");

			ImGui::SameLine();
			if (ImGui::Button("Remove Sequence##CutsceneEditor"))
				ImGui::OpenPopup("Remove Sequence##CutsceneEditorPopup");

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
					m_SelectedSequence = newName;
					newName = "";
					ImGui::CloseCurrentPopup();
				}

				ImGui::EndPopup();
			}

			ImGui::SetNextWindowSize({ popup_len, 0.f });
			if (ImGui::BeginPopup("Remove Sequence##CutsceneEditorPopup"))
			{
				ImGui::Text("Confirm Delete?");
				if (ImGui::Button("Yes##CutsceneSequenceDelete", { popup_len * 0.2f, 0.f }))
				{					
					CutsceneManager::Instance()->RemoveSequence(m_SelectedSequence);
					m_SelectedSequence = "";
					ImGui::CloseCurrentPopup();
				}
				ImGui::SameLine();
				if (ImGui::Button("No##CutsceneSequenceDelete", { popup_len * 0.2f, 0.f }))
				{
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
					u32 count{ 0 };
					for (auto i : CutsceneManager::Instance()->m_Sequences[m_SelectedSequence])
					{
						std::string cutscene = i.m_CutsceneName + "## Cutscene Name " + std::to_string(count++);
						if (ImGui::Selectable(cutscene.c_str(), i.m_OrderIndex == m_SelectedCutsceneIndex))
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
					newCutscene.m_OrderIndex = static_cast<u32>( CutsceneManager::Instance()->m_Sequences[m_SelectedSequence].size() );
					newCutscene.m_CutsceneName = m_SelectedSequence + ": Cutscene #" + std::to_string(newCutscene.m_OrderIndex);
					CutsceneManager::Instance()->AddCutscene(m_SelectedSequence, newCutscene);
				}
			}
			ImGui::EndChild();
			ImGui::EndChild();

			bool toDeleteCutscene{ false };

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

					ImGui::SameLine();
					if (ImGui::Button("Delete Cutscene##CutsceneEditor"))
						toDeleteCutscene = true;

					borderSize.y = ImGui::GetContentRegionAvail().y;
					ImGui::NewLine(); ImGui::SameLine((ImGui::GetWindowContentRegionMax().x * 0.5f) - (borderSize.x * 0.5f));
					ImGui::BeginChild("Selected Cutscene##PanelChild", borderSize, true);
					{
						Cutscene& selected = CutsceneManager::Instance()->m_Sequences[m_SelectedSequence][m_SelectedCutsceneIndex];
						ImGui::InputText("Name##CutsceneEditor", &selected.m_CutsceneName);

						s32 index{ static_cast<s32>(selected.m_OrderIndex) };
						ImGui::InputInt("Index##CutsceneEditor", &index, 1, 1, ImGuiInputTextFlags_ReadOnly);

						ImGui::Checkbox("Has Timer", &selected.m_HasTimer);
						ImGui::SameLine();	 ImGui::Checkbox("Wait For Input", &selected.m_WaitForInput);
						if (selected.m_HasTimer == true)
							ImGui::InputFloat("Time##CutsceneEditor", &selected.m_CutsceneTime);

						ImGui::Checkbox("Has Image", &selected.m_HasImage);

						if (selected.m_HasImage)
						{
							std::string fadeSelected{};
							switch (selected.m_FadeInType)
							{
							case FadeType::FADE_NONE:
								fadeSelected = "No Fade";
								break;
							case FadeType::FADE_FROM_BLACK:
								fadeSelected = "Fade From Black";
								break;
							case FadeType::FADE_FROM_WHITE:
								fadeSelected = "Fade From White";
								break;
							case FadeType::FADE_FROM_PREV:
								fadeSelected = "Fade From Previous";
								break;
							}
							// Fade In Stuff
							if (ImGui::BeginCombo("Fade In Type##CutsceneEditor", fadeSelected.c_str()))
							{
								for (u32 i{ 0 }; i < (u32)FadeType::FADE_TOTAL; ++i)
								{
									if (i == (u32)FadeType::FADE_FROM_BLACK)
									{
										if (ImGui::Selectable("Fade From Black", (u32)selected.m_FadeInType == i))
											selected.m_FadeInType = static_cast<FadeType>(FadeType::FADE_FROM_BLACK);
									}
									else if (i == (u32)FadeType::FADE_FROM_WHITE)
									{
										if (ImGui::Selectable("Fade From White", (u32)selected.m_FadeInType == i))
											selected.m_FadeInType = static_cast<FadeType>(FadeType::FADE_FROM_WHITE);
									}
									else if (i == (u32)FadeType::FADE_FROM_PREV)
									{
										if (ImGui::Selectable("Fade From Previous", (u32)selected.m_FadeInType == i))
											selected.m_FadeInType = static_cast<FadeType>(FadeType::FADE_FROM_PREV);
									}
									else if (i == (u32)FadeType::FADE_NONE)
									{
										if (ImGui::Selectable("No Fade", (u32)selected.m_FadeInType == i))
											selected.m_FadeInType = static_cast<FadeType>(FadeType::FADE_NONE);
									}
								}
								ImGui::EndCombo();
							}
							if (selected.m_FadeInType != FadeType::FADE_NONE)
								ImGui::InputFloat("Fade In Time##CutsceneEditor", &selected.m_FadeInTime, 0.f, 0.f, "%.3f", selected.m_FadeInType == FadeType::FADE_NONE ? ImGuiInputTextFlags_ReadOnly : 0);

							switch (selected.m_FadeOutType)
							{
							case FadeType::FADE_NONE:
								fadeSelected = "No Fade";
								break;
							case FadeType::FADE_TO_BLACK:
								fadeSelected = "Fade To Black";
								break;
							case FadeType::FADE_TO_WHITE:
								fadeSelected = "Fade To White";
								break;
							case FadeType::FADE_TO_NEXT:
								fadeSelected = "Fade To Next";
								break;
							}
							// Fade Out Stuff
							if (ImGui::BeginCombo("Fade Out Type##CutsceneEditor", fadeSelected.c_str()))
							{
								for (u32 i{ 0 }; i < (u32)FadeType::FADE_TOTAL; ++i)
								{
									if (i == (u32)FadeType::FADE_TO_BLACK)
									{
										if (ImGui::Selectable("Fade To Black", (u32)selected.m_FadeOutType == i))
											selected.m_FadeOutType = static_cast<FadeType>(FadeType::FADE_TO_BLACK);
									}
									else if (i == (u32)FadeType::FADE_TO_WHITE)
									{
										if (ImGui::Selectable("Fade To White", (u32)selected.m_FadeOutType == i))
											selected.m_FadeOutType = static_cast<FadeType>(FadeType::FADE_TO_WHITE);
									}
									else if (i == (u32)FadeType::FADE_TO_NEXT)
									{
										if (ImGui::Selectable("Fade To Next", (u32)selected.m_FadeOutType == i))
											selected.m_FadeOutType = static_cast<FadeType>(FadeType::FADE_TO_NEXT);
									}
									else if (i == (u32)FadeType::FADE_NONE)
									{
										if (ImGui::Selectable("No Fade", (u32)selected.m_FadeOutType == i))
											selected.m_FadeOutType = static_cast<FadeType>(FadeType::FADE_NONE);
									}
								}
								ImGui::EndCombo();
							}
							if (selected.m_FadeOutType != FadeType::FADE_NONE)
								ImGui::InputFloat("Fade Out Time##CutsceneEditor", &selected.m_FadeOutTime, 0.f, 0.f, "%.3f", selected.m_FadeOutType == FadeType::FADE_NONE ? ImGuiInputTextFlags_ReadOnly : 0);
						}
						// Image
						if(selected.m_HasImage)
						{
							if (selected.m_CutsceneImageFilePath != "")
							{
								Guid id = Engine::AssetManager::Instance()->GetGuid(selected.m_CutsceneImageFilePath.c_str());
								u64 texture = (u64)Engine::AssetManager::Instance()->GetButtonImage(id);
								ImVec2 winSize = ImGui::GetContentRegionAvail();
								ImGui::Image(reinterpret_cast<ImTextureID>(texture), { winSize.x * 0.5f, winSize.x * 0.5f * ASPECT_RATIO }, { 0, 1 }, { 1, 0 });
							}
							ImGui::InputTextWithHint("Image File Path##CutsceneEditorImage", "File Path", &selected.m_CutsceneImageFilePath, ImGuiInputTextFlags_ReadOnly);

							// Drag Drop!
							if (ImGui::BeginDragDropTarget())
							{
								// Payload flag
								ImGuiDragDropFlags payload_flag{ 0 };
								//payload_flag |= ImGuiDragDropFlags_AcceptNoDrawDefaultRect;

								// Get Drag and Drop Payload
								if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ASSET_ITEM", payload_flag))
								{
									// Get filepath
									size_t fileLen;	c8 filePath[FILE_BUFFER_SIZE];
									wcstombs_s(&fileLen, filePath, FILE_BUFFER_SIZE, (const wchar_t*)payload->Data, payload->DataSize);

									// Check if image (png or jpg)
									std::string fileString = filePath;
									if (fileString.find(".png") != std::string::npos)
									{
										selected.m_CutsceneImageFilePath = fileString;
									}
									else
									{
										AL_CORE_ERROR("A .jpg or .png file is required!");
									}
								}
								ImGui::EndDragDropTarget();
							}
						}

						ImGui::Checkbox("Has Text", &selected.m_HasText);
						if (selected.m_HasText) { ImGui::SameLine();	ImGui::Checkbox("Text Is Above", &selected.m_TextIsAbove); }

						// Text
						if (selected.m_HasText)
						{
							if (selected.m_CutsceneText != "")
							{
								ImGui::Text("Cutscene Text");
								ImGui::BeginChild("Text Box##CutsceneEditor", { 0.f, ImGui::GetContentRegionAvail().x * 0.4f }, true);
								ImGui::TextWrapped(selected.m_CutsceneText.c_str());
								ImGui::EndChild();
							}
							ImGui::InputText("Cutscene Text##CutsceneEditor", &selected.m_CutsceneText);
						}
					}
					ImGui::EndChild();
				}

				if (toDeleteCutscene && CutsceneManager::Instance()->m_Sequences[m_SelectedSequence].empty() == false)
				{
					CutsceneManager::Instance()->m_Sequences[m_SelectedSequence].erase(CutsceneManager::Instance()->m_Sequences[m_SelectedSequence].begin() + m_SelectedCutsceneIndex);
					m_SelectedCutsceneIndex = 0;
				}

				ImGui::EndChild();
			}
		}
		else
		{
			ImGui::EndChild();
		}
	}

	void CutsceneEditorPanel::UpdateMenuBar(void)
	{
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::Selectable("Save##CutsceneEditorPanel"))
			{
				// Save Sequence
				if (m_SelectedSequence != "")
					Engine::Scene::CutsceneManager::Instance()->SerializeSequence(m_SelectedSequence);
			}

			ImGui::EndMenuBar();
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