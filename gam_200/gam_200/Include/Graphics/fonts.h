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
			std::map<GLchar, Graphics::Character> characterCollection;
			u32 fontsVAO{}, fontsVBO{};

			//static std::map<std::string, Font> font2;
			static std::map<std::string, std::map<std::string, Font>> fontCollection;
			static std::string currentFont;
			static std::string currentType;
			static Shader fontShader;
			static Math::Vector2 position;
			static Math::Vector3 colour;
			static f32 scale;
			static std::string text;

			static void RenderText();
			static void SetShader(Shader shader) { fontShader = shader; }
			static void SetFont(std::string fontName) { currentFont = fontName; }
			static void SetFontType(std::string typeName) { currentType = typeName; }
			static void SetPos(Math::Vector2 pos) { position = pos; }
			static void SetPos(f32 x, f32 y) { position = Math::Vector2(x, y); }
			static void SetCol(Math::Vector3 col) { colour = col; }
			static void SetCol(f32 r, f32 g, f32 b) { colour = Math::Vector3(r, g, b); }
			static void SetScale(f32 size) { scale = size; }
			static void SetText(std::string texts) { text = texts; }
		};
	}
}

#endif
