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
			static f32 gizmoLineWidith; // thickness of line
			static Graphics::Shader gizmolineShader;
			static bool gizmoToggle;
			static Math::Vector3 gizmoColor;
			//static u32 gizmoCircleSegments;

			static void GizmoInit();
			static void RenderLine(Math::Vector2 pt1, Math::Vector2 pt2);
			static void RenderAllLines();
			//static void RenderCircle(Math::Vector2 center, f32 radius);
			static void SetGizmoLineWidth(f32 width) { gizmoLineWidith = width; }
			static void SetGizmoColor(Math::Vector3 color) { gizmoColor = color; }
		};
	}
}

#endif
