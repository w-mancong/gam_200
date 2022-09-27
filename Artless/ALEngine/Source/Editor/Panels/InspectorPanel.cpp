#include "pch.h"

namespace ALEngine::Editor 
{
	// Set default operation to be Translate
	ImGuizmo::OPERATION InspectorPanel::m_CurrentGizmoOperation{ ImGuizmo::TRANSLATE };

	enum class TRANSFORM_MODE {
		TRANSLATE = 0,
		ROTATE,
		SCALE
	};

	InspectorPanel::InspectorPanel(void)
	{
		m_CurrentGizmoOperation = ImGuizmo::TRANSLATE;
		m_SelectedEntity = ECS::MAX_ENTITIES;
		m_HasEntityTransform = false;
	}
	
	void InspectorPanel::OnImGuiRender(void)
	{
		// Skip if there are no selected entities
		if (m_SelectedEntity == ECS::MAX_ENTITIES)
			return;

		// Set size constraints of inspector
		ImGui::SetNextWindowSizeConstraints(PANEL_MIN, PANEL_MAX);

		// Begin ImGui
		if (!ImGui::Begin("Inspector"))
		{
			ImGui::End();
			return;
		}

		// Window size
		ImVec2 winSize = ImGui::GetWindowSize();

		// Check if entity has EntityData component
		if (Coordinator::Instance()->HasComponent<EntityData>(m_SelectedEntity))
			DisplayEntityData();

		// Check if has transform component
		if (Coordinator::Instance()->HasComponent<Transform>(m_SelectedEntity))
			DisplayTransform();

		// Check if there is sprite component
		if (Coordinator::Instance()->HasComponent<Sprite>(m_SelectedEntity))
			DisplaySprite();
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

	void InspectorPanel::DisplayEntityData()
	{
		ImGui::Text("Entity Data");

		// Get entity data
		EntityData& data = Coordinator::Instance()->GetComponent<EntityData>(m_SelectedEntity);

		// Tag
		c8* tag = (c8*)data.tag.c_str();

		// Entity Tag
		ImGui::InputText("Entity Tag", tag, 20);

		// Entity active
		ImGui::Checkbox("Entity Active", &data.active);

		ImGui::Separator();

		data.tag = tag;
	}

	void InspectorPanel::DisplayTransform()
	{
		// Get transform
		Transform& xform = Coordinator::Instance()->GetComponent<Transform>(m_SelectedEntity);
		// Transform
		ImGui::Text("Transform Component");
		/*
			// Select between the 3 Gizmos Operations by keypress
			if (Input::KeyTriggered(KeyCode::T))
				m_CurrentGizmoOperation = ImGuizmo::TRANSLATE;
			if (Input::KeyTriggered(KeyCode::S))
				m_CurrentGizmoOperation = ImGuizmo::SCALE;
			if (Input::KeyTriggered(KeyCode::R))
				m_CurrentGizmoOperation = ImGuizmo::ROTATE;

		// Rotate
		if (ImGui::RadioButton("Rotate", m_CurrentGizmoOperation == ImGuizmo::ROTATE))
			m_CurrentGizmoOperation = ImGuizmo::ROTATE;
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
		f32 mtx_translate[3]{ xform.position.x, xform.position.y, 0.f },
			mtx_scale[3]{ xform.scale.x, xform.scale.y, 0.f };

		// FLoat inputs
		ImGui::DragFloat2("Tr", mtx_translate);						// Traslate
		ImGui::DragFloat("Rt", &xform.rotation, 1.f, 0.f, 360.f);	// Rotate
		ImGui::DragFloat2("Sc", mtx_scale);							// Scale

		// Rotate matrix
		f32 mtx_rot[3]{ 0.f, 0.f, xform.rotation };

		// Make transform matrix
		f32 mtx[16];
		ImGuizmo::RecomposeMatrixFromComponents(mtx_translate, mtx_rot, mtx_scale, mtx);

		// Set changes
		xform.position.x = mtx_translate[0];
		xform.position.y = mtx_translate[1];

		xform.scale.x = mtx_scale[0];
		xform.scale.y = mtx_scale[1];

		// Separate
		ImGui::Separator();
	}

	void InspectorPanel::DisplaySprite()
	{
		// Get Sprite
		Sprite& spr = Coordinator::Instance()->GetComponent<Sprite>(m_SelectedEntity);
		ImGui::Text("Sprite Component");
		ImGuiColorEditFlags clr_flags = ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_PickerHueBar;
		f32 clr[4] = { spr.color.r, spr.color.g, spr.color.b, spr.color.a };
		ImGui::ColorPicker4("Color", clr, clr_flags);

		// Set new color
		spr.color.r = clr[0];
		spr.color.g = clr[1];
		spr.color.b = clr[2];
		spr.color.a = clr[3];
	}
}