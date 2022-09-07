#include "pch.h"
#include "Graphics/Shader.h"
#include "Graphics/Gizmo.h"
#include "Engine/Camera.h"

namespace ALEngine
{
	namespace Gizmos
	{
		// declare static class member variables
		u32 Gizmo::GizmoVaoId, Gizmo::GizmoVboId;
		Graphics::Shader Gizmo::gizmolineShader;
		f32 Gizmo::gizmoLineWidith;
		std::vector<std::pair<Math::Vector2, Math::Vector2>> Gizmo::linesContainer;
		bool Gizmo::gizmoToggle;
		Math::Vector3 Gizmo::gizmoColor;

		// this function makes the sample line
		void Gizmo::GizmoInit()
		{
			f32 position[] =
			{
				-0.5f, 0.0f,
				 0.5f, 0.0f
			};

			// initialize line shader
			gizmolineShader = Graphics::Shader{ "Assets/Shaders/gizmo.vert", "Assets/Shaders/gizmo.frag" };
			Engine::Camera camera{ Math::Vector3(0.0f, 0.0f, 725.0f) };
			gizmolineShader.use();
			gizmolineShader.Set("view", camera.ViewMatrix());
			gizmolineShader.Set("proj", camera.ProjectionMatrix());
			Math::Matrix4x4 model = Math::Matrix4x4::Scale(0.0f, 1.0f, 1.0f) * Math::Matrix4x4::Rotation(0.f, Math::Vector3(0.0f, 0.0f, 1.0f)) * Math::Matrix4x4::Translate(0.f, 0.f, 0.0f);
			gizmolineShader.Set("model", model);
			gizmoLineWidith = 1.f;
			gizmoToggle = true;
			gizmoColor = Math::Vector3(0.f, 1.f, 0.f); // default color is green

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

		void Gizmo::RenderLine(Math::Vector2 pt1, Math::Vector2 pt2)
		{
			if(gizmoToggle)
				linesContainer.push_back(std::pair<Math::Vector2, Math::Vector2>(pt1, pt2));
		}

		void Gizmo::RenderCircle(Math::Vector2 center, f32 radius)
		{
			f32 step = 2.f * 3.141592f / 26.f; 
			Math::Vector2 first, second;
			for (f32 theta{}; theta < 6.f; theta += step)
			{
				first.x = center.x + radius * cos(theta);
				first.y = center.y + radius * sin(theta);
				theta += step;
				second.x = center.x + radius * cos(theta);
				second.y = center.y + radius * sin(theta);
				RenderLine(first, second);
			}
		}

		void Gizmo::RenderAllLines()
		{
			if (gizmoToggle)
			{
				for (std::pair<Math::Vector2, Math::Vector2>& pair : linesContainer)
				{
					Math::Vector2 pt1 = pair.first, pt2 = pair.second;
					Math::Vector2 midPoint = (pt1 + pt2) / 2.f;
					f32 lineLength = sqrt((pt1.x - pt2.x) * (pt1.x - pt2.x) + (pt1.y - pt2.y) * (pt1.y - pt2.y));
					f32 angle = atan2(pt2.y - pt1.y, pt2.x - pt1.x) * 180.f / 3.141592f;
					gizmolineShader.use();
					Math::Matrix4x4 model = Math::Matrix4x4::Scale(lineLength, 1.0f, 1.0f) *
						Math::Matrix4x4::Rotation(angle, Math::Vector3(0.0f, 0.0f, 1.0f)) *
						Math::Matrix4x4::Translate(midPoint.x, midPoint.y, 0.0f);
					gizmolineShader.Set("model", model);
					gizmolineShader.Set("color", gizmoColor.x, gizmoColor.y, gizmoColor.z, 1.f);
					glLineWidth(gizmoLineWidith);

					glBindVertexArray(GizmoVaoId);
					glBindBuffer(GL_ARRAY_BUFFER, GizmoVboId);

					glDrawArrays(GL_LINES, 0, 2); // render line

					glEnableVertexAttribArray(0);
					glBindBuffer(GL_ARRAY_BUFFER, 0);

					glLineWidth(1.f); // reset line thickness
				}
			}
			linesContainer.clear();
		}
	}
}