/*!
file:	Gizmo.h
author:	Mohamed Zafir
email:	m.zafir@digipen.edu
brief:	This file contains the Gizmo class declaration. Gizmo is a debugging feature
		for the engine.

		All content � 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/

#ifndef	GIZMO_H
#define GIZMO_H

namespace ALEngine::Gizmo
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

		/*!*********************************************************************************
			\brief
				Initializes Gizmo, instantiates and buffers a line etc.
		***********************************************************************************/
		static void GizmoInit();

		/*!*********************************************************************************
			\brief
				Pushes line data into linesContainer to be rendered.
			\param [in] pt1:
				Start point
			\param [in] pt2:
				End point
		***********************************************************************************/
		static void RenderLine(Math::Vector2 pt1, Math::Vector2 pt2);

		/*!*********************************************************************************
			\brief
				Renders all Gizmo line draw requests.
		***********************************************************************************/
		static void RenderAllLines();

		// Gizmo data member set functions
		static void SetGizmoLineWidth(f32 width) { gizmoLineWidith = width; }
		static void SetGizmoColor(Math::Vector3 color) { gizmoColor = color; }
	};
}

#endif
