#include "pch.h"

namespace ALEngine
{
	namespace Editor
	{
		void SceneHierarchyPanel::OnImGuiRender()
		{
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

				AL_CORE_INFO("Entity Created!");
			}

			u32 count = 0;
			ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
			for (auto e : m_entityList)
			{
				std::string tag = "entity #" + std::to_string(e);
				b8 opened = ImGui::TreeNodeEx((void*)e, flags, tag.c_str());

				// If this is clicked, select this
				if (ImGui::IsItemClicked())
					ALEditor::Instance()->SetSelectedEntityTransform(e);

				if (opened)
					ImGui::TreePop();
			}

			ImGui::End();
		}
	}
}