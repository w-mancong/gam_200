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

				static std::map<std::string, Font> fontCollection;
				static std::string currentFont;
				static Shader fontShader;

				static void RenderText(std::string text, f32 x, f32 y, f32 scale, Math::Vector3 color);
				static void setShader(Shader shader) { fontShader = shader; }
				static void setFont(std::string fontName) { currentFont = fontName; }

			};
		}
}

#endif
