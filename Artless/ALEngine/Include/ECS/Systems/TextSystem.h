#ifndef	TEXTSYSTEM_H
#define TEXTSYSTEM_H

namespace ALEngine::ECS
{
	class TextSystem : public System
	{
	public:
		void Update(void);
	};

	void RegisterTextSystem(void);
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
		/*!*********************************************************************************
			\brief
			Enum class used for differentiating the different type of fonts
		***********************************************************************************/

		std::map<GLchar, Character> characterCollection;
		u32 fontsVAO{}, fontsVBO{};
		ALEngine::Graphics::Shader fontShader;
		static std::map<std::string, std::map<Text::FontType, Font>> fontCollection;
		static std::vector<Text> textCollection;

		/*!*********************************************************************************
			\brief
				Initializes and processes font file (.ttf)
			\param [in] fontAddress:
				File path of font file.
			\param [in] fontName:
				Name of font.
			\param [in] fontType:
				Type of font.
		***********************************************************************************/
		static void FontInit(std::string fontAddress, std::string fontName, Text::FontType fontType);
		static void RenderText(Text& text);
		static void RenderAllText();
	};
}
#endif