#ifndef	GIZMO_H
#define GIZMO_H

namespace ALEngine
{
	namespace  Gizmo
	{
		class Gizmo
		{
		public:
			static f32 lineWidith;

			static void RenderLine(Math::Vector2 pt0, Math::Vector2 pt1);
			static void DrawLineBox(Math::Vector2 pt0, Math::Vector2 pt1, Math::Vector2 pt2, Math::Vector2 pt3);
			static void SetLineWidth(f32 width) { lineWidith = width; }
		};
	}
}

#endif
