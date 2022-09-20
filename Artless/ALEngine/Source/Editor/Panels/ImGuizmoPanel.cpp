#include "pch.h"

namespace ALEngine
{
	namespace Editor 
	{
		// Set default operation to be Translate
		ImGuizmo::OPERATION ImGuizmoPanel::m_CurrentGizmoOperation{ ImGuizmo::TRANSLATE };
		
		void ImGuizmoPanel::OnImGuiRender()
		{
			// Begin ImGui
			if (!ImGui::Begin("Editor"))
			{
				ImGui::End();
				//AL_CORE_ERROR("Editor Panel Collapsed");
				return;
			}

			// Select between the 3 Gizmos Operations by keypress
			if (Input::KeyTriggered(KeyCode::T))
				m_CurrentGizmoOperation = ImGuizmo::TRANSLATE;
			if (Input::KeyTriggered(KeyCode::S))
				m_CurrentGizmoOperation = ImGuizmo::SCALE;
			if (Input::KeyTriggered(KeyCode::R))
				m_CurrentGizmoOperation = ImGuizmo::ROTATE;

			// Select betwen the 3 Gizmos Operations by radio buttons
			if (ImGui::RadioButton("Translate", m_CurrentGizmoOperation == ImGuizmo::TRANSLATE))
				m_CurrentGizmoOperation = ImGuizmo::TRANSLATE;
			if (ImGui::RadioButton("Rotate", m_CurrentGizmoOperation == ImGuizmo::ROTATE))
				m_CurrentGizmoOperation = ImGuizmo::ROTATE;
			if (ImGui::RadioButton("Scale", m_CurrentGizmoOperation == ImGuizmo::SCALE))
				m_CurrentGizmoOperation = ImGuizmo::SCALE;

			// Translate and Scale matrix
			float mtx_translate[3]{ m_SelectedTransform->position.x, m_SelectedTransform->position.y, 0.f },
				mtx_scale[3]{ m_SelectedTransform->scale.x, m_SelectedTransform->scale.y, 0.f };

			// FLoat inputs
			ImGui::InputFloat2("Tr", mtx_translate);					// Traslate
			ImGui::InputFloat("Rt", &m_SelectedTransform->rotation);	// Rotate
			ImGui::InputFloat2("Sc", mtx_scale);						// Scale

			// Rotate matrix
			float mtx_rot[3]{ 0.f, 0.f, m_SelectedTransform->rotation };


			// Make transform matrix
			float mtx[16];
			ImGuizmo::RecomposeMatrixFromComponents(mtx_translate, mtx_rot, mtx_scale, mtx);

			// Set changes
			m_SelectedTransform->position.x = mtx_translate[0];
			m_SelectedTransform->position.y = mtx_translate[1];

			m_SelectedTransform->scale.x = mtx_scale[0];
			m_SelectedTransform->scale.y = mtx_scale[1];

			/*
			ImGuizmo::SetDrawlist();
			float windowWidth = (float)ImGui::GetWindowWidth();
			float windowHeight = (float)ImGui::GetWindowHeight();
			ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);

			// Manipulate
			ImGuizmo::Manipulate(ECS::GetView().value_ptr(), ECS::GetProjection().value_ptr(),
				m_CurrentGizmoOperation, ImGuizmo::LOCAL, mtx);
				*/

			ImGui::End();
		}
	}
}