#include "pch.h"
#include <glm/glm/glm.hpp>

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

		// Set Scene Width and Height
		if (m_SceneWidth != ImGui::GetContentRegionAvail().x)
			m_SceneWidth = ImGui::GetContentRegionAvail().x;
		if (m_SceneHeight != ImGui::GetContentRegionAvail().y)
			m_SceneHeight = ImGui::GetContentRegionAvail().y;

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
				m_CurrentGizmoOperation, ImGuizmo::WORLD, mtx);

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
			// Get Style
			ImGuiStyle style = ImGui::GetStyle();

			// Convert mouse pos from ImGui space to screen space		
			glm::vec4 mousePos{ ImGui::GetMousePos().x - ImGui::GetCursorScreenPos().x,
				-ImGui::GetMousePos().y + ImGui::GetCursorScreenPos().y - style.WindowPadding.y * 0.75f, 0.f, 1.f};

			// Get NDC coords of mouse pos
			mousePos.x = 2.f * (mousePos.x / m_SceneWidth) - 1.f;
			mousePos.y = 2.f * (mousePos.y / m_SceneHeight) - 1.f;

			// Check if within range of scene
			if (mousePos.x >= -1.f && mousePos.x <= 1.f &&
				mousePos.y >= -1.f && mousePos.y <= 1.f)
			{
				AL_CORE_INFO("Mouse Position: {}, {}", mousePos.x, mousePos.y);

				// Ray clip coords
				glm::vec4 ray_clip = glm::vec4(mousePos.x, mousePos.y, -1.f, 1.f);

				// Convert mouse pos from screen space to world space
				// Projection mtx
				glm::mat4x4 inv_proj;
				for (size_t i = 0; i < 4; ++i)
					for (size_t j = 0; j < 4; ++j)
						inv_proj[i][j] = ECS::GetProjection()(i, j);
				inv_proj = glm::inverse(inv_proj);

				// Ray Camera Coords
				glm::vec4 ray_eye = inv_proj * ray_clip;
				ray_eye.z = -1.f;	ray_eye.w = 0.f;

				// View matrix
				glm::mat4x4 inv_view;
				for (size_t i = 0; i < 4; ++i)
					for (size_t j = 0; j < 4; ++j)
						inv_view[i][j] = ECS::GetView()(i, j);
				inv_view = glm::inverse(inv_view);

				mousePos = inv_view * mousePos;

				//mousePos.w = (mousePos.w == 0.f) ? 1.f : 1.f / mousePos.w;
				//mousePos.x *= (f32)Graphics::OpenGLWindow::width * 0.5f;
				//mousePos.y *= (f32)Graphics::OpenGLWindow::height * 0.5f;

				AL_CORE_DEBUG("Mouse Position: {}, {}", mousePos.x, mousePos.y);


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

						if (Check_Point_To_AABB(Math::Vec2(mousePos.x, mousePos.y), enttXform.position, enttCol.scale[0], enttCol.scale[1]))
						{
							m_SelectedEntity = entt;
							break;
						}
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

	f64 ScenePanel::GetSceneWidth(void)
	{
		return m_SceneWidth;
	}

	f64 ScenePanel::GetSceneHeight(void)
	{
		return m_SceneHeight;
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