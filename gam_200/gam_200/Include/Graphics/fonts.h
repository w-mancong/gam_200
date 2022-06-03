#include "pch.h"

#ifndef	FONTS_H
#define FONTS_H

namespace ManCong
{
	namespace Graphics
	{
		struct Character {
			u32 textureID{}; // ID handle of the glyph texture
			Math::Vector2 size;      // Size of glyph
			Math::Vector2 bearing;   // Offset from baseline to left/top of glyph
			u32 advance{};   // Horizontal offset to advance to next glyph
		};
	}
}

#endif
