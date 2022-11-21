#include "pch.h"

#include "imgui.h"
#include "imgui_internal.h"
#ifdef EDITOR

namespace ALEngine::Editor
{
	const std::filesystem::path amimatorPath{ "Assets/Dev/Animator" };//base file 
	b8 popUpWindow{false};

	AnimatorEditorPanel::AnimatorEditorPanel()
	{
	}

	AnimatorEditorPanel::~AnimatorEditorPanel()
	{
	}

	void AnimatorEditorPanel::OnImGuiRender(b8 pOpen)
	{
		std::string animatorsContents = FileContents(amimatorPath);
		std::string word;

		std::istringstream data(animatorsContents);
		std::vector<const c8*> items;

		while (std::getline(data, word, ' '))
		{
			if (word.empty())
			{
				break;
			}
			word = word.substr(word.find_last_of("\\") + 1);
			items.push_back(word.c_str());
		}

		ImGuiWindowFlags flag = 0;
		if (ImGui::Begin("Animator Panel##", &pOpen))
		{
		
			if (ImGui::Button("close"))
			{
				pOpen = false;
			}

			//static int item_type = 4;
            //const char* items[] = { "Item1", "Item2", "AAAA", "AAAB", "AABB", "ABBB", "ABBB" };

			if (Input::KeyTriggered(KeyCode::A))
			{
				items.push_back("hi");
			}

			//static char testing[15];

			//ImGui::Combo("Testing", &item_type, items.data(), items.size(), items.size());
			//ImGui::Combo("Testing", &item_type, items, IM_ARRAYSIZE(items), IM_ARRAYSIZE(items));

			static const char* current_item = NULL;
			if (ImGui::BeginCombo("Animators##", current_item)) // The second parameter is the label previewed before opening the combo.
			{
				for (size_t n = 0; n < items.size(); n++)
				{
					bool is_selected = (current_item == items[n]); // You can store your selection however you want, outside or inside your objects
					if (ImGui::Selectable(items[n], is_selected))
					{
						current_item = items[n];
					}
					if (is_selected)
					{
						ImGui::SetItemDefaultFocus();   // You may set the initial focus when opening the combo (scrolling + for keyboard navigation support)
					}
				}
				ImGui::EndCombo();
			}

			if (ImGui::Button("Create Clip##"))//create clip button
			{
				popUpWindow = true;			
			}

			if (popUpWindow)
			{
				ImGui::Begin("CreateClip");

				if (ImGui::Button("close##"))
				{
					popUpWindow = false;
				}
				ImGui::End();
			}
		

			ImGui::End();
		}
		

	}

	void AnimatorEditorPanel::SetPanelMin(Math::Vec2 min)
	{
		m_PanelMin = ImVec2(min.x, min.y);
	}

	void AnimatorEditorPanel::SetDefaults(Math::Vec2 pos, Math::Vec2 size)
	{
		m_DefaultPos = ImVec2(pos.x, pos.y);
		m_DefaultSize = ImVec2(size.x, size.y);
	}

	void AnimatorEditorPanel::Default(void)
	{
	}

	std::string AnimatorEditorPanel::FileContents(const std::filesystem::path& path)
	{
		// Sanity check
		//if (!std::filesystem::is_regular_file(path))
		//{
		//	return { };
		//}

		// Open the file
		// Note that we have to use binary mode as we want to return a string
		// representing matching the bytes of the file on the file system.
		std::string resultstring;

		for (const auto& entry : std::filesystem::directory_iterator(path))
		{
			resultstring = entry.path().string();
			//std::ifstream file(entry, std::ios::in | std::ios::binary);
			//
			//if (!file.is_open())
			//{
			//	return { };
			//}

			//// Read contents
			//std::string content{ std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>() };

			//// Close the file
			//file.close();
			//resultstring = content;
		}
		return resultstring;
	}

	void AnimatorEditorPanel::FilterOutWords(std::string sentence)
	{
		
	}
}
#endif