#include "pch.h"
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
	extern const std::filesystem::path assetPath = "assets";//base file path

	ContentBrowserPanel::ContentBrowserPanel()
	:m_CurrentDirectory(assetPath)
	{}

	ContentBrowserPanel::~ContentBrowserPanel()
	{
		
	}

	void ContentBrowserPanel::OnImGuiRender(void)
	{
		//imgui window-------------------------------------------------------------------------
		ImGui::Begin("Content Browser");

		if (m_CurrentDirectory != std::filesystem::path(assetPath))
		{
			//render makeshift back button
			if (ImGui::Button("<- Back"))
			{
				m_CurrentDirectory = m_CurrentDirectory.parent_path();
			}
		}

		//loop through directory and create buttons for each file
		for (auto& directoryEntry : std::filesystem::directory_iterator(m_CurrentDirectory))
		{
			//file default path
			const auto& path = directoryEntry.path();

			//file relative path
			std::filesystem::path relativepath = std::filesystem::relative(directoryEntry.path(), assetPath);

			//file name from relative path 
			std::string fileNamestring = relativepath.filename().string();

			//for dragging file, need to fix window crash when moving window
			//if (ImGui::BeginDragDropSource())
			//{
			//	auto relativePath = std::filesystem::relative(path, assetPath);
			//	const wchar_t* itemPath = relativePath.c_str();
			//	ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", itemPath, (wcslen(itemPath) + 1) * sizeof(wchar_t));
			//	ImGui::EndDragDropSource();
			//}

			if (ImGui::Selectable(fileNamestring.c_str()) && ImGui::IsItemHovered())
			{
				if (directoryEntry.is_directory())
				{
					//selectable to show file
					//if (ImGui::Selectable(fileNamestring.c_str()))
					{
						m_CurrentDirectory /= path.filename();
					}
				}
			}
		}

		ImGui::End();
		//------------------------------------------------------------------------------------
	}	
}	
