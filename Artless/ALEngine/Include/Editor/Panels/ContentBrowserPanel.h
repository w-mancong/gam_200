/*!
file: ContentBrowserPanel.h
author: Chan Jie Ming Stanley
email: c.jiemingstanley\@digipen.edu
brief: This file contains the function declaration for ContentBrowserPanel. 
       ContentBrowserPanel handles the panel that display the loaded assets in specified
	   asset filepath.
All content :copyright: 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/

#ifndef	CONTENT_BROWSER_PANEL_H
#define CONTENT_BROWSER_PANEL_H

namespace ALEngine::Editor
{
	class ContentBrowserPanel
	{
	public:
		ContentBrowserPanel();
		~ContentBrowserPanel();

		/*!*********************************************************************************
		\brief
		renders out the panel displaying the loaded assets in specified
		asset filepath and update the panel based on panel buttons.
		***********************************************************************************/
		void OnImGuiRender(void);

		//need to create a setter function
		//void SetAssetDirectory();

	private:
		c8 searchKeyword[256]; //for search bar

		std::filesystem::path m_CurrentDirectory;// for keeping track of current file directory or path
		std::filesystem::path m_MainDirectory;
	};
}
#endif
