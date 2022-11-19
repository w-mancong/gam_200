/*!
file:	SceneHierarchyPanel.cpp
author: Lucas Nguyen
email:	l.nguyen@digipen.edu
brief:	This file contains function definitions for the SceneHierarchPanel class.
		The SceneHierarchyPanel class contains information and functions necessary for
		the Scene Hierarchy Panel of the editor to be displayed.

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#include "pch.h"

#if EDITOR

namespace ALEngine::Editor
{

	SceneHierarchyPanel::SceneHierarchyPanel(void)
	{
		m_EntityList = &Coordinator::Instance()->GetEntities();
	}

	void SceneHierarchyPanel::OnImGuiRender()
	{
		Tree::BinaryTree& sceneGraph = ECS::GetSceneGraph();

		// Set constraints
		ImGui::SetNextWindowSizeConstraints(m_PanelMin, ImGui::GetMainViewport()->WorkSize);

		if (!ImGui::Begin("Scene Hierarchy"))
		{
			ImGui::End();
			return;
		}

		// Make Panel Child so panel can have Drag & Drop
		ImGui::BeginChild("SceneHierarchyPanel##PanelChild", ImGui::GetContentRegionAvail());

		// Add Entity Button
		if (ImGui::Button("Add Entity"))
		{
			// Entity Transform
			Transform xform = Transform{ Math::Vector2(0.f, 0.f),
				Math::Vector2(50.f, 50.f) };

			// Create Entity
			ECS::Entity GO = Coordinator::Instance()->CreateEntity();
			ECS::CreateSprite(GO, xform);

			sceneGraph.Push(-1, GO);

			Sprite& sprite2 = Coordinator::Instance()->GetComponent<Sprite>(GO);
			sprite2.color = Color{ 0.0f, 1.0f, 0.0f, 1.0f };

			ALEditor::Instance()->SetSelectedEntity(ECS::MAX_ENTITIES);

			AL_CORE_INFO("Entity Created!");
		}

		ImGui::SameLine();

		// Remove Entity Button
		if (!m_EntityList->empty() && ImGui::Button("Remove Entity"))
			ImGui::OpenPopup("remove_entity_popup");

		b8 remove{ false };
		// Remove Entity Popup!!
		if (ImGui::BeginPopup("remove_entity_popup"))
		{
			// Print selectable for popup
			for (auto e_it = m_EntityList->begin(); 
				e_it != Coordinator::Instance()->GetEntities().end(); ++e_it)
			{
				EntityData& data = Coordinator::Instance()->GetComponent<EntityData>(*e_it);

				// Each selectable
				if (ImGui::Selectable(data.tag.c_str()))
				{
					ALEditor::Instance()->SetSelectedEntity(*e_it);
					remove = true;
				}
			}

			ImGui::EndPopup();
		}

		// Displaying each entity
		std::vector<s32> parentList = sceneGraph.GetParents();

		b8 popup_hasopen{ false };
		// Iterate through parent list
		for (auto e_it = parentList.begin(); e_it != parentList.end(); ++e_it)
			UpdateEntitySHP(*e_it, popup_hasopen);

		// Check if any popups open
		if (popup_hasopen)
			ImGui::OpenPopup("entity_rightclick");
		
		// Right click to remove entity
		if (ImGui::BeginPopup("entity_rightclick"))
		{
			// Remove button, to be clicked
			if (ImGui::Selectable("Remove"))
			{
				// Check if selectable clicked
				remove = true;
			}
			ECS::Entity selectedEntity = ALEditor::Instance()->GetSelectedEntity();
			// Add child
			if (ImGui::Selectable("Add child") && (selectedEntity != ECS::MAX_ENTITIES))
			{
				// Entity Transform
				Transform xform = Transform{ Math::Vector2(2.f, 2.f),
					Math::Vector2(1.f, 1.f) };

				// Create Entity
				ECS::Entity GO = Coordinator::Instance()->CreateEntity();
				ECS::CreateSprite(GO, xform);
				sceneGraph.Push(selectedEntity, GO); // add child entity under parent

				Sprite& sprite2 = Coordinator::Instance()->GetComponent<Sprite>(GO);
				sprite2.color = Color{ 0.0f, 1.0f, 0.0f, 1.0f };

				AL_CORE_INFO("Entity Created!");
			}
			ImGui::EndPopup();
		}
		// If there is an entity to remove
		if (remove)
		{
			ECS::Entity to_delete = ALEditor::Instance()->GetSelectedEntity();
			sceneGraph.FindChildren(to_delete);
			std::vector<s32> childrenVect = sceneGraph.GetChildren();
			for (s32 child : childrenVect)
			{
				Coordinator::Instance()->DestroyEntity(child); // delete children
			}
			sceneGraph.Destruct(to_delete);
			Coordinator::Instance()->DestroyEntity(to_delete); // delete parent
			
			remove = false;
			ALEditor::Instance()->SetSelectedEntity(ECS::MAX_ENTITIES);
		}

		ImGui::EndChild();

		// Drop object here
		if (ImGui::BeginDragDropTarget())
		{
			// Set payload
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("HIERARCHY_ENTITY"))
			{
				assert(payload->DataSize == sizeof(ECS::Entity));
				ECS::Entity child_pl = *(ECS::Entity*)payload->Data;
				// Insert remove parent code here
				sceneGraph.MoveBranch(child_pl, -1);
				
				Transform& xform = Coordinator::Instance()->GetComponent<Transform>(child_pl);
				xform.localPosition = xform.position;
				xform.localRotation = xform.rotation;
				xform.localScale	= xform.scale;
			}

			ImGui::EndDragDropTarget();
		}

		ImGui::End();
	}
	
	void SceneHierarchyPanel::UpdateEntitySHP(ECS::Entity child, b8& popup_hasopen)
	{
		Tree::BinaryTree& sceneGraph = ECS::GetSceneGraph();

		// Flag for Tree Node
		ImGuiTreeNodeFlags flags = 0;

		// Check if has children
		sceneGraph.FindImmediateChildren(child);
		std::vector<s32> childrenList = sceneGraph.GetChildren();

		if (childrenList.empty())
			flags |= ImGuiTreeNodeFlags_Leaf;
		else
			flags |= ImGuiTreeNodeFlags_OpenOnArrow;

		// Get entity data
		EntityData data = Coordinator::Instance()->GetComponent<EntityData>(child);

		// Begin Tree Node
		b8 opened = ImGui::TreeNodeEx((void*)static_cast<u64>(child), flags, data.tag.c_str());

		// Check if hovered on entity
		ImGuiHoveredFlags hover_flag = ImGuiHoveredFlags_AllowWhenBlockedByActiveItem;
		if (ImGui::IsItemHovered(hover_flag))
		{
			//AL_CORE_CRITICAL("Hovering Over: {}", child);
			m_EntityHover = child;
		}

		// Drag object from here
		if (ImGui::BeginDragDropSource())
		{
			ECS::Entity dragged = ALEditor::Instance()->GetSelectedEntity();
			ImGui::SetDragDropPayload("HIERARCHY_ENTITY", &dragged, sizeof(ECS::Entity));
			ImGui::EndDragDropSource();
		}

		// Drop object here
		if (ImGui::BeginDragDropTarget())
		{
			// Set payload
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("HIERARCHY_ENTITY"))
			{
				assert(payload->DataSize == sizeof(ECS::Entity));
				ECS::Entity child_pl = *(ECS::Entity*)payload->Data;

				// Check payload is not own Entity
				if (m_EntityHover != child_pl)
				{
					// move child_pl to be child under m_EntityHover
					sceneGraph.MoveBranch(child_pl, m_EntityHover);

					Transform& childTransform = Coordinator::Instance()->GetComponent<Transform>(child_pl);
					Transform const& parentTrans = Coordinator::Instance()->GetComponent<Transform>(m_EntityHover);
					// Recalculating the local position for child_pl, no need to recaculate it's children as they will always be their local position amount away from it's parent
					childTransform.localPosition = math::mat4::Model({}, { parentTrans.scale.x, parentTrans.scale.y, 1.0f }, childTransform.rotation).Inverse() * (childTransform.position - parentTrans.position);
					childTransform.localRotation = childTransform.rotation - parentTrans.rotation;
					childTransform.localScale = { childTransform.scale.x / parentTrans.scale.x, childTransform.scale.y / parentTrans.scale.y };

					s32 node{ static_cast<s32>(child_pl) }, parentNode{};
					while (1)
					{
						parentNode = sceneGraph.GetMap()[node].parent;
						if (parentNode == -1)
						{
							break;
						}

						node = parentNode;
					}
				}
			}
			ImGui::EndDragDropTarget();
		}
		
		// If this is clicked, select this
		if (ImGui::IsItemClicked())
			ALEditor::Instance()->SetSelectedEntity(child);
		// If this is right clicked, select this
		else if (ImGui::IsItemClicked(1))
		{
			ImGui::OpenPopup("entity_rightclick");
			popup_hasopen = true;
			ALEditor::Instance()->SetSelectedEntity(child);
		}

		// If tree node is open
		if (opened)
		{
			for (auto child_it : childrenList)
				UpdateEntitySHP(child_it, popup_hasopen);
			ImGui::TreePop();
		}
	}

	void SceneHierarchyPanel::SetPanelMin(Math::Vec2 min)
	{
		m_PanelMin = ImVec2(min.x, min.y);
	}

	void SceneHierarchyPanel::SetDefaults(Math::Vec2 pos, Math::Vec2 size)
	{
		m_DefaultPos = ImVec2(pos.x, pos.y);
		m_DefaultSize = ImVec2(size.x, size.y);
	}
}

#endif