/*!
file: AudioEditorPanel.h
author: Chan Jie Ming Stanley
email: c.jiemingstanley\@digipen.edu
brief: This file contains the function declaration for AudioEditorPanel.
	   AudioEditorPanel handles the panel that display the  audio editor
All content :copyright: 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#ifndef	ANIMATOR_EDITOR_PANEL_H
#define ANIMATOR_EDITOR_PANEL_H

namespace ALEngine::Editor
{
	class AnimatorEditorPanel
	{
	public:
		AnimatorEditorPanel();
		~AnimatorEditorPanel();

		/*!*********************************************************************************
		\brief
		renders out the panel displaying the audio editor panel
		***********************************************************************************/
		void OnImGuiRender(void);


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
		void SetDefaults(Math::Vec2 pos, Math::Vec2 size);

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

	    static std::string FileContents(const std::filesystem::path& path);

	};
}
#endif
