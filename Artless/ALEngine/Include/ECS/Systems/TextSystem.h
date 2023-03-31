/*!
file:	TextSystem.h
author:	Mohamed Zafir
email:	m.zafir\@digipen.edu
brief:	This file contains the function declarations for TextSystem.h

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/

#ifndef	TEXTSYSTEM_H
#define TEXTSYSTEM_H

namespace ALEngine::ECS
{
	class TextSystem : public System
	{
	public:
		/*!*********************************************************************************
			\brief
			Updates Text System Components
		***********************************************************************************/
		void Update(void);
	};

	/*!*********************************************************************************
		\brief
		Initializes the Text System
	***********************************************************************************/
	void RegisterTextSystem(void);

	/*!*********************************************************************************
		\brief
		Updates the Text System (calls TextSystem->Update)
	***********************************************************************************/
	void UpdateTextSystem(void);

	/*!*********************************************************************************
		\brief
		struct to encapsulate font character data
    ***********************************************************************************/
	struct Character
	{
		u32 textureID{}; // ID handle of the glyph texture
		Math::Vector2 size;      // Size of glyph
		Math::Vector2 bearing;   // Offset from baseline to left/top of glyph
		u32 advance{};   // Horizontal offset to advance to next glyph
	};

	class Font
	{
	public:
		std::map<GLchar, Character> characterCollection;
		u32 fontsVAO{}, fontsVBO{};
		ALEngine::Graphics::Shader fontShader;
		std::string fontName;
		static std::vector<Text> textCollection;
		static b8 enableTextRendering;

		/*!*********************************************************************************
			\brief
				Initializes and processes font file (.ttf)
			\param [in] fontAddress:
				File path of font file.
			\param [in] fontName:
				Name of font.
		***********************************************************************************/
		static Font FontInit(std::string fontAddress, std::string fontName);

		/*!*********************************************************************************
			\brief
				Stores text to render into a container to render later.
			\param [in] text:
				Text to render.
		***********************************************************************************/
		static void RenderText(Text& text);

		/*!*********************************************************************************
			\brief
				Renders all instances of Text.
			\param [in] camera:
				Camera to render to.
		***********************************************************************************/
		static void RenderAllText(Engine::Camera const& camera);

		/*!*********************************************************************************
			\brief
				Enable or disable text rendering
			\param [in] bool:
				True or false.
		***********************************************************************************/
		static void EnableTextRendering(b8);
	};
}
#endif