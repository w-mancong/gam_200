#include "pch.h"

namespace ALEngine
{
	namespace Editor 
	{
		// Set default operation to be Translate
		ImGuizmo::OPERATION InspectorPanel::m_CurrentGizmoOperation{ ImGuizmo::TRANSLATE };

		enum class TRANSFORM_MODE {
			TRANSLATE = 0,
			ROTATE,
			SCALE
		};
		
		void InspectorPanel::OnImGuiRender()
		{
			static bool useSnap[3] = { false, false, false };
			static float snap[3] = { 1.f, 1.f, 1.f };
			static float bounds[] = { -0.5f, -0.5f, -0.5f, 0.5f, 0.5f, 0.5f };
			static float boundsSnap[] = { 0.1f, 0.1f, 0.1f };
			static bool boundSizing = false;
			static bool boundSizingSnap = false;

			ImGui::SetNextWindowPos(ImVec2(10, 10));
			ImGui::SetNextWindowSize(ImVec2(320, 340));
			// Begin ImGui
			if (!ImGui::Begin("Inspector"))
			{
				ImGui::End();
				//AL_CORE_CRITICAL("Editor Panel Collapsed");
				return;
			}

			// Get transform
			Transform& xform = Coordinator::Instance()->GetComponent<Transform>(m_SelectedEntity);
			// Transform
			ImGui::Text("Transform");
			/*
			// Select between the 3 Gizmos Operations by keypress
			if (Input::KeyTriggered(KeyCode::T))
				m_CurrentGizmoOperation = ImGuizmo::TRANSLATE;
			if (Input::KeyTriggered(KeyCode::S))
				m_CurrentGizmoOperation = ImGuizmo::SCALE;
			if (Input::KeyTriggered(KeyCode::R))
				m_CurrentGizmoOperation = ImGuizmo::ROTATE;

			// Select betwen the 3 Gizmos Operations by radio buttons
			// Translate
			if (ImGui::RadioButton("Translate", m_CurrentGizmoOperation == ImGuizmo::TRANSLATE))
				m_CurrentGizmoOperation = ImGuizmo::TRANSLATE;
			ImGui::SameLine();

			// Rotate
			if (ImGui::RadioButton("Rotate", m_CurrentGizmoOperation == ImGuizmo::ROTATE))
				m_CurrentGizmoOperation = ImGuizmo::ROTATE;
			ImGui::SameLine();

			// Scale
			if (ImGui::RadioButton("Scale", m_CurrentGizmoOperation == ImGuizmo::SCALE))
				m_CurrentGizmoOperation = ImGuizmo::SCALE;
				*/

			// Translate and Scale matrix
			float mtx_translate[3]{ xform.position.x, xform.position.y, 0.f },
				mtx_scale[3]{ xform.scale.x, xform.scale.y, 0.f };

			// FLoat inputs
			ImGui::DragFloat2("Tr", mtx_translate);		// Traslate
			ImGui::DragFloat("Rt", &xform.rotation);	// Rotate
			ImGui::DragFloat2("Sc", mtx_scale);			// Scale

			// Rotate matrix
			float mtx_rot[3]{ 0.f, 0.f, xform.rotation };

			// Make transform matrix
			float mtx[16];
			ImGuizmo::RecomposeMatrixFromComponents(mtx_translate, mtx_rot, mtx_scale, mtx);

			// Set changes
			xform.position.x = mtx_translate[0];
			xform.position.y = mtx_translate[1];

			xform.scale.x = mtx_scale[0];
			xform.scale.y = mtx_scale[1];

			// Separate
			ImGui::Separator();
			

			// Get Sprite
			Sprite& spr = Coordinator::Instance()->GetComponent<Sprite>(m_SelectedEntity);
			ImGui::Text("Sprite");
			ImGuiColorEditFlags clr_flags =  ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_PickerHueBar;
			float clr[4] = { spr.color.r, spr.color.g, spr.color.b, spr.color.a };
			ImGui::ColorPicker4("Color", clr, clr_flags);

			// Set new color
			spr.color.r = clr[0];
			spr.color.g = clr[1];
			spr.color.b = clr[2];
			spr.color.a = clr[3];

			/*
			ImGuizmo::SetDrawlist();
			float windowWidth = (float)Graphics::OpenGLWindow::width;
			float windowHeight = (float)Graphics::OpenGLWindow::height;
			ImGuizmo::SetRect(0, 0, windowWidth, windowHeight);

			// Manipulate
			ImGuizmo::Manipulate(ECS::GetView().value_ptr(), ECS::GetProjection().value_ptr(),
				m_CurrentGizmoOperation, ImGuizmo::LOCAL, mtx);
			*/

			ImGui::End();
		}
	}
}