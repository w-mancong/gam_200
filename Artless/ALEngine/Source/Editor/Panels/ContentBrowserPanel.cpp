#include "pch.h"

/*!
file: ContentBrowserPanel.cpp
author: Chan Jie Ming Stanley
email: c.jiemingstanley\@digipen.edu
brief:
This file contains

All content :copyright: 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
namespace ALEngine
{
	namespace Editor
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
				if (ImGui::Button("<-"))
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
				std::filesystem::path relativepath = std::filesystem::relative(directoryEntry.path(), assetpath);

				//file name from relative path 
				std::string filenamestring = relativepath.filename().string();

				//ImTextureID icon = directoryentry.is_directory()?  ;

			   // ImGui::ImageButton(ImTextureID)

				if (directoryEntry.is_directory())
				{
					//buttons that show the files
					//if (ImGui::Button(filenamestring.c_str()))
					//{
					//}

					//selectable to show file
					if (ImGui::Selectable(filenamestring.c_str()))
					{
						m_CurrentDirectory /= directoryEntry.path().filename();
					}

				}
				else
				{
					//static char buf[128] = ;

					//ImGui::InputText("1", buf, IM_ARRAYSIZE(filenamestring.c_str()));
					if (ImGui::Selectable(filenamestring.c_str()))
					{
						ImGui::Button(filenamestring.c_str());
		
						//std::filesystem::rename();
					}
				}
	
				//ImGui::TextWrapped(filenamestring.c_str());
				//ImGui::NextColumn();

			}

			ImGui::End();
			//------------------------------------------------------------------------------------
		}
	}
	
}


	
