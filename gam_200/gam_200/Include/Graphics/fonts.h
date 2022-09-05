#include "pch.h"

#ifndef	FONTS_H
#define FONTS_H

namespace ManCong
{
	namespace Graphics
	{
		struct Character
		{
			u32 textureID{}; // ID handle of the glyph texture
			Math::Vector2 size;      // Size of glyph
			Math::Vector2 bearing;   // Offset from baseline to left/top of glyph
			u32 advance{};   // Horizontal offset to advance to next glyph
		};

		struct Font
		{
			enum class FontType
			{
				Regular,
				Bold,
				Italic,
				ItalicBold
			};

			std::map<GLchar, Graphics::Character> characterCollection;
			u32 fontsVAO{}, fontsVBO{};

			static std::map<std::string, std::map<Font::FontType, Font>> fontCollection;
			static Shader fontShader;
			
			static void SetShader(Shader shader) { fontShader = shader; }
			
		};

		struct Text
		{
			static std::vector<Text> textCollection;
			std::string currentFont; // current selected font
			Font::FontType currentType; // current selected font type
			Math::Vector2 position; // position of text
			Math::Vector3 colour; // color of text
			f32 scale; // scale of text
			std::string text; // text string

			void RenderText();
			void SetFont(std::string fontName) { currentFont = fontName; }
			void SetFontType(Font::FontType typeName) { currentType = typeName; }
			void SetPos(Math::Vector2 pos) { position = pos; }
			void SetPos(f32 x, f32 y) { position = Math::Vector2(x, y); }
			void SetCol(Math::Vector3 col) { colour = col; }
			void SetCol(f32 r, f32 g, f32 b) { colour = Math::Vector3(r, g, b); }
			void SetScale(f32 size) { scale = size; }
			void SetText(std::string texts) { text = texts; }
		};
	}
}

#endif
