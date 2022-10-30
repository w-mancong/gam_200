#include "pch.h"
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>

#include "imgui.h"
#ifndef IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_DEFINE_MATH_OPERATORS
#endif
#include "imgui_internal.h"

#if defined(_MSC_VER) || defined(__MINGW32__)
#include <malloc.h>
#endif
#if !defined(_MSC_VER) && !defined(__MINGW64_VERSION_MAJOR)
#define _malloca(x) alloca(x)
#define _freea(x)
#endif

namespace ALEngine::Editor
{
	// Set default operation to be Translate
	ImGuizmo::OPERATION ScenePanel::m_CurrentGizmoOperation{ ImGuizmo::TRANSLATE };

	ScenePanel::ScenePanel(void)
	{
		m_CurrentGizmoOperation = ImGuizmo::TRANSLATE;
		m_SelectedEntity = ECS::MAX_ENTITIES;

		// Set camera to ortho projection
		m_EditorCamera.ProjectionMatrix(Engine::Camera::Projection::Orthographic);
	}

	ScenePanel::~ScenePanel(void)
	{
	}

	void ScenePanel::OnImGuiRender(void)
	{
		// Check if there is an entity selected (For Gizmos)
		b8 hasSelectedEntity = (m_SelectedEntity == ECS::MAX_ENTITIES) ? false : true;

		Math::vec3 camPos = m_EditorCamera.Position();

		f32 constexpr CAM_SPEED{ 2.5f };

		if (Input::KeyDown(KeyCode::W))
			m_EditorCamera.Position().y += CAM_SPEED;
		if (Input::KeyDown(KeyCode::A))
			m_EditorCamera.Position().x -= CAM_SPEED;
		if (Input::KeyDown(KeyCode::S))
			m_EditorCamera.Position().y -= CAM_SPEED;
		if (Input::KeyDown(KeyCode::D))
			m_EditorCamera.Position().x += CAM_SPEED;

		// Begin ImGui
		if (!ImGui::Begin("Editor Scene"))
		{
			ImGui::End();
			return;
		}

		ECS::Render(m_EditorCamera);

		// Set Scene Width and Height
		if (m_SceneWidth != ImGui::GetContentRegionAvail().x)
			m_SceneWidth = ImGui::GetContentRegionAvail().x;
		if (m_SceneHeight != ImGui::GetContentRegionAvail().y)
			m_SceneHeight = ImGui::GetContentRegionAvail().y;

		// Draw Scene
		ImGui::Image((void*)ECS::GetEditorTexture(), ImGui::GetContentRegionAvail(), ImVec2(0, 1), ImVec2(1, 0));

		// Only render gizmos if an entity is selected
		if (hasSelectedEntity)
		{
			// Get transform
			Transform& xform = Coordinator::Instance()->GetComponent<Transform>(m_SelectedEntity);

			// Translate and Scale matrix
			float mtx_translate[3]{ xform.position.x, xform.position.y, 0.f },
				mtx_scale[3]{ xform.scale.x, xform.scale.y, 0.f },
				mtx_rot[3]{ 0.f, 0.f, xform.rotation };

			// Add camera position
			mtx_translate[0] -= m_EditorCamera.Position().x;
			mtx_translate[1] -= m_EditorCamera.Position().y;

			// Matrix to contain all transforms (SRT)
			float mtx[16];
			
			ImGuizmo::RecomposeMatrixFromComponents(mtx_translate, mtx_rot, mtx_scale, mtx);
			
			// Size of the scene
			ImGuizmo::SetDrawlist();
			ImGuiStyle& style = ImGui::GetStyle();
			ImGuizmo::SetRect(ImGui::GetWindowPos().x + style.WindowPadding.x, 
				ImGui::GetWindowPos().y + style.WindowPadding.y * 3.5f, m_SceneWidth, m_SceneHeight);

			// Manipulate, used for Gizmos
			ImGuizmo::Manipulate(ECS::GetView().value_ptr(), m_EditorCamera.ProjectionMatrix().value_ptr(),
				m_CurrentGizmoOperation, ImGuizmo::WORLD, mtx);

			// Get transform matrices
			ImGuizmo::DecomposeMatrixToComponents(mtx, mtx_translate, mtx_rot, mtx_scale);

			// Set changes
			xform.position.x = mtx_translate[0] + m_EditorCamera.Position().x;
			xform.position.y = mtx_translate[1] + m_EditorCamera.Position().y;

			xform.scale.x = mtx_scale[0];
			xform.scale.y = mtx_scale[1];

			xform.rotation = mtx_rot[2];			
		}

		// Select Entity by clicking on Scene
		if (Input::KeyTriggered(KeyCode::MouseLeftButton))
		{	// Left click
			// Get mouse pos
			Math::Vec2 mousePos = GetMouseWorldPos();
			
			// Check if mouse clicked within scene
			if(mousePos.x != std::numeric_limits<f32>::max() && 
				mousePos.y != std::numeric_limits<f32>::max() )
			{
				// Get list
				ECS::EntityList list = Coordinator::Instance()->GetEntities();

				// If any entities were clicked on
				b8 entity_clicked{ false };

				// Iterate List
				for (const auto& entt : list)
				{
					// Check if has collider
					if (Coordinator::Instance()->HasComponent<Transform>(entt))
					{
						// Get entity Transform
						Transform enttXform = Coordinator::Instance()->GetComponent<Transform>(entt);

						if (Check_Point_To_AABB(mousePos, enttXform.position, enttXform.scale.x, enttXform.scale.y))
						{
							m_SelectedEntity = entt;
							entity_clicked = true;
							break;
						}
					}
				}
				// No entities clicked (clicked viewport)
				if (!entity_clicked && !ImGuizmo::IsOver())
					m_SelectedEntity = ECS::MAX_ENTITIES;
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

	Math::Vec2 ScenePanel::GetMouseWorldPos()
	{
		ImGuiStyle style = ImGui::GetStyle();
		// Set mouse position
		m_ImGuiMousePos = ImGui::GetMousePos();

		// Find the Editor panel
		ImGuiWindow* win = ImGui::FindWindowByName("Editor Scene");

		// Get panel position
		m_ImGuiPanelPos = win->DC.CursorPos;

		// Convert mouse pos from ImGui space to screen space		
		Math::vec4 mousePos{ m_ImGuiMousePos.x - m_ImGuiPanelPos.x,
			-m_ImGuiMousePos.y + m_ImGuiPanelPos.y - style.WindowPadding.y * 0.75f, 0.f, 1.f };

		// Get NDC coords of mouse pos
		mousePos.x = 2.f * (mousePos.x / m_SceneWidth) - 1.f;
		mousePos.y = 2.f * (mousePos.y / m_SceneHeight) - 1.f;

		// Check if within range of scene
		if (mousePos.x >= -1.f && mousePos.x <= 1.f &&
			mousePos.y >= -1.f && mousePos.y <= 1.f)
		{
			using namespace Math;

			// Convert mouse pos from screen space to world space
			// Projection mtx
			Mat4 inv_proj = m_EditorCamera.ProjectionMatrix();
			inv_proj = mat4::InverseT(inv_proj);

			// View matrix
			Mat4 inv_view = m_EditorCamera.ViewMatrix();
			inv_view = Mat4::InverseT(inv_view);

			mousePos = inv_view * inv_proj * mousePos;

			return Math::Vec2(mousePos.x, mousePos.y);
		}
		
		return Math::Vec2(std::numeric_limits<f32>::max(), std::numeric_limits<f32>::max());
	}

	Engine::Camera& ScenePanel::GetEditorCamera(void)
	{
		return m_EditorCamera;
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