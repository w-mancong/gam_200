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
	searchKeyword("")
	{}

	ContentBrowserPanel::~ContentBrowserPanel()
	{
		
	}

	void ContentBrowserPanel::OnImGuiRender(void)
	{
		//imgui window-------------------------------------------------------------------------
		ImGui::Begin("Content Browser");

		//loop through directory and create buttons for each file
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

			ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_Leaf;
			if (ImGui::TreeNodeEx(fileNamestring.c_str(), flags))
			{
				//for dragging file, need to fix window crash when moving window
				if (ImGui::BeginDragDropSource())
				{
					const wchar_t* itemPath = relativePath.c_str();
					ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", itemPath, (wcslen(itemPath) + 1) * sizeof(wchar_t));
					ImGui::EndDragDropSource();
				}

				if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left) && ImGui::IsItemHovered())
				{
					if (directoryEntry.is_directory() && (m_MainDirectory == m_CurrentDirectory))
					{
						//selectable to show file
						m_CurrentDirectory /= path.filename();
					}
				}

				ImGui::TreePop();
			}

		
			//set next column
			ImGui::NextColumn();

			//pop files ID
			ImGui::PopID();
		}
		ImGui::End();
		//------------------------------------------------------------------------------------

		//imgui window 2----------------------------------------------------------------------
		ImGui::Begin("Assets");

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
		ImGui::InputText("Search Tag", searchKeyword, 256);

		std::string searchString(searchKeyword);
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

		//loop through directory and create buttons for each file
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

			//need change to imagebuttons for icons

			if (fileNamestring.find(".jpg")!= std::string::npos || fileNamestring.find(".png") != std::string::npos)
			{
				Guid id = Engine::AssetManager::Instance()->GetGuid(directoryEntry.path().string());
				u64 texture = static_cast<u64>(Engine::AssetManager::Instance()->GetButtonImage(id));
				ImGui::ImageButton(reinterpret_cast<ImTextureID>(texture), { thumbnailSize, thumbnailSize }, { 0, 1 }, { 1, 0 });
			}
			else
			{
				ImGui::Button(fileNamestring.c_str(), { thumbnailSize, thumbnailSize });
			}
			
			//for dragging file, need to fix window crash when moving window
			if (ImGui::BeginDragDropSource())
			{
				const wchar_t* itemPath = path.c_str();
				ImGui::SetDragDropPayload("ASSET_ITEM", itemPath, (wcslen(itemPath) + 1) * sizeof(wchar_t));
				ImGui::EndDragDropSource();
			}

			if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left) && ImGui::IsItemHovered())
			{
				if (directoryEntry.is_directory())
				{
					//selectable to show file
					m_CurrentDirectory /= path.filename();
				}
			}
			//file name under button of file
			ImGui::TextWrapped(fileNamestring.c_str());

			//set next column
			ImGui::NextColumn();

			//pop files ID
			ImGui::PopID();
		}

		ImGui::Columns(1);

		//ImGui::SliderFloat("Thumbnail Size", &thumbnailSize, 16, 512);
		//ImGui::SliderFloat("Padding", &padding, 0, 32);

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