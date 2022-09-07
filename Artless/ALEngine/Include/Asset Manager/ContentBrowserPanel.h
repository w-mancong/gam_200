#pragma once
#include <filesystem>

/*!*****************************************************************************
 \file ContentBrowserPanel.h
 \author Chan Jie Ming Stanley
 \par DP email: c.jiemingstanley\@digipen.edu
 \par Course: CSD2400
 \par Gam200
 \date 3-9-2022
 \brief

*******************************************************************************/

#ifndef	CONTENT_BROWSER_PANEL_H
#define CONTENT_BROWSER_PANEL_H

namespace ALEngine
{
	namespace CBP
	{
		class ContentBrowserPanel
		{
		public:
			ContentBrowserPanel();

			~ContentBrowserPanel();

			void OnImGuiRender();
		private:
			std::filesystem::path currentdirectory;// for keeping track of current file directory or path
		};
	}


}





#endif
