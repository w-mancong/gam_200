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
		
		ImGui::Image((void*)ECS::GetFBTexture(), ImGui::GetContentRegionAvail(), ImVec2(0, 1), ImVec2(1, 0));
		
		// Only render gizmos if an entity is selected
		if (hasSelectedEntity)
		{
			// Get transform
			Transform& xform = Coordinator::Instance()->GetComponent<Transform>(m_SelectedEntity);

			// Translate and Scale matrix
			float mtx_translate[3]{ xform.position.x, xform.position.y, 0.f },
				mtx_scale[3]{ xform.scale.x, xform.scale.y, 0.f },
				mtx_rot[3]{ 0.f, 0.f, xform.rotation };

			float mtx[16];
			
			ImGuizmo::RecomposeMatrixFromComponents(mtx_translate, mtx_rot, mtx_scale, mtx);
									
			ImGuizmo::SetDrawlist();
			float windowWidth = (float)ImGui::GetWindowWidth();
			float windowHeight = (float)ImGui::GetWindowHeight();
			ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);

			// Manipulate
			ImGuizmo::Manipulate(ECS::GetView().value_ptr(), ECS::GetProjection().value_ptr(),
				m_CurrentGizmoOperation, ImGuizmo::LOCAL, mtx);

			ImGuizmo::DecomposeMatrixToComponents(mtx, mtx_translate, mtx_rot, mtx_scale);

			// Set changes
			xform.position.x = mtx_translate[0];
			xform.position.y = mtx_translate[1];

			xform.scale.x = mtx_scale[0];
			xform.scale.y = mtx_scale[1];

			xform.rotation = mtx_rot[2];			
		}

		// Select Entity by clicking on Scene
		if (Input::KeyTriggered(KeyCode::MouseLeftButton))
		{	// Left click
			// Convert mouse pos from ImGui space to screen space		
			Math::Vec2 mousePos{ ImGui::GetMousePos().x, ImGui::GetMousePos().y };
			mousePos.x = mousePos.x * (Graphics::OpenGLWindow::width / ImGui::GetWindowWidth());
			mousePos.y = mousePos.y * (Graphics::OpenGLWindow::height / ImGui::GetWindowHeight());

			// Convert mouse pos from screen space to world space


			// Get list
			ECS::EntityList list = Coordinator::Instance()->GetEntities();

			// Iterate List
			for (const auto& entt : list)
			{
				// Check if has collider
				if (Coordinator::Instance()->HasComponent<Collider2D>(entt) &&
					Coordinator::Instance()->HasComponent<Transform>(entt))
				{
					// Get entity Collider2D and Transform
					Collider2D enttCol = Coordinator::Instance()->GetComponent<Collider2D>(entt);
					Transform enttXform = Coordinator::Instance()->GetComponent<Transform>(entt);

					if (Check_Point_To_AABB(mousePos, enttXform.position, enttCol.scale[0], enttCol.scale[1]))
					{
						m_SelectedEntity = entt;
						break;
					}
				}
			}
		}

		ImGui::End();
	}

	void ScenePanel::SetCurrentGizmoOperation(ImGuizmo::OPERATION _op)
	{
		m_CurrentGizmoOperation = _op;
	}
	
	void ScenePanel::SetSelectedEntity(ECS::Entity _entt)
	{
		m_SelectedEntity = _entt;
	}

	ECS::Entity ScenePanel::GetSelectedEntity(void)
	{
		return m_SelectedEntity;
	}

	bool Check_Point_To_AABB(Math::Vec2 position, Math::Vec2 boxCenter,
		float width, float height) 
	{
		//Holder for bottom left and top right
		Math::Vec2 bottomLeft = boxCenter - Math::Vec2(width * 0.5f, height * 0.5f);
		Math::Vec2 topRight = boxCenter + Math::Vec2(width * 0.5f, height * 0.5f);

		//Intersection check
		if (position.x < bottomLeft.x || 
			position.x > topRight.x || 
			position.y < bottomLeft.y || 
			position.y > topRight.y) {
			//No intersection
			return false;
		}

		//Intersection
		return true;
	}
}