#ifndef	GIZMO_H
#define GIZMO_H

namespace ALEngine
{
	namespace Gizmos
	{
		class Gizmo
		{
		private:
			static u32 GizmoVaoId, GizmoVboId;

		public:
			static std::vector<std::pair<Math::Vector2, Math::Vector2>> linesContainer;
			static f32 GizmoLineWidith; // thickness of line
			static Graphics::Shader GizmolineShader;

			static void GizmoInit();
			static void RenderLine(Math::Vector2 pt1, Math::Vector2 pt2);
			static void RenderAllLines();
			static void SetGizmoLineWidth(f32 width) { GizmoLineWidith = width; }
		};
	}
}

#endif
