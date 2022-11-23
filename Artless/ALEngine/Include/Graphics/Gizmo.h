/*!
file:	Gizmo.h
author:	Mohamed Zafir
email:	m.zafir@digipen.edu
brief:	This file contains the Gizmo class declaration. Gizmo is a debugging feature
		for the engine.

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/

#ifndef	GIZMO_H
#define GIZMO_H

namespace ALEngine::Gizmos
{
	class Gizmo
	{
	private:
		static u32 GizmoVaoId, GizmoVboId;

	public:
		static Graphics::Shader gizmolineShader;
		static bool gizmoToggle;

		/*!*********************************************************************************
			\brief
				Initializes Gizmo, instantiates and buffers a line etc.
		***********************************************************************************/
		static void GizmoInit();
		static void RenderLine(Math::Vector2 pt1, Math::Vector2 pt2, Math::Vector4 const& color = Math::Vector4{ 1.0f, 1.0f, 1.0f, 1.0f }, f32 width = 1.0f);
		static void RenderAllLines();
		static void ClearContainer(void);
		//static void RenderCircle(Math::Vector2 center, f32 radius);
	};
}
#endif
