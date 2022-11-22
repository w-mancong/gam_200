#include "pch.h"

#include "imgui.h"
#include "imgui_internal.h"

#ifdef EDITOR

namespace ALEngine::Editor
{
	const std::filesystem::path audioPath{ "Assets/Audio" };//base file 
	AudioEditorPanel::AudioEditorPanel()
	{
	}

	AudioEditorPanel::~AudioEditorPanel()
	{
	}

	void AudioEditorPanel::OnImGuiRender(b8& pOpen)
	{
		//std::string audioContents = FileContents(audioPath);
		std::vector<const c8*> items{"testing1", "testing2"};

		//////loop through directory and create buttons for each file
		//for (auto& directoryEntry : std::filesystem::directory_iterator(audioPath))
		//{
		//	//file default path
		//	const auto& path = directoryEntry.path();

		//	//file relative path
		//	std::filesystem::path const& relativePath = std::filesystem::relative(path, audioPath);

		//	//file name from relative path 
		//	std::string const& fileNamestring = relativePath.filename().string();

		//	if (fileNamestring.find(".meta") != std::string::npos)
		//	{
		//		continue;
		//	}
		//	ImGui::PushID(fileNamestring.c_str());

		//	items.push_back(fileNamestring.c_str());
		//	ImGui::PopID();
		//}

		if (pOpen)
		{
			ImGui::OpenPopup("AudioVolumeMixer");
		}


		//ImGui::Begin("AudioVolumeMixer");


		if (ImGui::BeginPopupModal("AudioVolumeMixer", NULL, ImGuiWindowFlags_MenuBar))
		{
			if (ImGui::Button("close"))
			{
				pOpen = false;
			}
			if (Input::KeyTriggered(KeyCode::A))
			{
				items.push_back("hi");
			}

			//static char testing[15];

			//ImGui::Combo("Testing", &item_type, items.data(), items.size(), items.size());
			//ImGui::Combo("Testing", &item_type, items, IM_ARRAYSIZE(items), IM_ARRAYSIZE(items));

			static const char* current_item = NULL;
			if (ImGui::BeginCombo("Audio##", current_item)) // The second parameter is the label previewed before opening the combo.
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

			const float spacing = 4;

			static f32 float_value_master = 0.f;
			static f32 float_value_music = 0.f;
			static f32 float_value_reverb = 0.f;
			static f32 float_value_effects = 0.f;

			ImGui::Text("Master");
			ImGui::SameLine();
			ImGui::Text("Music");
			ImGui::SameLine();
			ImGui::Text("Reverb");
			ImGui::SameLine();
			ImGui::Text("Effects");

			ImGui::PushID(0);
			ImGui::PushStyleColor(ImGuiCol_FrameBg, (ImVec4)ImColor::HSV(0.f / 7.0f, 0.5f, 0.5f));
			ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, (ImVec4)ImColor::HSV(0.f / 7.0f, 0.6f, 0.5f));
			ImGui::PushStyleColor(ImGuiCol_FrameBgActive, (ImVec4)ImColor::HSV(0.f / 7.0f, 0.7f, 0.5f));
			ImGui::PushStyleColor(ImGuiCol_SliderGrab, (ImVec4)ImColor::HSV(0.f / 7.0f, 0.9f, 0.9f));
			ImGui::VSliderFloat("##master", ImVec2(60, 160), &float_value_master, 0.0f, 1.0f);
			ImGui::PopStyleColor(4);
			ImGui::PopID();

			ImGui::SameLine();

			ImGui::PushID(1);
			ImGui::PushStyleColor(ImGuiCol_FrameBg, (ImVec4)ImColor::HSV(1.f / 7.0f, 0.5f, 0.5f));
			ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, (ImVec4)ImColor::HSV(1.f / 7.0f, 0.6f, 0.5f));
			ImGui::PushStyleColor(ImGuiCol_FrameBgActive, (ImVec4)ImColor::HSV(1.f / 7.0f, 0.7f, 0.5f));
			ImGui::PushStyleColor(ImGuiCol_SliderGrab, (ImVec4)ImColor::HSV(1.f / 7.0f, 0.9f, 0.9f));
			ImGui::VSliderFloat("##music", ImVec2(60, 160), &float_value_music, 0.0f, 1.0f);
			ImGui::PopStyleColor(4);
			ImGui::PopID();

			ImGui::SameLine();

			ImGui::PushID(2);
			ImGui::PushStyleColor(ImGuiCol_FrameBg, (ImVec4)ImColor::HSV(4.f / 7.0f, 0.5f, 0.5f));
			ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, (ImVec4)ImColor::HSV(4.f / 7.0f, 0.6f, 0.5f));
			ImGui::PushStyleColor(ImGuiCol_FrameBgActive, (ImVec4)ImColor::HSV(4.f / 7.0f, 0.7f, 0.5f));
			ImGui::PushStyleColor(ImGuiCol_SliderGrab, (ImVec4)ImColor::HSV(4.f / 7.0f, 0.9f, 0.9f));
			ImGui::VSliderFloat("##reverb", ImVec2(60, 160), &float_value_reverb, 0.0f, 1.0f);
			ImGui::PopStyleColor(4);
			ImGui::PopID();

			ImGui::SameLine();

			ImGui::PushID(3);
			ImGui::PushStyleColor(ImGuiCol_FrameBg, (ImVec4)ImColor::HSV(6.f / 7.0f, 0.5f, 0.5f));
			ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, (ImVec4)ImColor::HSV(6.f / 7.0f, 0.6f, 0.5f));
			ImGui::PushStyleColor(ImGuiCol_FrameBgActive, (ImVec4)ImColor::HSV(6.f / 7.0f, 0.7f, 0.5f));
			ImGui::PushStyleColor(ImGuiCol_SliderGrab, (ImVec4)ImColor::HSV(6.f / 7.0f, 0.9f, 0.9f));
			ImGui::VSliderFloat("##effects", ImVec2(60, 160), &float_value_effects, 0.0f, 1.0f);
			ImGui::PopStyleColor(4);
			ImGui::PopID();

			ImGui::EndPopup();
		}

		//ImGui::End();
	}

	void AudioEditorPanel::SetPanelMin(Math::Vec2 min)
	{
		m_PanelMin = ImVec2(min.x, min.y);
	}

	void AudioEditorPanel::SetDefaults(Math::Vec2 pos, Math::Vec2 size)
	{
		m_DefaultPos = ImVec2(pos.x, pos.y);
		m_DefaultSize = ImVec2(size.x, size.y);
	}

	void AudioEditorPanel::Default(void)
	{
	}

	std::string AudioEditorPanel::FileContents(const std::filesystem::path& path)
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

}

#endif