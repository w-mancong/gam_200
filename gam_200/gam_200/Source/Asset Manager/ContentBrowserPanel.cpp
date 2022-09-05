#include "pch.h"


/*!*****************************************************************************
 \file ContentBrowserPanel.cpp
 \author Chan Jie Ming Stanley
 \par DP email: c.jiemingstanley\@digipen.edu
 \par Course: CSD2400
 \par Gam200
 \date 3-9-2022
 \brief

*******************************************************************************/

namespace stanley {

	//change this later for projects
	static const std::filesystem::path assetpath = "assets";

	ContentBrowserPanel::ContentBrowserPanel()
		:currentdirectory(assetpath)
	{}

	void ContentBrowserPanel::OnImGuiRender()
	{
		//imgui window
		//ImGui::Begin("Content Browser");

		if (currentdirectory != std::filesystem::path(assetpath))
		{
			//if (ImGui::Button("<-"))
			//{
			     currentdirectory = currentdirectory.parent_path();
			//}

		}

		for (auto& directoryentry : std::filesystem::directory_iterator(currentdirectory))
		{
			const auto& path = directoryentry.path();

			auto relativepath = std::filesystem::relative(directoryentry.path(), assetpath);

			std::string filenamestring = relativepath.filename().string();

			if (directoryentry.is_directory())
			{
				//if (ImGui::Button(filenamestring.c_str()))
				//{
				     currentdirectory /= directoryentry.path().filename();
				//}
			}
			else
			{
				//if (ImGui::Button(filenamestring.c_str()))
				//{
			         
				//}

			}

			
		}

		//ImGui::End();
	}

}
