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
				/*
				// Create Entity
				ECS::Entity GO = Coordinator::Instance()->CreateEntity();

				// Entity Transform
				Transform xform{ Math::Vector2(0.f, 0.f), Math::Vector2(50.f, 100.f), 0.f };
				Coordinator::Instance()->AddComponent<Transform>(GO, xform);

				// Add sprite
				Sprite spr;
				spr.mode = RenderMode::Line;
				spr.color = Color{ 0.f, 1.f, 0.f, 1.f };
				Coordinator::Instance()->AddComponent<Sprite>(GO, spr);

				// Add entity to queue
				m_entityList.push_back(GO);
				*/

				AL_CORE_INFO("Entity Created!");
			}

			u32 count = 0;
			ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
			for (auto e : m_entityList)
			{
				std::string tag = "entity #" + std::to_string(count++);
				b8 opened = ImGui::TreeNodeEx((void*)e, flags, tag.c_str());
				if (ImGui::IsItemClicked())
				{
					Transform& e_xform = Coordinator::Instance()->GetComponent<Transform>(e);
					ALEditor::Instance()->SetSelectedEntityTransform(&e_xform);
				}

				if (opened)
					ImGui::TreePop();
			}

			ImGui::End();
		}
	}
}