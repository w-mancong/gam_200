#include "pch.h"

namespace ALEngine::Editor
{
	void SceneHierarchyPanel::OnImGuiRender()
	{
		// Entity to be deleted
		static ECS::EntityList::iterator e_delete = Coordinator::Instance()->GetEntities().end();

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
			Transform xform{ Math::Vector2( Random::Range(-300.0f, 300.f), Random::Range(-300.0f, 300.f) ), 
				Math::Vector2(50.f, 50.f), 0.f };

			// Create Entity
			ECS::Entity GO = Coordinator::Instance()->CreateEntity();
			ECS::CreateSprite(GO, xform);

			Sprite& sprite2 = Coordinator::Instance()->GetComponent<Sprite>(GO);
			sprite2.color = Color{ 0.0f, 1.0f, 0.0f, 1.0f };

			e_delete = Coordinator::Instance()->GetEntities().end();

			AL_CORE_INFO("Entity Created!");
		}

		ImGui::SameLine();

		// Remove Entity Button
		if (!Coordinator::Instance()->GetEntities().empty() && ImGui::Button("Remove Entity"))
			ImGui::OpenPopup("remove_entity_popup");

		b8 remove{ false };
		if (ImGui::BeginPopup("remove_entity_popup"))
		{
			// Print selectable for popup
			for (auto e_it = Coordinator::Instance()->GetEntities().begin(); 
				e_it != Coordinator::Instance()->GetEntities().end(); ++e_it)
			{
				std::string tag = "entity #" + std::to_string(*e_it);

				// Each selectable
				if (ImGui::Selectable(tag.c_str()))
				{
					e_delete = e_it;
					remove = true;
					ALEditor::Instance()->SetSelectedEntityTransform(static_cast<ECS::Entity>(-1));
				}
			}

			ImGui::EndPopup();
		}

		// Displaying each entity
		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
		for (auto e_it = Coordinator::Instance()->GetEntities().begin(); 
			e_it != Coordinator::Instance()->GetEntities().end(); ++e_it)
		{
			EntityData data = Coordinator::Instance()->GetComponent<EntityData>(*e_it);
			b8 opened = ImGui::TreeNodeEx((void*)static_cast<u64>(*e_it), flags, data.tag.c_str());

			// If this is clicked, select this
			if (ImGui::IsItemClicked())
				ALEditor::Instance()->SetSelectedEntityTransform(*e_it);
			else if (ImGui::IsItemClicked(1))
			{
				ImGui::OpenPopup("remove_entity_rightclick"); 
				e_delete = e_it;
			}

			if (opened)
				ImGui::TreePop();
		}

		// Right click to remove entity
		if (ImGui::BeginPopup("remove_entity_rightclick"))
		{
			if (ImGui::Selectable("Remove") && 
				(e_delete != Coordinator::Instance()->GetEntities().end()))
			{
				ALEditor::Instance()->SetSelectedEntityTransform(static_cast<ECS::Entity>(-1));
				remove = true;
			}
			ImGui::EndPopup();
		}

		// If there is an entity to remove
		if (remove)
		{
			Coordinator::Instance()->DestroyEntity(*e_delete);
			e_delete = Coordinator::Instance()->GetEntities().end();
		}

		ImGui::End();
	}	
}