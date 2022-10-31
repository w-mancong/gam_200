/*!
file:	InspectorPanel.cpp
author: Lucas Nguyen
email:	l.nguyen@digipen.edu
brief:	This file contains function definitions for the InspectorPanel class.
		The InspectorPanel class contains information and functions necessary for 
		the Inspector Panel of the editor to be displayed.

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#include "pch.h"

#include "imgui_internal.h"

namespace ALEngine::Editor 
{
	// Set default operation to be Translate
	ImGuizmo::OPERATION InspectorPanel::m_CurrentGizmoOperation{ ImGuizmo::TRANSLATE };

	// File buffer size
	const u32 FILE_BUFFER_SIZE{ 1000 };

	enum class TRANSFORM_MODE {
		TRANSLATE = 0,
		ROTATE,
		SCALE
	};

	InspectorPanel::InspectorPanel(void)
	{
		m_CurrentGizmoOperation = ImGuizmo::TRANSLATE;
		m_SelectedEntity = ECS::MAX_ENTITIES;
		m_SelectedComponent = InspectorComponents::InComp_Total;
	}

	InspectorPanel::~InspectorPanel(void)
	{
	}
	
	void InspectorPanel::OnImGuiRender(void)
	{
		// Set size constraints of inspector
		ImGui::SetNextWindowSizeConstraints(PANEL_MIN, PANEL_MAX);

		// Begin ImGui
		if (!ImGui::Begin("Inspector"))
		{
			return;
		}

		// Exit if no entity
		if (!HasSelectedEntity())
		{
			ImGui::NewLine();
			ImGui::Text("Click on an Entity to view it's components");
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

		ImGui::End();
	}	

	void InspectorPanel::SetSelectedEntity(ECS::Entity setter)
	{
		m_SelectedEntity = setter;
	}

	const ECS::Entity InspectorPanel::GetSelectedEntity(void)
	{
		return m_SelectedEntity;
	}

	b8 InspectorPanel::HasSelectedEntity(void)
	{
		return !(m_SelectedEntity == ECS::MAX_ENTITIES);
	}

	void InspectorPanel::DisplayEntityData(void)
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

	void InspectorPanel::DisplayTransform(void)
	{
		// Get transform
		Transform& xform = Coordinator::Instance()->GetComponent<Transform>(m_SelectedEntity);

		// Transform
		if (ImGui::TreeNodeEx("Transform Component"))
		{
			// Select between the 3 Gizmos Operations by keypress
			if (Input::KeyTriggered(KeyCode::T))
				m_CurrentGizmoOperation = ImGuizmo::TRANSLATE;
			if (Input::KeyTriggered(KeyCode::S))
				m_CurrentGizmoOperation = ImGuizmo::SCALE;
			if (Input::KeyTriggered(KeyCode::R))
				m_CurrentGizmoOperation = ImGuizmo::ROTATE;

			// Rotate
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

			// Translate and Scale matrix
			f32 mtx_translate[3]{ xform.position.x, xform.position.y, 0.f },
				mtx_scale[3]{ xform.scale.x, xform.scale.y, 0.f };

			// FLoat inputs
			ImGui::DragFloat2("Tr", mtx_translate);						// Traslate
			ImGui::DragFloat("Rt", &xform.rotation, 1.f, 0.f, 360.f);	// Rotate
			ImGui::DragFloat2("Sc", mtx_scale);							// Scale

			// Set changes
			xform.position.x = mtx_translate[0];
			xform.position.y = mtx_translate[1];

			xform.scale.x = mtx_scale[0];
			xform.scale.y = mtx_scale[1];

			ImGui::TreePop();

			ImGui::Separator();
		}


		if (Input::KeyTriggered(KeyCode::MouseRightButton))
		{
			ImGui::OpenPopup("component_rightclick");
		}

		if (ImGui::BeginPopup("component_rightclick"))
		{
			if (ImGui::Selectable("Remove"))
			{

			}
			ImGui::EndPopup();
		}
	}

	void InspectorPanel::DisplaySprite(void)
	{
		// Get Sprite
		Sprite& spr = Coordinator::Instance()->GetComponent<Sprite>(m_SelectedEntity);
		if (ImGui::TreeNodeEx("Sprite Component"))
		{
			// Image
			if (spr.filePath != "")
			{
				Guid id = Engine::AssetManager::Instance()->GetGuid(spr.filePath.c_str());
				u32 texture = Engine::AssetManager::Instance()->GetButtonImage(id);
				ImVec2 winSize = ImGui::GetWindowSize();
				ImGui::Image(reinterpret_cast<ImTextureID>(texture), { winSize.x * 0.5f, winSize.x * 0.5f }, { 0, 1 }, { 1, 0 });
			}

			// File path
			c8* fp = (c8*)spr.filePath.c_str();
			ImGui::PushID("FilePath");
			ImGui::InputText("File Path", fp, FILE_BUFFER_SIZE);

			// Drag Drop!
			if (ImGui::BeginDragDropTarget())
			{
				// Payload flag
				ImGuiDragDropFlags payload_flag{ 0 };
				payload_flag |= ImGuiDragDropFlags_AcceptNoDrawDefaultRect;

				// Get Drag and Drop Payload
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ASSET_ITEM", payload_flag))
				{
					// Get filepath
					size_t fileLen;	c8 filePath[FILE_BUFFER_SIZE];
					wcstombs_s(&fileLen, filePath, FILE_BUFFER_SIZE, (const wchar_t*)payload->Data, payload->DataSize);

					// Check if image (png or jpg)
					std::string fileString = filePath;
					if (fileString.find(".jpg") != std::string::npos ||
						fileString.find(".png") != std::string::npos)
					{
						// Set Filepath
						spr.filePath = filePath;

						spr.id = Engine::AssetManager::Instance()->GetGuid(filePath);
					}
					else
					{
						AL_CORE_ERROR("A .jpg or .png file is required!");
					}
				}
				ImGui::EndDragDropTarget();
			}
			else
				spr.filePath = fp;

			ImGui::PopID();

			// Color wheel
			ImGuiColorEditFlags clr_flags = ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_PickerHueBar;
			f32 clr[4] = { spr.color.r, spr.color.g, spr.color.b, spr.color.a };
			ImGui::ColorEdit4("Color", clr, clr_flags);

			// Set new color
			spr.color.r = clr[0];
			spr.color.g = clr[1];
			spr.color.b = clr[2];
			spr.color.a = clr[3];

			// Pop TreeNodeEx
			ImGui::TreePop();

			ImGui::Separator();
		}
	}

	ImGuizmo::OPERATION InspectorPanel::GetCurrGizmoOperation(void) const
	{
		return m_CurrentGizmoOperation;
	}

}