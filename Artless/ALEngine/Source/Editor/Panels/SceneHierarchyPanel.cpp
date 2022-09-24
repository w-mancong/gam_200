#include "pch.h"

namespace ALEngine
{
	namespace Editor
	{
		void SceneHierarchyPanel::OnImGuiRender()
		{
			static std::vector<ECS::Entity>::iterator e_delete = m_entityList.end();

			if (!ImGui::Begin("Scene Hierarchy"))
			{
				ImGui::End();
				return;
			}

			if (ImGui::Button("Add Entity"))
			{
				// Entity Transform
				Transform xform{ Math::Vector2(0.f, 0.f), Math::Vector2(50.f, 50.f), 0.f };

				// Create Entity
				ECS::Entity GO = Coordinator::Instance()->CreateEntity();
				ECS::CreateSprite(GO, xform);

				Sprite& sprite2 = Coordinator::Instance()->GetComponent<Sprite>(GO);
				sprite2.color = Color{ 0.0f, 1.0f, 0.0f, 1.0f };

				// Add entity to queue
				m_entityList.push_back(GO);

				e_delete = m_entityList.end();

				AL_CORE_INFO("Entity Created!");
			}

			ImGui::SameLine();

			if (!m_entityList.empty() && ImGui::Button("Remove Entity"))
				ImGui::OpenPopup("remove_entity_popup");

			b8 remove{ false };
			if (ImGui::BeginPopup("remove_entity_popup"))
			{
				// Print selectable for popup
				for (auto e_it = m_entityList.begin(); e_it != m_entityList.end(); ++e_it)
				{
					std::string tag = "entity #" + std::to_string(*e_it);

					// Each selectable
					if (ImGui::Selectable(tag.c_str()))
					{
						Coordinator::Instance()->DestroyEntity(*e_it);
						e_delete = e_it;
						remove = true;
						ALEditor::Instance()->SetSelectedEntityTransform(-1);
					}
				}

				ImGui::EndPopup();
			}

			if (remove)
			{
				m_entityList.erase(e_delete);
				e_delete = m_entityList.end();
			}

			// Displaying each entity
			ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
			for (auto e_it = m_entityList.begin(); e_it != m_entityList.end(); ++e_it)
			{
				std::string tag = "entity #" + std::to_string(*e_it);
				b8 opened = ImGui::TreeNodeEx((void*)*e_it, flags, tag.c_str());

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
				if (ImGui::Selectable("Remove") && (e_delete != m_entityList.end()))
				{
					Coordinator::Instance()->DestroyEntity(*e_delete);
					m_entityList.erase(e_delete);
					e_delete = m_entityList.end();
					ALEditor::Instance()->SetSelectedEntityTransform(-1);
				}
				ImGui::EndPopup();
			}

			ImGui::End();
		}
	}
}