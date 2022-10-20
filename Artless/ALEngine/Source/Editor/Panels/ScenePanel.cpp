#include "pch.h""

namespace ALEngine::Editor
{
	// Set default operation to be Translate
	ImGuizmo::OPERATION ScenePanel::m_CurrentGizmoOperation{ ImGuizmo::TRANSLATE };

	ScenePanel::ScenePanel(void)
	{
		m_CurrentGizmoOperation = ImGuizmo::TRANSLATE;
		m_SelectedEntity = ECS::MAX_ENTITIES;
	}

	ScenePanel::~ScenePanel(void)
	{
	}

	void ScenePanel::OnImGuiRender(void)
	{
		// Check if there is an entity selected (For Gizmos)
		b8 hasSelectedEntity = (m_SelectedEntity == ECS::MAX_ENTITIES) ? false : true;

		// Begin ImGui
		if (!ImGui::Begin("Viewport"))
		{
			ImGui::End();
			return;
		}

		ImGui::Image((void*)ECS::GetFBTexture(), ImGui::GetWindowSize(), ImVec2(0, 1), ImVec2(1, 0));
		
		// Only render gizmos if an entity is selected
		if (hasSelectedEntity)
		{
			float mtx[16];
			
			ImGuizmo::SetDrawlist();
			float windowWidth = (float)Graphics::OpenGLWindow::width;
			float windowHeight = (float)Graphics::OpenGLWindow::height;
			ImGuizmo::SetRect(0, 0, windowWidth, windowHeight);

			// Manipulate
			ImGuizmo::Manipulate(ECS::GetView().value_ptr(), ECS::GetProjection().value_ptr(),
				m_CurrentGizmoOperation, ImGuizmo::LOCAL, mtx);
			
		}

		ImGui::End();
	}

	void ScenePanel::SetCurrentGizmoOperation(ImGuizmo::OPERATION _op)
	{
		m_CurrentGizmoOperation = _op;
	}
}