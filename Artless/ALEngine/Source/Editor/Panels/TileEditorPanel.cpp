/*!
file:	TileEditorPanel.cpp
author:	Lucas Nguyen
email:	l.nguyen\@digipen.edu
brief:	This file contains the function definitions for the TileEditorPanel class.
		The Tile Editor Panel creates or loads previous Tile Levels created.
		The TileEditorPanel class contains information and functions necessary for
		the Tile Editor Panel of the editor to be displayed.

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#include "pch.h"

#if EDITOR
#include "imgui_internal.h"
#include <imgui/cpp/imgui_stdlib.h>
#define MIN_TILES_SHOWN 5
#define MAX_TILES_SHOWN 15
#define FILE_BUFFER_SIZE 100
#define TILE_EDITOR_DATA_PATH "Assets/Dev/Objects/TileEditorData.json"
#define PLUS_ICON "Assets/Dev/Images/plus.png"

namespace ALEngine::Editor
{
	TileEditorPanel::TileEditorPanel(void)
	{
		using namespace rapidjson;

		std::string filePath_str{ TILE_EDITOR_DATA_PATH };
		c8* buffer = Utility::ReadBytes(filePath_str.c_str());

		assert(buffer);

		Document doc;
		doc.Parse(buffer);

		Memory::DynamicMemory::Delete(buffer);

		Value const& val{ *doc.Begin() };

		// Iterate all  values
		for (Value::ConstMemberIterator it = val.MemberBegin(); it != val.MemberEnd(); ++it)
			m_ImageMap[it->name.GetString()] = it->value.GetString();

		m_HasMapLoaded = false;
		m_CurrentLoadStage = LoadStage::CreateOrLoadSelection;
		m_NumTilesSeen = MAX_TILES_SHOWN;

		m_SelectedTile = m_ImageMap.begin()->first;

		SaveTileEditorData();
	}

	TileEditorPanel::~TileEditorPanel(void)
	{
	}

	void TileEditorPanel::OnImGuiRender(void)
	{
		if (m_PanelIsOpen == false)
			return;

		// Set constraints
		if (m_HasMapLoaded)
			ImGui::SetNextWindowSizeConstraints(ImVec2(static_cast<f32>(ALEditor::Instance()->GetSceneWidth()),
				static_cast<f32>(ALEditor::Instance()->GetSceneHeight())), ImGui::GetMainViewport()->WorkSize);
		else
			ImGui::SetNextWindowSizeConstraints(m_PanelMin, ImGui::GetMainViewport()->WorkSize);

		ImGuiWindowFlags winFlags = 0;

		if (m_HasMapLoaded)
			winFlags = ImGuiWindowFlags_MenuBar;

		if (!ImGui::Begin("Tile Editor", &m_PanelIsOpen, winFlags))
		{
			ImGui::End();
			return;
		}

		// No level loaded, so select 
		if (m_HasMapLoaded == false)
			SelectMap();
		else
		{
			UpdateMenuBar();
			Update();
		}

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

	void TileEditorPanel::Update(void)
	{
		ImVec2 tilesArea = ImVec2(ImGui::GetContentRegionAvail().x * 0.6f, ImGui::GetContentRegionAvail().y);

		ImGuiWindowFlags childFlags = ImGuiWindowFlags_HorizontalScrollbar;
		if (ImGui::BeginChild("##TileEditor_TileArea", tilesArea, true, childFlags))
		{
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(1.f, 1.f));
			u32 tileNum{ 0 };
			m_TileSize = ImGui::GetContentRegionAvail().x / static_cast<f32>(m_NumTilesSeen);
			m_TileSize -= ImGui::GetStyle().WindowPadding.x;

			for (auto& row : m_TileMap)
			{
				ImGui::NewLine();
				for (auto& tile : row)
				{
					Guid id = Engine::AssetManager::Instance()->GetGuid(m_ImageMap[tile]);
					u64 texID = (u64)Engine::AssetManager::Instance()->GetButtonImage(id);

					ImGui::SameLine();
					std::string btnID = "##TileEditorTile" + std::to_string(tileNum++);
					if (ImGui::ImageButton(btnID.c_str(), reinterpret_cast<ImTextureID>(texID), 
						ImVec2(m_TileSize, m_TileSize), { 0, 1 }, { 1, 0 }))
					{
						tile = m_SelectedTile;
					}
				}
			}
			ImGui::PopStyleVar();

			ImGui::EndChild();
		}
		else
			ImGui::EndChild();

		ImGui::SameLine();
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2());
		if (ImGui::BeginChild("##TileEditor_Panel_Edit", ImGui::GetContentRegionAvail(), false))
		{
			ImGui::PopStyleVar();
			if (ImGui::BeginChild("##TileEditor_Settings", ImVec2(0.f, ImGui::GetContentRegionAvail().y * 0.4f), true))
			{
				ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x * 0.6f);
				ImGui::SliderInt("Tiles Shown##TileEditor", &m_NumTilesSeen, MIN_TILES_SHOWN, MAX_TILES_SHOWN);
				ImGui::PopItemWidth();
				ImGui::EndChild();
			}
			else
				ImGui::EndChild();

			if (ImGui::BeginChild("##TileEditor_ImagePanel", ImVec2(0.f, ImGui::GetContentRegionAvail().y), true))
			{
				ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(5.f, 5.f));
				[[maybe_unused]] f32 winWidth{ ImGui::GetContentRegionAvail().x };
				f32 width{ (ImGui::GetContentRegionAvail().x / 2.f) - ImGui::GetStyle().FramePadding.x };
				f32 textLen{ 0.f };
				
				static std::pair<std::string, std::string> selectedTileImage{ "", "" };
				u32 count{ 0 };
				for(auto& type : m_ImageMap)
				{	
					std::string name = type.first + "##TileEditorImage";

					ImGui::BeginChild(name.c_str(), ImVec2(width, width), true);

					// Text area
					if (selectedTileImage.first != type.first)
					{
						textLen = ImGui::CalcTextSize(type.first.c_str()).x;
						ImGui::SameLine((width - textLen) * 0.5f);
						ImGui::Text(type.first.c_str());
					}
					else
					{
						ImGui::InputText("##SelectedNameToBeEdited", &selectedTileImage.second);

						// Enter or click outside means exit
						if (Input::KeyTriggered(KeyCode::Enter) ||
							(!ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left)))
						{
							// Iterate through whole tilemap
							for (auto& row : m_TileMap)
							{
								for (auto& tile : row)
								{
									if (tile == type.first)
										tile = selectedTileImage.first;
								}
							}

							SaveMap(m_FilePath.c_str());

							// Change key of Image Map
							auto node = m_ImageMap.extract(type.first);
							node.key() = selectedTileImage.second;
							m_ImageMap.insert(std::move(node));
							selectedTileImage = { "", "" };

							// Save the Tile Editor Image Data
							SaveTileEditorData();
						}
					}

					// Check if double clicked
					if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
					{
						selectedTileImage.first = type.first;
						selectedTileImage.second = type.first;
					}

					Guid id = Engine::AssetManager::Instance()->GetGuid(type.second.c_str());
					u64 texture = (u64)Engine::AssetManager::Instance()->GetButtonImage(id);
					f32 ImageSize = ImGui::GetContentRegionAvail().y;

					ImGui::NewLine(); ImGui::SameLine((width - ImageSize) * 0.5f);
					ImGui::Image(reinterpret_cast<ImTextureID>(texture), { ImageSize, ImageSize }, { 0, 1 }, { 1, 0 });

					ImGui::EndChild();

					if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left) && selectedTileImage.second == "")
						m_SelectedTile = type.first;

					// Drag Drop for Selectable
					if (ImGui::BeginDragDropTarget())
					{
						// Payload flag
						ImGuiDragDropFlags payload_flag{ 0 };
						//payload_flag |= ImGuiDragDropFlags_AcceptNoDrawDefaultRect;

						// Get Drag and Drop Payload
						if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ASSET_ITEM", payload_flag))
						{
							// Get filepath
							size_t fileLen;	c8 dd_filepath[FILE_BUFFER_SIZE];
							wcstombs_s(&fileLen, dd_filepath, FILE_BUFFER_SIZE, (const wchar_t*)payload->Data, payload->DataSize);

							// Check if image (png or jpg)
							std::string fileString = dd_filepath;
							if (fileString.find(".jpg") != std::string::npos ||
								fileString.find(".png") != std::string::npos)
							{
								type.second = fileString;
								selectedTileImage = { "", "" };
								SaveTileEditorData();
							}
							else
							{
								AL_CORE_ERROR("A .jpg or .png file is required!");
							}
						}
						ImGui::EndDragDropTarget();
					}

					if (m_SelectedTile == type.first)
					{
						ImDrawFlags draw_flag = 0;
						ImGui::GetWindowDrawList()->AddRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), 
							ImColor(ALEditor::Instance()->m_ColorActive), 0.f, draw_flag, 3.f);
					}

					if (count++ % 2 == 0)
						ImGui::SameLine();
				}
				
				// Add Tile
				ImGui::BeginChild("Add Tile##TileEditorPanel", ImVec2(width, width), true);

				textLen = ImGui::CalcTextSize("Add Tile").x;
				ImGui::SameLine((width - textLen) * 0.5f);
				ImGui::Text("Add Tile");

				Guid id = Engine::AssetManager::Instance()->GetGuid("Assets/Dev/Images/plus_icon.png");
				u64 texture = (u64)Engine::AssetManager::Instance()->GetButtonImage(id);
				f32 ImageSize = ImGui::GetContentRegionAvail().y;

				ImGui::NewLine(); ImGui::SameLine((width - ImageSize) * 0.5f);
				ImGui::Image(reinterpret_cast<ImTextureID>(texture), { ImageSize, ImageSize }, { 0, 1 }, { 1, 0 });

				ImGui::EndChild();

				if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left) && selectedTileImage.second == "")
					ImGui::OpenPopup("Add Tile##TileEditorPopup");
				
				f32 popup_len = ALEditor::Instance()->m_MenuSize.x;
				ImGui::SetNextWindowSize({ popup_len, 0.f });
				if (ImGui::BeginPopup("Add Tile##TileEditorPopup"))
				{
					textLen = ImGui::CalcTextSize("Add New Tile!").x;
					ImGui::SameLine((popup_len - textLen) * 0.5f);
					ImGui::TextColored(ALEditor::Instance()->m_ColorActive, "Add New Tile!");
					
					ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
					
					static std::string name;
					ImGui::InputTextWithHint("##NewTileNameInput_TileEditor", "Tile Name", &name);
					ImGui::NewLine();

					ImGui::PopItemWidth();

					b8 nameFilled = !name.empty();

					textLen = ImGui::CalcTextSize("Add").x;
					ImGui::NewLine();  ImGui::SameLine((popup_len - textLen) * 0.5f);					
					if(nameFilled == false)
						ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);	

					if (ImGui::Button("Add") && nameFilled)
					{
						m_ImageMap[name] = "";
						SaveTileEditorData();
						ImGui::CloseCurrentPopup();
					}
					
					if (nameFilled == false)
						ImGui::PopStyleVar();

					ImGui::EndPopup();
				}

				ImGui::PopStyleVar();
				ImGui::EndChild();
			}
			else
				ImGui::EndChild();

			ImGui::EndChild();
		}
		else
		{
			ImGui::EndChild();
			ImGui::PopStyleVar();
		}
	}

	void TileEditorPanel::UpdateMenuBar(void)
	{
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("Setting##TileEditorMenuBar"))
			{
				// Save
				if (ImGui::MenuItem("Save##TileEditorMenuBar"))
				{
					SaveMap(m_FilePath.c_str());
				}

				// Save As
				if (ImGui::MenuItem("Save As...##TileEditorMenuBar"))
				{
					std::string ret = Utility::WindowsFileDialog::SaveFile("ALEngine Map (*.map)\0*.map\0");

					// Save Success
					if (ret.empty() == false)
					{
						SaveMap(ret.c_str());
						m_HasMapLoaded = true;
						m_FilePath = ret;

						ImGui::EndMenu();
						ImGui::EndMenuBar();
						return;
					}
				}

				// Open File
				if (ImGui::MenuItem("Open File##TileEditorMenuBar"))
				{
					SaveMap(m_FilePath.c_str());

					ResetVariables();

					std::string ret = Utility::WindowsFileDialog::LoadFile("ALEngine Map (*.map)\0*.map\0");

					// Save Success
					if (ret.empty() == false)
					{
						LoadMap(ret.c_str());
						m_HasMapLoaded = true;
						m_FilePath = ret;

						ImGui::EndMenu();
						ImGui::EndMenuBar();
						return;
					}

				}
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Edit##TileEditorMenuBar"))
			{
				// Change Size
				if (ImGui::MenuItem("Open File##TileEditorMenuBar"))
				{

				}

				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}
	}

	void TileEditorPanel::CreateNewMap(void)
	{
		f32 winLen{ ImGui::GetContentRegionAvail().x };
		// Back button
		if (ImGui::Button("Back##TileEditor_CreateTileMap"))
		{
			m_CurrentLoadStage = LoadStage::CreateOrLoadSelection;
			return;
		}

		ImGui::NewLine();
		ImGui::PushItemWidth(winLen * 0.5f);

		// Map Width Text
		f32 textLen = ImGui::CalcTextSize("Map Width").x;
		ImGui::SameLine((winLen - textLen) * 0.5f);
		ImGui::Text("Map Width");

		// Map Width Drag Int
		ImGui::NewLine();  ImGui::SameLine(winLen * 0.25f);
		ImGui::DragInt("##TileEditorWidth", &m_MapWidth, 1.f, 0, 64);

		// Map Height Text
		textLen = ImGui::CalcTextSize("Map Width").x;
		ImGui::NewLine();  ImGui::SameLine((winLen - textLen) * 0.5f);
		ImGui::Text("Map Height");

		// Map Height Drag Int
		ImGui::NewLine();  ImGui::SameLine(winLen * 0.25f);
		ImGui::DragInt("##TileEditorHeight", &m_MapHeight, 1.f, 0, 64);

		ImGui::PopItemWidth();

		if (m_MapWidth == 0 || m_MapHeight == 0)
		{
			ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
			ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
		}

		ImGui::NewLine(); ImGui::NewLine(); ImGui::SameLine((winLen - ImGui::CalcTextSize("Save").x) * 0.5f);
		if (ImGui::Button("Save"))
		{
			// Get Tile Map
			m_TileMap.clear();
			for (s32 i{ 0 }; i < m_MapHeight; ++i)
			{
				std::vector<std::string> row{};
				for (s32 j{ 0 }; j < m_MapWidth; ++j)
				{
					// Push the first in the map, which is normally the "Empty" tile
					row.push_back(m_ImageMap.begin()->first);
				}
				m_TileMap.push_back(row);
			}

			std::string ret = Utility::WindowsFileDialog::SaveFile("ALEngine Map (*.map)\0*.map\0");

			// Save Success
			if (ret.empty() == false)
			{
				SaveMap(ret.c_str());
				m_HasMapLoaded = true;
				m_FilePath = ret;
				return;
			}
		}

		if (m_MapWidth == 0 || m_MapHeight == 0)
		{
			ImGui::PopItemFlag();
			ImGui::PopStyleVar();
		}
	}

	void TileEditorPanel::SelectMap(void)
	{
		if (m_CurrentLoadStage == LoadStage::CreateOrLoadSelection)
		{
			ImGuiStyle& style = ImGui::GetStyle();

			f32 btn_width = ImGui::GetContentRegionAvail().x * 0.6f;
			f32 btn_height = ImGui::GetContentRegionAvail().y * 0.2f;
			f32 btn_size = btn_width + style.FramePadding.x * 2.f;
			f32 align = (ImGui::GetWindowSize().x - btn_size) * 0.5f;

			ImGui::NewLine(); ImGui::NewLine(); ImGui::SameLine(align);
			if (ImGui::Button("Create New Map##TileEditor", ImVec2(btn_width, btn_height)))
			{
				m_CurrentLoadStage = LoadStage::CreateMap;

				// Reset to Zero
				m_MapHeight = 0;
				m_MapWidth = 0;
				for (auto& i : m_TileMap)
					i.clear();
				m_TileMap.clear();
				return;
			}

			ImGui::NewLine(); ImGui::NewLine(); ImGui::SameLine(align);
			if (ImGui::Button("Load Map##TileEditor", ImVec2(btn_width, btn_height)))
			{
				m_CurrentLoadStage = LoadStage::LoadMap;

				// Reset to Zero
				m_MapHeight = 0;
				m_MapWidth = 0;
				for (auto& i : m_TileMap)
					i.clear();
				m_TileMap.clear();
				return;
			}
		}
		else if (m_CurrentLoadStage == LoadStage::LoadMap)
		{
			std::string ret = Utility::WindowsFileDialog::LoadFile("ALEngine Map (*.map)\0*.map\0");

			if (!ret.empty())
			{
				LoadMap(ret.c_str());
				m_HasMapLoaded = true;
				m_FilePath = ret;
				m_CurrentLoadStage = LoadStage::CreateOrLoadSelection;
				return;
			}
			m_CurrentLoadStage = LoadStage::CreateOrLoadSelection;
		}
		else if (m_CurrentLoadStage == LoadStage::CreateMap)
		{
			CreateNewMap();
		}
	}

	void TileEditorPanel::SaveMap(const c8* filePath)
	{
		using namespace rapidjson;
		std::string filePath_str{ filePath };
		if (filePath_str.find(".map") == std::string::npos)
			filePath_str += ".map";

		StringBuffer strbuf{};
		PrettyWriter writer(strbuf);

		writer.StartArray();
		{
			writer.StartObject();

			// Dimensions
			writer.Key("Width");
			writer.Int(m_MapWidth);
			writer.Key("Height");
			writer.Int(m_MapHeight);

			writer.Key("Map");
			writer.StartArray();
			{
				u32 rowCount{ 0 };
				for (const auto& i : m_TileMap)
				{
					std::string row_key = std::to_string(rowCount++);
					writer.StartArray();
					for (const auto& j : i)
						writer.String(j.c_str());
					writer.EndArray();
				}
			}
			writer.EndArray();

			writer.EndObject();
		}
		writer.EndArray();

		std::ofstream ofs{ filePath_str };

		if (!ofs)
		{
			AL_CORE_WARN("Could not open file: {}", filePath_str);
			return;
		}

		ofs.write(strbuf.GetString(), strbuf.GetLength());

		ofs.close();
	}

	void TileEditorPanel::LoadMap(const c8* filePath)
	{
		using namespace rapidjson;
		std::string filePath_str{ filePath };

		c8* buffer = Utility::ReadBytes(filePath);

		if (!buffer)
		{
			AL_CORE_CRITICAL("Error: Unable to load map: {}", filePath);
			return;
		}

		Document doc;
		doc.Parse(buffer);

		Memory::DynamicMemory::Delete(buffer);

		Value const& val{ *doc.Begin() };

		// Get Width
		if (val.HasMember("Width"))
		{
			m_MapWidth = val["Width"].GetInt();
			AL_CORE_INFO("Width Assigned to {}", m_MapWidth);
		}

		// Get Height
		if (val.HasMember("Height"))
		{
			m_MapHeight = val["Height"].GetInt();
			AL_CORE_INFO("Height Assigned to {}", m_MapHeight);
		}

		// Make sure width or height are not 0
		assert((m_MapWidth != 0) || (m_MapHeight != 0));

		// Make sure has TileMap
		assert(val.HasMember("Map"));

		for (s32 i{ 0 }; i < m_MapHeight; ++i)
		{
			Value const& row_val = val["Map"][i];
			std::vector<std::string> row_tiles{};

			for (s32 j{ 0 }; j < m_MapWidth; ++j)
				row_tiles.emplace_back(row_val[j].GetString());

			m_TileMap.emplace_back(row_tiles);
		}

	}

	void TileEditorPanel::SaveTileEditorData(void)
	{
		using namespace rapidjson;
		std::string filePath{ TILE_EDITOR_DATA_PATH };

		StringBuffer strbuf{};
		PrettyWriter writer(strbuf);

		writer.StartArray();
		{
			writer.StartObject();

			for (auto& i : m_ImageMap)
			{
				writer.Key(i.first.c_str());
				writer.String(i.second.c_str());
			}
			
			writer.EndObject();
		}
		writer.EndArray();

		std::ofstream ofs{ filePath };

		if (!ofs)
		{
			AL_CORE_WARN("Could not open file: {}", filePath);
			return;
		}

		ofs.write(strbuf.GetString(), strbuf.GetLength());

		ofs.close();
	}
	
	void TileEditorPanel::ResetVariables(void)
	{
		if (!m_TileMap.empty())
		{
			for (auto& i : m_TileMap)
				i.clear();
			m_TileMap.clear();
		}

		m_MapWidth = m_MapHeight = 0;

		m_CurrentLoadStage = LoadStage::CreateOrLoadSelection;

		m_HasMapLoaded = false;

		m_FilePath = "";
	}
}
#endif