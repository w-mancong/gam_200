#include "pch.h"

#ifndef	FONTS_H
#define FONTS_H

namespace ManCong
{
	namespace Graphics
	{
		struct Character {
			u32 TextureID; // ID handle of the glyph texture
			Math::Vector2 Size;      // Size of glyph
			Math::Vector2 Bearing;   // Offset from baseline to left/top of glyph
			u32 Advance;   // Horizontal offset to advance to next glyph
		};
	}
}

#endif
