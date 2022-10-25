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

namespace ALEngine::Editor
{
	
	SceneHierarchyPanel::SceneHierarchyPanel(void)
	{
		m_EntityList = &Coordinator::Instance()->GetEntities();
	}

	void SceneHierarchyPanel::OnImGuiRender()
	{
		// Entity to be deleted
		static ECS::EntityList::iterator e_delete = m_EntityList->end();

		// Set size constraints of inspector
		ImGui::SetNextWindowSizeConstraints(PANEL_MIN, PANEL_MAX);

		if (!ImGui::Begin("Scene Hierarchy"))
		{
			ImGui::End();
			return;
		}

		// Add Entity Button
		if (ImGui::Button("Add Entity"))
		{
			// Entity Transform
			Transform xform{ Math::Vector2(Random::Range(-300.0f, 300.f), Random::Range(-300.0f, 300.f)),
				Math::Vector2(50.f, 50.f), 0.f };

			// Create Entity
			ECS::Entity GO = Coordinator::Instance()->CreateEntity();
			ECS::CreateSprite(GO, xform);

			sceneGraph.Push(-1, GO);

			Sprite& sprite2 = Coordinator::Instance()->GetComponent<Sprite>(GO);
			sprite2.color = Color{ 0.0f, 1.0f, 0.0f, 1.0f };

			e_delete = m_EntityList->end();

			AL_CORE_INFO("Entity Created!");
		}

		ImGui::SameLine();

		// Remove Entity Button
		if (!m_EntityList->empty() && ImGui::Button("Remove Entity"))
			ImGui::OpenPopup("remove_entity_popup");

		b8 remove{ false };
		if (ImGui::BeginPopup("remove_entity_popup"))
		{
			// Print selectable for popup
			for (auto e_it = m_EntityList->begin(); 
				e_it != Coordinator::Instance()->GetEntities().end(); ++e_it)
			{
				EntityData data = Coordinator::Instance()->GetComponent<EntityData>(*e_it);

				// Each selectable
				if (ImGui::Selectable(data.tag.c_str()))
				{
					e_delete = e_it;
					remove = true;
					ALEditor::Instance()->SetSelectedEntity(ECS::MAX_ENTITIES);
				}
			}

			ImGui::EndPopup();
		}

		// Displaying each entity
		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
		std::vector<ECS::Entity> parentList = sceneGraph.GetParents();

		for (auto e_it = parentList.begin(); e_it != parentList.end(); ++e_it)
		{
			EntityData data = Coordinator::Instance()->GetComponent<EntityData>(*e_it);
			b8 opened = ImGui::TreeNodeEx((void*)static_cast<u64>(*e_it), flags, data.tag.c_str());
			
			// If this is clicked, select this
			if (ImGui::IsItemClicked())
				ALEditor::Instance()->SetSelectedEntity(*e_it);
			else if (ImGui::IsItemClicked(1))
			{
				ImGui::OpenPopup("remove_entity_rightclick"); 
				ALEditor::Instance()->SetSelectedEntity(*e_it);
				//e_delete = e_it;
			}

			if (opened)
			{
				sceneGraph.FindChildren(*e_it);
				std::vector<ECS::Entity> childrenList = sceneGraph.GetChildren();

				for (auto child = childrenList.begin(); child != childrenList.end(); ++child)
				{
					EntityData childData = Coordinator::Instance()->GetComponent<EntityData>(*child);
					b8 childOpened = (ImGui::TreeNodeEx((void*)static_cast<u64>(*child), flags, childData.tag.c_str()));
					// If this is clicked, select this
					if (ImGui::IsItemClicked())
						ALEditor::Instance()->SetSelectedEntity(*child);
					else if (ImGui::IsItemClicked(1))
					{
						ImGui::OpenPopup("child_entity_rightclick");
						ALEditor::Instance()->SetSelectedEntity(*child);
						//e_delete = e_it;
					}

					if (childOpened)
						ImGui::TreePop();
				}
				ImGui::TreePop();
			}
		}

		// Right click to remove entity
		if (ImGui::BeginPopup("remove_entity_rightclick"))
		{
			if (ImGui::Selectable("Remove") && 
				(e_delete != m_EntityList->end()))
			{
				ALEditor::Instance()->SetSelectedEntity(ECS::MAX_ENTITIES);
				remove = true;
			}
			if (ImGui::Selectable("Add child") && (ALEditor::Instance()->GetSelectedEntity() != ECS::MAX_ENTITIES))
			{
				// Entity Transform
				Transform xform{ Math::Vector2(Random::Range(-300.0f, 300.f), Random::Range(-300.0f, 300.f)),
					Math::Vector2(50.f, 50.f), 0.f };

				// Create Entity
				ECS::Entity GO = Coordinator::Instance()->CreateEntity();
				ECS::CreateSprite(GO, xform);

				sceneGraph.Push(ALEditor::Instance()->GetSelectedEntity(), GO); // add child entity under parent

				Sprite& sprite2 = Coordinator::Instance()->GetComponent<Sprite>(GO);
				sprite2.color = Color{ 0.0f, 1.0f, 0.0f, 1.0f };

				e_delete = m_EntityList->end();

				AL_CORE_INFO("Entity Created!");
			}
			ImGui::EndPopup();
		}

		// If there is an entity to remove
		if (remove)
		{
			sceneGraph.Destruct(*e_delete);
			Coordinator::Instance()->DestroyEntity(*e_delete);
			
			remove = false;
		}

		ImGui::End();
	}	
}