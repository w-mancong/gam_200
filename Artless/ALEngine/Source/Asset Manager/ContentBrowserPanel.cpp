#include "pch.h"


/*!*****************************************************************************
 \file ContentBrowserPanel.cpp
 \author Chan Jie Ming Stanley
 \par DP email: c.jiemingstanley\@digipen.edu
 \par Course: CSD2400
 \par Gam200
 \date 3-9-2022
 \brief
  This file contains

*******************************************************************************/

namespace ALEngine
{


	namespace CBP
	{
		//change this later for projects
		extern const std::filesystem::path assetpath = "assets";//base file path

		ContentBrowserPanel::ContentBrowserPanel()
			:currentdirectory(assetpath)
		{}

		ContentBrowserPanel::~ContentBrowserPanel()
		{
			
		}

		void ContentBrowserPanel::OnImGuiRender()
		{
			//imgui window-------------------------------------------------------------------------
			ImGui::Begin("Content Browser");

			if (currentdirectory != std::filesystem::path(assetpath))
			{
				//render makeshift back button
				if (ImGui::Button("<-"))
				{
					currentdirectory = currentdirectory.parent_path();
				}
			}

			//loop through directory and create buttons for each file
			for (auto& directoryentry : std::filesystem::directory_iterator(currentdirectory))
			{
				//file default path
				const auto& path = directoryentry.path();

				//file relative path
				std::filesystem::path relativepath = std::filesystem::relative(directoryentry.path(), assetpath);

				//file name from relative path 
				std::string filenamestring = relativepath.filename().string();

				//ImTextureID icon = directoryentry.is_directory()?  ;

			   // ImGui::ImageButton(ImTextureID)

				if (directoryentry.is_directory())
				{
					//buttons that show the files
					//if (ImGui::Button(filenamestring.c_str()))
					//{
					//}

					//selectable to show file
					if (ImGui::Selectable(filenamestring.c_str()))
					{
						currentdirectory /= directoryentry.path().filename();
					}

				}
				else
				{
					//static char buf[128] = ;


					//ImGui::InputText("1", buf, IM_ARRAYSIZE(filenamestring.c_str()));
					//if (ImGui::Selectable(filenamestring.c_str()))
					//{

						//ImGui::Button(filenamestring.c_str());
						
					

						//std::filesystem::rename();
						
					//}
				}
	
				//ImGui::TextWrapped(filenamestring.c_str());
				//ImGui::NextColumn();

			}

			ImGui::End();
			//------------------------------------------------------------------------------------
		}
	}
	
}


	
