#ifndef	GIZMO_H
#define GIZMO_H

namespace ALEngine
{
	namespace Gizmos
	{
		class Gizmo
		{
		public:
		
			f32 lineWidith;
			f32 lineScale;
			Math::Vector2 lineMidPoint;
			f32 lineRotate;
			static Graphics::Shader lineShader;
			static u32 vaoID, vboID;

			static void GizmoInit();
			static void RenderLine();
			//static void DrawLineBox(Math::Vector2 pt0, Math::Vector2 pt1, Math::Vector2 pt2, Math::Vector2 pt3);
			//static SetLineWidth(f32 width) { lineWidith = width; }
		};
	}
}

#endif
