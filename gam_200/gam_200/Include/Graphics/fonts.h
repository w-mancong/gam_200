#include "pch.h"

#ifndef	FONTS_H
#define FONTS_H

namespace ManCong
{
	namespace Graphics
	{
		struct Character {
			unsigned int TextureID; // ID handle of the glyph texture
			Math::Vector2 Size;      // Size of glyph
			Math::Vector2 Bearing;   // Offset from baseline to left/top of glyph
			unsigned int Advance;   // Horizontal offset to advance to next glyph
		};
	}
}

#endif
