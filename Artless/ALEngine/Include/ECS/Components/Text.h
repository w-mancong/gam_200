#ifndef TEXT_H
#define TEXT_H

namespace ALEngine
{
	namespace ECS
	{
		namespace Component
		{
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
				enum class FontType
				{
					Regular,
					Bold,
					Italic,
					ItalicBold
				};

				std::map<GLchar, Character> characterCollection;
				u32 fontsVAO{}, fontsVBO{};
				ALEngine::Graphics::Shader fontShader;
				static std::map<std::string, std::map<Font::FontType, Font>> fontCollection;
				static void FontInit(std::string fontAddress, std::string fontName, Font::FontType fontType);
			};

			struct Text
			{
				std::string currentFont{}; // current selected font
				Font::FontType currentType{}; // current selected font type
				Math::Vector2 position{}; // position of text
				Math::Vector3 colour{}; // color of text
				f32 scale{ 1.f }; // scale of text
				std::string textString{}; // text string
			};

			void RenderText(Text& text);
			void SetFont(Text& text, std::string fontName);
			void SetFontType(Text& text, Font::FontType typeName);
			void SetTextPos(Text& text, Math::Vector2 pos);
			void SetTextPos(Text& text, f32 x, f32 y);
			void SetTextColor(Text& text, Math::Vector3 col);
			void SetTextColor(Text& text, f32 r, f32 g, f32 b);
			void SetTextSize(Text& text, f32 size);
			void SetTextString(Text& text, std::string texts);
		}
	}
}

#endif
