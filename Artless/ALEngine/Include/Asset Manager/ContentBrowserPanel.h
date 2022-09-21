#pragma once
#include <filesystem>

/*!
file: ContentBrowserPanel.h
author: Chan Jie Ming Stanley
email: c.jiemingstanley\@digipen.edu
brief:
This file contains

All content :copyright: 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/


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
