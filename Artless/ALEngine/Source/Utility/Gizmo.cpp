/*!
file:	Gizmo.cpp
author:	Mohamed Zafir
email:	m.zafir\@digipen.edu
brief:	This file contains the Gizmo class definition. Gizmo is a debugging feature
		for the engine.

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/

#include "pch.h"
#include "Engine/Camera.h"

namespace ALEngine::Gizmos
{
	namespace
	{
		/*!*********************************************************************************
			\brief
				Contain relevant data to render line
		***********************************************************************************/
		struct LineData
		{
			Math::vec2 pt1{}, pt2{};
			Math::vec4 color{};
			f32 width{ 1.0f };
		};
	}

	// declare static class member variables
	u32 Gizmo::GizmoVaoId, Gizmo::GizmoVboId;
	Graphics::Shader Gizmo::gizmolineShader;
	std::vector<LineData> linesContainer;
	bool Gizmo::gizmoToggle;

	/*!*********************************************************************************
		\brief
			Initializes Gizmo, instantiates and buffers a line etc.
	***********************************************************************************/
	void Gizmo::GizmoInit()
	{
		f32 position[] =
		{
			-0.5f, 0.0f,
			 0.5f, 0.0f
		};

		// initialize line shader
		gizmolineShader = Graphics::Shader{ "Assets/Dev/Shaders/gizmo.vert", "Assets/Dev/Shaders/gizmo.frag" };
		gizmoToggle = true;
		//gizmoCircleSegments = 32; // default circle will have 32 line segments

		glGenVertexArrays(1, &GizmoVaoId);
		glGenBuffers(1, &GizmoVboId);
		glBindVertexArray(GizmoVaoId);
		glBindBuffer(GL_ARRAY_BUFFER, GizmoVboId);
		glBufferData(GL_ARRAY_BUFFER, sizeof(position), position, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(f32) * 2, (void*)0);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	/*!*********************************************************************************
		\brief
			Pushes line data into linesContainer to be rendered.

		\param [in] pt1:
			Start point
		\param [in] pt2:
			End point
		\param [in] color:
			color of the line
		\param [in] width:
			width of the line
	***********************************************************************************/
	void Gizmo::RenderLine(Math::Vector2 pt1, Math::Vector2 pt2, Math::Vector4 const& color, f32 width)
	{
		if (gizmoToggle)
			linesContainer.push_back({ pt1, pt2, color, width });
	}

	/*!*********************************************************************************
		\brief
			Renders all Gizmo line draw requests.
	***********************************************************************************/
	void Gizmo::RenderAllLines()
	{
		if (gizmoToggle)
		{
			for (LineData const& ld : linesContainer)
			{
				Math::Vector2 const& pt1 = ld.pt1, pt2 = ld.pt2;
				Math::Vector2 midPoint = (pt1 + pt2) / 2.f;
				f32 lineLength = sqrt((pt1.x - pt2.x) * (pt1.x - pt2.x) + (pt1.y - pt2.y) * (pt1.y - pt2.y));
				f32 angle = atan2(pt2.y - pt1.y, pt2.x - pt1.x) * 180.f / 3.141592f;
				gizmolineShader.use();
				gizmolineShader.Set("view", ECS::GetView());
				gizmolineShader.Set("proj", ECS::GetProjection());
				gizmolineShader.Set("scale", Math::Matrix4x4::Scale(lineLength, 1.0f, 1.0f));
				gizmolineShader.Set("rotate", Math::Matrix4x4::Rotation(angle, Math::Vector3(0.0f, 0.0f, 1.0f)));
				gizmolineShader.Set("translate", Math::Matrix4x4::Translate(midPoint.x, midPoint.y, 0.0f));
				gizmolineShader.Set("color", ld.color.x, ld.color.y, ld.color.z, ld.color.w);
				glLineWidth(ld.width);

				glBindVertexArray(GizmoVaoId);
				glBindBuffer(GL_ARRAY_BUFFER, GizmoVboId);

				glDrawArrays(GL_LINES, 0, 2); // render line

				glEnableVertexAttribArray(0);
				glBindBuffer(GL_ARRAY_BUFFER, 0);

				glLineWidth(1.f); // reset line thickness
			}
		}
	}

	void Gizmo::ClearContainer(void)
	{
		linesContainer.clear();
	}
}