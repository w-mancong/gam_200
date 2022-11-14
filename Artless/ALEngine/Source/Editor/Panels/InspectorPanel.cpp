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

#ifdef EDITOR

#include "imgui_internal.h"
#define make_string(str) #str

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
		// Set constraints
		ImGui::SetNextWindowSizeConstraints(m_PanelMin, ImGui::GetMainViewport()->WorkSize);

		// Begin ImGui
		if (!ImGui::Begin("Inspector"))
		{
			ImGui::End();
			return;
		}

		// Exit if no entity
		if (!HasSelectedEntity())
		{
			ImGui::NewLine();
			ImVec2 textSize = ImGui::CalcTextSize("Click on an Entity to view it's components");
			ImGui::SameLine((ImGui::GetWindowContentRegionMax().x * 0.5f) - (textSize.x * 0.5f));
			ImGui::TextWrapped("Click on an Entity to view it's components");
			ImGui::End();
			return;
		}

		// Inspector Menu
		InspectorMenu();
		
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

		// Check if there is sprite component
		if (Coordinator::Instance()->HasComponent<Rigidbody2D>(m_SelectedEntity))
			DisplayRigidBody();

		// Check if there is sprite component
		if (Coordinator::Instance()->HasComponent<Collider2D>(m_SelectedEntity))
			DisplayCollider();

		//// Check if there is Audio component
		//if (Coordinator::Instance()->HasComponent<______>(m_SelectedEntity))
		//	DisplayAudio();

		//// Check if there is Animator component
		//if (Coordinator::Instance()->HasComponent<______>(m_SelectedEntity))
		//	DisplayAnimator();

		// Add component button
		AddComponentButton();

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

	void InspectorPanel::InspectorMenu(void)
	{
		if (ImGui::BeginMenuBar())
		{
			// File
			if (ImGui::BeginMenu("Add"))
			{
				ImGui::EndMenu();
			}

			// Edit
			if (ImGui::BeginMenu("Remove"))
			{
				ImGui::EndMenu();
			}

			ImGui::EndMainMenuBar();
		}
	}

	void InspectorPanel::DisplayEntityData(void)
	{
		ImGui::Text("Entity Data");

		// Get entity data
		EntityData& data = Coordinator::Instance()->GetComponent<EntityData>(m_SelectedEntity);

		// Entity active
		ImGui::Checkbox("##active", &data.active);

		ImGui::SameLine();

		// Tag
		c8* tag = (c8*)data.tag.c_str();
		// Entity Tag
		ImGui::InputText("Tag", tag, 20);

		ImGui::Separator();

		data.tag = tag;
	}

	void InspectorPanel::DisplayTransform(void)
	{
		// Get transform
		Transform& xform = Coordinator::Instance()->GetComponent<Transform>(m_SelectedEntity);

		// Select between the 3 Gizmos Operations by keypress
		if (Input::KeyTriggered(KeyCode::W))
			if(!ALEditor::Instance()->GetReceivingKBInput())
			m_CurrentGizmoOperation = ImGuizmo::TRANSLATE;
		if (Input::KeyTriggered(KeyCode::R))
			m_CurrentGizmoOperation = ImGuizmo::SCALE;
		if (Input::KeyTriggered(KeyCode::E))
			m_CurrentGizmoOperation = ImGuizmo::ROTATE;

		// Transform
		if (ImGui::TreeNodeEx("Transform Component"))
		{
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
			// 1) Get parent global
			// 2) Temp Variable "offset" = xform.position.x * parent Global Scale
			// 3) Display this offset in inspector
			// 4) Calculate new Local (Parent Inverse Global Scale * offset
			f32 mtx_translate[3]{ xform.position.x, xform.position.y, 0.f },
				mtx_scale[3]{ xform.scale.x, xform.scale.y, 0.f };

			// Float inputs
			ImGui::DragFloat2("Tr", mtx_translate);						// Traslate
			//EDITOR_KEYBOARD_CHECK

			ImGui::DragFloat("Rt", &xform.rotation, 1.f, 0.f, 360.f);	// Rotate
			//EDITOR_KEYBOARD_CHECK

			ImGui::DragFloat2("Sc", mtx_scale);							// Scale
			EDITOR_KEYBOARD_CHECK

			// Set changes
			Transform a(xform);
			a.position.x = mtx_translate[0];
			a.position.y = mtx_translate[1];
			
			a.scale.x = mtx_scale[0];
			a.scale.y = mtx_scale[1];

			// If there are any differences in transform, run command
			if (xform.position.x != a.position.x || xform.position.y != a.position.y ||
				xform.rotation != a.rotation ||
				xform.scale.x != a.scale.x || xform.scale.y != a.scale.y)
			{
				std::shared_ptr<Commands::UpdateComponentCommand<Transform>> cmd = std::make_shared<Commands::UpdateComponentCommand<Transform>>(xform, a);
				Commands::EditorCommandManager::AddCommand(cmd);
			}

			ImGui::TreePop();

			ImGui::Separator();
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
				u64 texture = (u64)Engine::AssetManager::Instance()->GetButtonImage(id);
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
				//payload_flag |= ImGuiDragDropFlags_AcceptNoDrawDefaultRect;

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
			//ImGui::ColorEdit4("Color", clr, clr_flags);
			ImGui::ColorPicker4("Color", clr, clr_flags);
			// Set new color
			spr.color.r = clr[0];
			spr.color.g = clr[1];
			spr.color.b = clr[2];
			spr.color.a = clr[3];

			// Pop TreeNodeEx
			ImGui::TreePop();

			ImGui::Separator();
		}

		// Drag Drop for Selectable
		if (ImGui::BeginDragDropTarget())
		{
			// Payload flag
			ImGuiDragDropFlags payload_flag{ 0 };
			//payload_flag |= ImGuiDragDropFlags_AcceptNoDrawDefaultRect;

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

		// Right click!
		if (ImGui::IsItemClicked(1))
		{
			ImGui::OpenPopup("spritecomp_rightclick");
		}

		if (ImGui::BeginPopup("spritecomp_rightclick"))
		{
			if (ImGui::Selectable("Remove Component"))
			{
				ECS::Coordinator::Instance()->RemoveComponent<Sprite>(m_SelectedEntity);
			}
			ImGui::EndPopup();
		}
	}

	void InspectorPanel::DisplayRigidBody(void)
	{
		// Mass, HasGravity, IsEnabled
		if (ImGui::TreeNodeEx("RigidBody Component"))
		{
			Rigidbody2D& rb = ECS::Coordinator::Instance()->GetComponent<Rigidbody2D>(m_SelectedEntity);

			ImGui::DragFloat("Mass", &rb.mass, 1.f, 1.f, 100.f);

			ImGui::Checkbox("Has Gravity", &rb.hasGravity);

			ImGui::Checkbox("Is Enabled", &rb.isEnabled);

			ImGui::TreePop();

			ImGui::Separator();
		}
	}

	void InspectorPanel::DisplayCollider(void)
	{
		// Enum ColliderType, Rotation, Array2 F32 Scale, IsTriggered, IsDebug, IsEnabled, Vec2 LocalPos, 
		if (ImGui::TreeNodeEx("Collider Component"))
		{
			Collider2D& collider = ECS::Coordinator::Instance()->GetComponent<Collider2D>(m_SelectedEntity);
			
			const char* typeList[1] = {"Rectangle2D_AABB"};
			s32 index = (s32)collider.colliderType;
			// Enum ColliderType
			ImGui::Combo("Collider Type##Collider", &index, typeList, IM_ARRAYSIZE(typeList));

			switch (index)
			{
			case (s32)ColliderType::Rectangle2D_AABB:
				collider.colliderType = ColliderType::Rectangle2D_AABB;
				break;
			case (s32)ColliderType::Rectangle2D_OOBB:
				collider.colliderType = ColliderType::Rectangle2D_OOBB;
				break;
			case (s32)ColliderType::Circle2D:
				collider.colliderType = ColliderType::Circle2D;
				break;
			}

			ImGui::DragFloat("Rotation##Collider", &collider.rotation, 1.f, 0.f, 360.f);

			ImGui::DragFloat2("Scale##Collider", collider.scale);

			ImGui::Checkbox("Is Triggered##Collider", &collider.isTrigger);

			ImGui::Checkbox("Is Enabled##Collider", &collider.isEnabled);

			f32 pos[2]{ collider.m_localPosition.x, collider.m_localPosition.y };
			ImGui::DragFloat("Local Position##Collider", pos);
			collider.m_localPosition = Math::Vec2(pos[0], pos[1]);

			ImGui::TreePop();
		}
	}

	void InspectorPanel::DisplayAudio(void)
	{
		if (ImGui::TreeNodeEx("Audio Component"))
		{
			ImGui::TreePop();
		}
	}

	void InspectorPanel::DisplayAnimator(void)
	{
		if (ImGui::TreeNodeEx("Animator Component"))
		{
			ImGui::TreePop();
		}
	}

	ImGuizmo::OPERATION InspectorPanel::GetCurrGizmoOperation(void) const
	{
		return m_CurrentGizmoOperation;
	}

	void InspectorPanel::AddComponentButton(void)
	{
		// Get Window size
		ImVec2 winsize = ImGui::GetWindowSize();
		if (ImGui::Button("Add Component", ImVec2(winsize.x * 0.8f, 20.f)))
		{
			ImGui::OpenPopup("addcomponent_popup");
		}

		ImGui::SetNextWindowSize(ImVec2(winsize.x * 0.75f, 100.f));
		// Popup for component
		if (ImGui::BeginPopup("addcomponent_popup"))
		{
			ImVec2 textSize = ImGui::CalcTextSize("Available Components");
			ImGui::SameLine((ImGui::GetWindowContentRegionMax().x * 0.5f) - (textSize.x * 0.5f));
			ImGui::Text("Available Components"); ImGui::Separator();
			u32 count = 0;
			for (u32 i = 1; i < (u32)InspectorComponents::InComp_Total; ++i)
			{
				// Check which component
				switch ((InspectorComponents)i)
				{
				case InspectorComponents::InComp_Transform:
					// Check if has component
					if (!ECS::Coordinator::Instance()->HasComponent<Transform>(m_SelectedEntity))
					{
						if (ImGui::Selectable("Transform Component") &&
							m_SelectedEntity != ECS::MAX_ENTITIES)
						{
							// Add Transform Component
							ECS::Coordinator::Instance()->AddComponent<Transform>(m_SelectedEntity, Transform());
						}
						++count;
					}
					break;
				case InspectorComponents::InComp_Sprite:
					// Check if has component
					if (!ECS::Coordinator::Instance()->HasComponent<Sprite>(m_SelectedEntity))
					{
						if (ImGui::Selectable("Sprite Component") &&
							m_SelectedEntity != ECS::MAX_ENTITIES)
						{
							// Add Transform Component
							ECS::Coordinator::Instance()->AddComponent<Sprite>(m_SelectedEntity, Sprite());
						}
						++count;
					}
					break;
				}
			}

			// Check how many
			if (count == 0)
			{	// Print no components
				ImGui::NewLine();
				textSize = ImGui::CalcTextSize("No Components to Add!");
				ImGui::SameLine((ImGui::GetWindowContentRegionMax().x * 0.5f) - (textSize.x * 0.5f));
				ImGui::Text("No Components to Add!");
			}

			ImGui::EndPopup();
		}
	}

	void InspectorPanel::SetPanelMin(Math::Vec2 min)
	{
		m_PanelMin = ImVec2(min.x, min.y);
	}

	void InspectorPanel::SetDefaults(Math::Vec2 pos, Math::Vec2 size)
	{
		m_DefaultPos = ImVec2(pos.x, pos.y);
		m_DefaultSize = ImVec2(size.x, size.y);
	}

}

#endif