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

		/*!*********************************************************************************
		\brief
		Set the panel's minimum size
		\param min
		Panel's min size
		***********************************************************************************/
		void SetPanelMin(Math::Vec2 min);

		/*!*********************************************************************************
		\brief
		Set the panel's default pos and size
		\param pos
		Panel's Pos
		\param size
		Panel's Size
		***********************************************************************************/
		void SetDefault(Math::Vec2 pos, Math::Vec2 size);

		/*!*********************************************************************************
		\brief
		Sets the panel to default position and size
		***********************************************************************************/
		void Default(void);

	private:
		// Panel sizes
		ImVec2 m_PanelMin{};	// Min Size

		// Panel Defaults
		ImVec2 m_DefaultPos{};	// Default Position
		ImVec2 m_DefaultSize{};	// Default Size

		c8 searchKeyword[256];	//for search bar

		std::filesystem::path m_CurrentDirectory;// for keeping track of current file directory or path
		std::filesystem::path m_MainDirectory;
	};
}
#endif
