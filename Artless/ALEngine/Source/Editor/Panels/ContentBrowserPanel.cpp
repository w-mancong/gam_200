#include "pch.h"

#if EDITOR

#include "imgui.h"
#include "imgui_internal.h"
/*!
file: ContentBrowserPanel.cpp
author: Chan Jie Ming Stanley
email: c.jiemingstanley\@digipen.edu
brief: This file contains the function declaration for ContentBrowserPanel.
	   ContentBrowserPanel handles the panel that display the loaded assets in specified
	   asset filepath.
All content :copyright: 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
namespace ALEngine::Editor
{
	//change this later to read from settings or 
	const std::filesystem::path assetPath = "Assets";//base file path

	ContentBrowserPanel::ContentBrowserPanel()
	:m_CurrentDirectory(assetPath),
	m_MainDirectory(assetPath),
	subDirectory(assetPath),
	m_SearchKeyword(""),
	m_OptionPromptEnabled(false),
	m_RenameFile(false),
	m_NameOfRenameFile("")
	{}

	ContentBrowserPanel::~ContentBrowserPanel()
	{
		
	}

	void ContentBrowserPanel::OnImGuiRender(void)
	{
		/*
		//imgui window-------------------------------------------------------------------------
		ImGui::Begin("Content Browser");

		//m_Loop through directory and create buttons for each file
		for (auto& directoryEntry : std::filesystem::directory_iterator(m_MainDirectory))
		{
			//file default path
			const auto& path = directoryEntry.path();

			//file relative path
			std::filesystem::path const& relativePath = std::filesystem::relative(path, assetPath);

			//file name from relative path 
			std::string const& fileNamestring = relativePath.filename().string();

			if (fileNamestring == "Dev")
			{
				continue;
			}

			if (fileNamestring.find(".meta") != std::string::npos)
			{
				continue;
			}

			//push files ID 
			ImGui::PushID(fileNamestring.c_str());

			//selectable files
		   // ImGui::MenuItem(fileNamestring.c_str());

			if (directoryEntry.is_directory()) //if is folder then do treenodeEx
			{
				if (ImGui::TreeNodeEx(fileNamestring.c_str()))
				{
					//selectable to show file
					for (auto& subDirectoryEntry : std::filesystem::recursive_directory_iterator(directoryEntry))
					{
						const auto& subpath = subDirectoryEntry.path();

						//file relative path
						std::filesystem::path const& subRelativePath = std::filesystem::relative(subpath, assetPath);

						//file name from relative path 
						std::string const& subFileNamestring = subRelativePath.filename().string();

						if (subFileNamestring.find(".meta") != std::string::npos)
						{
							continue;
						}

						//selectable files
						ImGui::Selectable(subFileNamestring.c_str());

						//for dragging file, need to fix window crash when moving window
						if (ImGui::BeginDragDropSource())
						{
							const wchar_t* itemPath = subRelativePath.c_str();
							ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", itemPath, (wcslen(itemPath) + 1) * sizeof(wchar_t));
							ImGui::EndDragDropSource();
						}

					}

					ImGui::TreePop();
				}
			}
			else //if not folder then do selectable
			{
				ImGui::Selectable(fileNamestring.c_str());
				//for dragging file, need to fix window crash when moving window
				if (ImGui::BeginDragDropSource())
				{
					const wchar_t* itemPath = relativePath.c_str();
					ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", itemPath, (wcslen(itemPath) + 1) * sizeof(wchar_t));
					ImGui::EndDragDropSource();
				}
			}
		
			//set next column
			ImGui::NextColumn();

			//pop files ID
			ImGui::PopID();
		}
		ImGui::End();
		//------------------------------------------------------------------------------------
		*/
		//imgui window 2----------------------------------------------------------------------
		ImGui::Begin("Assets");
		ImGui::BeginChild("ContentBrowserPanel##PanelChild", ImGui::GetContentRegionAvail());

		//back button
		if (m_CurrentDirectory != std::filesystem::path(assetPath))
		{
			//render makeshift back button
			if (ImGui::Button("<-- Back to previous"))
			{
				m_CurrentDirectory = m_CurrentDirectory.parent_path();
			}
		}

		ImGui::Text("Search Bar");
		ImGui::InputText("Search Tag", m_SearchKeyword, 256);

		std::string searchString(m_SearchKeyword);
		std::transform(searchString.begin(), searchString.end(), searchString.begin(), ::tolower);

		std::string fileNameCheck{ "" };

		static float padding = 16.0f;
		static float thumbnailSize = 96.0f;
		float cellSize = thumbnailSize + padding;

		float panelWidth = ImGui::GetContentRegionAvail().x;
		int columnCount = (int)(panelWidth / cellSize);
		if (columnCount < 1)
		{
			columnCount = 1;
		}

		ImGui::Columns(columnCount, 0, false);

		//m_Loop through directory and create buttons for each file
		for (auto& directoryEntry : std::filesystem::directory_iterator(m_CurrentDirectory))
		{
			//file default path
			const auto& path = directoryEntry.path();

			//file relative path
			std::filesystem::path const& relativePath = std::filesystem::relative(path, assetPath);

			//file name from relative path 
			std::string const& fileNamestring = relativePath.filename().string();

			fileNameCheck = fileNamestring;
			std::transform(fileNameCheck.begin(), fileNameCheck.end(), fileNameCheck.begin(), ::tolower);

			if (!(fileNameCheck.find(searchString) != std::string::npos))
			{
				continue;
			}

			if (fileNamestring == "Dev")
			{
				continue;
			}

			if (fileNamestring.find(".meta") != std::string::npos)
			{
				continue;
			}

			//push files ID 
			ImGui::PushID(fileNamestring.c_str());

			// To display folder icon
			if (directoryEntry.is_directory())
			{
				Guid id = Engine::AssetManager::Instance()->GetGuid("Assets\\Dev\\Images\\Icon_Folder.png");
				u64 texture = static_cast<u64>(Engine::AssetManager::Instance()->GetButtonImage(id));
				ImGui::ImageButton(reinterpret_cast<ImTextureID>(texture), { thumbnailSize, thumbnailSize }, { 0, 1 }, { 1, 0 });
			}
			// To display prefab icon
			else if (fileNamestring.find(".prefab") != std::string::npos)
			{
				Guid id = Engine::AssetManager::Instance()->GetGuid("Assets\\Dev\\Images\\Icon_Prefab.png");
				u64 texture = static_cast<u64>(Engine::AssetManager::Instance()->GetButtonImage(id));
				ImGui::ImageButton(reinterpret_cast<ImTextureID>(texture), { thumbnailSize, thumbnailSize }, { 0, 1 }, { 1, 0 });
			}
			// To display scene icon
			else if (fileNamestring.find(".scene") != std::string::npos)
			{
				Guid id = Engine::AssetManager::Instance()->GetGuid("Assets\\Dev\\Images\\Icon_Scene.png");
				u64 texture = static_cast<u64>(Engine::AssetManager::Instance()->GetButtonImage(id));
				ImGui::ImageButton(reinterpret_cast<ImTextureID>(texture), { thumbnailSize, thumbnailSize }, { 0, 1 }, { 1, 0 });
			}
			else if (fileNamestring.find(".map") != std::string::npos)
			{
				Guid id = Engine::AssetManager::Instance()->GetGuid("Assets\\Dev\\Images\\Icon_TileEditor.png");
				u64 texture = static_cast<u64>(Engine::AssetManager::Instance()->GetButtonImage(id));
				ImGui::ImageButton(reinterpret_cast<ImTextureID>(texture), { thumbnailSize, thumbnailSize }, { 0, 1 }, { 1, 0 });
			}
			// To display script icon
			else if (fileNamestring.find(".cs") != std::string::npos)
			{
				Guid id = Engine::AssetManager::Instance()->GetGuid("Assets\\Dev\\Images\\Icon_Script.png");
				u64 texture = static_cast<u64>(Engine::AssetManager::Instance()->GetButtonImage(id));
				ImGui::ImageButton(reinterpret_cast<ImTextureID>(texture), { thumbnailSize, thumbnailSize }, { 0, 1 }, { 1, 0 });
			}
			// To display font icon
			else if (fileNamestring.find(".ttf") != std::string::npos)
			{
				Guid id = Engine::AssetManager::Instance()->GetGuid("Assets\\Dev\\Images\\Icon_Text.png");
				u64 texture = static_cast<u64>(Engine::AssetManager::Instance()->GetButtonImage(id));
				ImGui::ImageButton(reinterpret_cast<ImTextureID>(texture), { thumbnailSize, thumbnailSize }, { 0, 1 }, { 1, 0 });
			}
			// To display audio icon
			else if (fileNamestring.find(".wav") != std::string::npos)
			{
				Guid id = Engine::AssetManager::Instance()->GetGuid("Assets\\Dev\\Images\\Icon_Sound.png");
				u64 texture = static_cast<u64>(Engine::AssetManager::Instance()->GetButtonImage(id));
				ImGui::ImageButton(reinterpret_cast<ImTextureID>(texture), { thumbnailSize, thumbnailSize }, { 0, 1 }, { 1, 0 });
			}
			// To display asset icon
			else if (fileNamestring.find(".jpg")!= std::string::npos || fileNamestring.find(".png") != std::string::npos)
			{
				Guid id = Engine::AssetManager::Instance()->GetGuid(directoryEntry.path().string());
				u64 texture = static_cast<u64>(Engine::AssetManager::Instance()->GetButtonImage(id));
				ImGui::ImageButton(reinterpret_cast<ImTextureID>(texture), { thumbnailSize, thumbnailSize }, { 0, 1 }, { 1, 0 });
			}
			else
			{
				ImGui::Button(fileNamestring.c_str(), { thumbnailSize, thumbnailSize });
			}
			
			if (ImGui::BeginDragDropSource())
			{
				const wchar_t* itemPath = path.c_str();
				ImGui::SetDragDropPayload("ASSET_ITEM", itemPath, (wcslen(itemPath) + 1) * sizeof(wchar_t));
				ImGui::EndDragDropSource();
			}

			//to enter into folder directory
			if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left) && ImGui::IsItemHovered())
			{
				if (fileNamestring.find(".scene") != std::string::npos)
				{
					ALEditor::Instance()->SetSelectedEntity(ECS::MAX_ENTITIES);
					std::string the_path = directoryEntry.path().string();
					Engine::Scene::LoadScene(the_path.c_str());
					ALEditor::Instance()->SetCurrentSceneName(the_path);
					ALEditor::Instance()->LoadMap();
				}
				else if (fileNamestring.find(".map") != std::string::npos)
				{
					Gameplay::MapManager::Instance()->SetMapPath(directoryEntry.path().string());
					Gameplay::MapManager::Instance()->DeserializeMap(directoryEntry.path().string());
					ALEditor::Instance()->LoadMap();
				}
				else if (directoryEntry.is_directory())
				{
					//selectable to show file
					m_CurrentDirectory /= path.filename();
				}
			}

			//for setting selected item
			if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Right) && ImGui::IsItemHovered())
			{
				//set selected item to delete when press delete key 
				m_DirectoryToDelete = directoryEntry.path();
			}

			if (ImGui::IsKeyPressed(ImGuiKey_Delete))
			{
				std::filesystem::remove_all(m_DirectoryToDelete);
			}

			//open prompt for delete
			if (ImGui::IsMouseClicked(ImGuiMouseButton_Right)&&ImGui::IsItemHovered())
			{
				//set selected item to delete when press delete button in the prompt
				m_DirectoryToDelete = directoryEntry.path();
				ImGui::OpenPopup("##Option prompt");
				m_OptionPromptEnabled = true;
			}

			if (ImGui::BeginPopup("##Option prompt", m_OptionPromptEnabled))
			{
				if (ImGui::Button("Delete"))
				{
					//std::cout << m_DirectoryToDelete << std::endl;
					std::filesystem::remove_all(m_DirectoryToDelete);
					ImGui::CloseCurrentPopup();
				}
				//rename not implemented yet
				if (ImGui::Button("Rename"))
				{
					m_RenameFile = true;
					ImGui::CloseCurrentPopup();
				}
				ImGui::EndPopup();
			}

		    strcpy_s(m_NameOfRenameFile, fileNamestring.c_str());

			//file name under button of file
			if (m_RenameFile)
			{
				ImGui::InputText("##FileName", m_NameOfRenameFile, 256);

				if (ImGui::IsKeyPressed(ImGuiKey_Enter))
				{
					if (std::rename(fileNamestring.c_str(), m_NameOfRenameFile))
					{
						std::perror("Error renaming");
					}
					m_RenameFile = false;
				}
			}
			else
			{
				ImGui::TextWrapped(fileNamestring.c_str());
				
			}
			
			//set next column
			ImGui::NextColumn();

			//pop files ID
			ImGui::PopID();
		}

		ImGui::Columns(1);

		//ImGui::SliderFloat("Thumbnail Size", &thumbnailSize, 16, 512);
		//ImGui::SliderFloat("Padding", &padding, 0, 32);

		ImGui::EndChild();

		// Drag Drop!
		if (ImGui::BeginDragDropTarget())
		{
			// Payload flag
			ImGuiDragDropFlags payload_flag{ 0 };
			//payload_flag |= ImGuiDragDropFlags_AcceptNoDrawDefaultRect;

			// Get Drag and Drop Payload
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("HIERARCHY_ENTITY", payload_flag))
			{
				ECS::Entity en = *(ECS::Entity*)payload->Data;
				SavePrefab(en);
			}
			ImGui::EndDragDropTarget();
		}

		ImGui::End();
		//------------------------------------------------------------------------------------
	}
	
	void ContentBrowserPanel::SetPanelMin(Math::Vec2 min)
	{
		m_PanelMin = ImVec2(min.x, min.y);
	}

	void ContentBrowserPanel::SetDefaults(Math::Vec2 pos, Math::Vec2 size)
	{
		m_DefaultPos = ImVec2(pos.x, pos.y);
		m_DefaultSize = ImVec2(size.x, size.y);
	}

	void ContentBrowserPanel::Default(void)
	{
		
	}

}	

#endif