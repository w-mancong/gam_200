/*!
file:	ScenePanel.cpp
author: Lucas Nguyen
email:	l.nguyen@digipen.edu
brief:	This file contains function definitions for the ScenePanel class.
		The ScenePanel class contains information and functions necessary for
		the Scene Panel of the editor to be displayed.

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#include "pch.h"
#if EDITOR

#include "imgui.h"
#include "imgui_internal.h"

namespace ALEngine::Editor
{
	ScenePanel::ScenePanel(void)
	{
		// Set camera to ortho projection
		m_EditorCamera.ProjectionMatrix(Engine::Camera::Projection::Orthographic);
		m_EditorCamera.Position() = { Math::Vec3(-static_cast<f32>(Graphics::OpenGLWindow::width >> 1), -static_cast<f32>(Graphics::OpenGLWindow::height >> 1), 725.f) };

		m_EditorCamera.ProjBottom() = 0.0f;
		m_EditorCamera.ProjTop()	= static_cast<f32>(Graphics::OpenGLWindow::height);
		m_EditorCamera.ProjLeft()   = 0.0f;
		m_EditorCamera.ProjRight()  = static_cast<f32>(Graphics::OpenGLWindow::width);
	}

	ScenePanel::~ScenePanel(void)
	{
	}

	void ScenePanel::OnImGuiRender(void)
	{
		using namespace Commands;
		ECS::Entity selectedEntity = ALEditor::Instance()->GetSelectedEntity();

		// Check if there is an entity selected (For Gizmos)
		b8 hasSelectedEntity = (selectedEntity == ECS::MAX_ENTITIES) ? false : true;

		UserInput();

		// Set constraints
		ImGui::SetNextWindowSizeConstraints(m_PanelMin, ImGui::GetMainViewport()->WorkSize);

		// Begin ImGui
		if (!ImGui::Begin("Editor Scene"))
		{
			ImGui::End();
			return;
		}

		//ECS::Render(m_EditorCamera);

		// Set Scene Width and Height
		if (m_SceneWidth != ImGui::GetContentRegionAvail().x)
			m_SceneWidth = ImGui::GetContentRegionAvail().x;
		if (m_SceneHeight != ImGui::GetContentRegionAvail().y)
			m_SceneHeight = ImGui::GetContentRegionAvail().y;

		// Draw Scene
		u64 tex = (u64)ECS::GetEditorTexture();
		ImGui::Image((void*)tex, ImGui::GetContentRegionAvail(), ImVec2(0, 1), ImVec2(1, 0));

		// Only render gizmos if an entity is selected
		if (hasSelectedEntity && Coordinator::Instance()->HasComponent<Transform>(selectedEntity))
		{
			// Get transform
			Transform& xform = Coordinator::Instance()->GetComponent<Transform>(selectedEntity);

			// Translate and Scale matrix
			float mtx_translate[3]{ xform.position.x, xform.position.y, 0.f },
				mtx_scale[3]{ xform.localScale.x, xform.localScale.y, 0.f },
				mtx_rot[3]{ 0.f, 0.f, xform.localRotation };

			//float mtx_translate[3]{ xform.position.x, xform.position.y, 0.f },
			//	mtx_scale[3]{ xform.scale.x, xform.scale.y, 0.f },
			//	mtx_rot[3]{ 0.f, 0.f, xform.rotation };

			//f32 const TEMP_POSITION[2]{ mtx_translate[0], mtx_translate[1] };

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
				ALEditor::Instance()->GetCurrentGizmoOperation(), ImGuizmo::WORLD, mtx);

			// Get transform matrices
			ImGuizmo::DecomposeMatrixToComponents(mtx, mtx_translate, mtx_rot, mtx_scale);

			mtx_translate[0] += m_EditorCamera.Position().x;
			mtx_translate[1] += m_EditorCamera.Position().y;

			Tree::BinaryTree const& sceneGraph = ECS::GetSceneGraph();
			s32 parent{ -1 };
			if ((parent = sceneGraph.GetParent(selectedEntity)) != -1)
			{
				Transform const& parentTranform = Coordinator::Instance()->GetComponent<Transform>(parent);
				Math::mat4 const& parentGlobalInverse = parentTranform.modelMatrix.Inverse();
				Math::vec3 const& newLocalPosition = parentGlobalInverse * Math::vec3(mtx_translate[0], mtx_translate[1], 0.0f);

				mtx_translate[0] = newLocalPosition.x;
				mtx_translate[1] = newLocalPosition.y;
			}

			// Set changes
			Transform updated;
			updated.localPosition.x = mtx_translate[0];
			updated.localPosition.y = mtx_translate[1];

			updated.localScale.x = mtx_scale[0];
			updated.localScale.y = mtx_scale[1];

			updated.localRotation = mtx_rot[2];

			// If there are any differences in transform, run command
			if (xform.localPosition.x != updated.localPosition.x || xform.localPosition.y != updated.localPosition.y ||
				xform.localRotation != updated.localRotation ||
				xform.localScale.x != updated.localScale.x || xform.localScale.y != updated.localScale.y)
			{
				if (Commands::EditorCommandManager::CanAddCommand())
				{
					utils::Ref<COMP_CMD<Transform>> cmd = utils::CreateRef<COMP_CMD<Transform>>(xform, updated);
					EditorCommandManager::AddCommand(cmd);
				}
			}
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
							ALEditor::Instance()->SetSelectedEntity(entt);
							entity_clicked = true;
							break;
						}
					}
				}
				// No entities clicked (clicked viewport)
				if (!entity_clicked && !ImGuizmo::IsOver())
					ALEditor::Instance()->SetSelectedEntity(ECS::MAX_ENTITIES);
			}
		}
		ImGui::End();
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
		// Get style
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

	void ScenePanel::SetPanelMin(Math::Vec2 min)
	{
		m_PanelMin = ImVec2(min.x, min.y);
	}

	void ScenePanel::SetDefaults(Math::Vec2 pos, Math::Vec2 size)
	{
		m_DefaultPos = ImVec2(pos.x, pos.y);
		m_DefaultSize = ImVec2(size.x, size.y);
	}

	void ScenePanel::UserInput(void)
	{
		f32 constexpr CAM_SPEED{ 7.5f };

		ImGuiIO io = ImGui::GetIO();

		if (io.WantTextInput == false)
		{
			if (ImGui::IsKeyDown(ImGuiKey_UpArrow))
				m_EditorCamera.Position().y += CAM_SPEED;
			if (ImGui::IsKeyDown(ImGuiKey_LeftArrow))
				m_EditorCamera.Position().x -= CAM_SPEED;
			if (ImGui::IsKeyDown(ImGuiKey_DownArrow))
				m_EditorCamera.Position().y -= CAM_SPEED;
			if (ImGui::IsKeyDown(ImGuiKey_RightArrow))
				m_EditorCamera.Position().x += CAM_SPEED;
		}

		// Right Mouse Button Move Camera
		static Math::Vec2 mousePosBegin{};
		if (Input::KeyTriggered(KeyCode::MouseRightButton))
		{
			Math::Vec2 pos = GetMouseWorldPos();
			if (pos.x != std::numeric_limits<f32>::max() && pos.y != std::numeric_limits<f32>::max())
			{
				mousePosBegin = pos;
			}
		}
		else if (Input::KeyDown(KeyCode::MouseRightButton))
		{
			Math::Vec2 pos = GetMouseWorldPos();

			if (pos.x != std::numeric_limits<f32>::max() && pos.y != std::numeric_limits<f32>::max())
			{
				Math::Vec2 change = mousePosBegin - pos;
				m_EditorCamera.Position() += Math::Vec3(change.x, change.y, 0.f);
			}
		}

		// Update camera zoom
		m_EditorCamera.ProjBottom() += static_cast<f32>(Input::m_MouseWheelEvent) * 10.0f;
		m_EditorCamera.ProjLeft()   += static_cast<f32>(Input::m_MouseWheelEvent) * 10.0f;

		std::cout << m_EditorCamera.ProjBottom() << std::endl;
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

#endif