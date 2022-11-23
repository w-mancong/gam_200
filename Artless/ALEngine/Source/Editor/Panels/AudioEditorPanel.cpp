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
		if (pOpen)
		{
			ImGui::OpenPopup("AudioVolumeMixer");
		}

		ImGuiWindowFlags flag = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize;

		//ImGui::Begin("AudioVolumeMixer");
		if (ImGui::BeginPopupModal("AudioVolumeMixer", &pOpen, flag))
		{
			const f32 spacing = 20;
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(spacing, spacing));

			static f32 floatValueMaster = 1.f;
			static f32 floatValueBgm = 1.f;
			static f32 floatValueSfx = 1.f;

			ImGui::Text("Master");
			ImGui::SameLine();
			ImGui::Text("BGM");
			ImGui::SameLine();
			ImGui::Text("SFX");

			//master vertical sliders
			ImGui::PushID(0);
			ImGui::PushStyleColor(ImGuiCol_FrameBg, (ImVec4)ImColor::HSV(0.f / 7.0f, 0.5f, 0.5f));
			ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, (ImVec4)ImColor::HSV(0.f / 7.0f, 0.6f, 0.5f));
			ImGui::PushStyleColor(ImGuiCol_FrameBgActive, (ImVec4)ImColor::HSV(0.f / 7.0f, 0.7f, 0.5f));
			ImGui::PushStyleColor(ImGuiCol_SliderGrab, (ImVec4)ImColor::HSV(0.f / 7.0f, 0.9f, 0.9f));
			ImGui::VSliderFloat("##master", ImVec2(50, 160), &floatValueMaster, 0.0f, 1.0f);
			Engine::SetChannelVolume(Engine::Channel::Master, floatValueMaster);
			ImGui::PopStyleColor(4);
			ImGui::PopID();

			ImGui::SameLine();

			//music vertical sliders
			ImGui::PushID(1);
			ImGui::PushStyleColor(ImGuiCol_FrameBg, (ImVec4)ImColor::HSV(1.f / 7.0f, 0.5f, 0.5f));
			ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, (ImVec4)ImColor::HSV(1.f / 7.0f, 0.6f, 0.5f));
			ImGui::PushStyleColor(ImGuiCol_FrameBgActive, (ImVec4)ImColor::HSV(1.f / 7.0f, 0.7f, 0.5f));
			ImGui::PushStyleColor(ImGuiCol_SliderGrab, (ImVec4)ImColor::HSV(1.f / 7.0f, 0.9f, 0.9f));
			ImGui::VSliderFloat("##bgm", ImVec2(50, 160), &floatValueBgm, 0.0f, 1.0f);
			Engine::SetChannelVolume(Engine::Channel::BGM, floatValueBgm);
			ImGui::PopStyleColor(4);
			ImGui::PopID();

			ImGui::SameLine();

			//reverb vertical sliders
			ImGui::PushID(2);
			ImGui::PushStyleColor(ImGuiCol_FrameBg, (ImVec4)ImColor::HSV(4.f / 7.0f, 0.5f, 0.5f));
			ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, (ImVec4)ImColor::HSV(4.f / 7.0f, 0.6f, 0.5f));
			ImGui::PushStyleColor(ImGuiCol_FrameBgActive, (ImVec4)ImColor::HSV(4.f / 7.0f, 0.7f, 0.5f));
			ImGui::PushStyleColor(ImGuiCol_SliderGrab, (ImVec4)ImColor::HSV(4.f / 7.0f, 0.9f, 0.9f));
			ImGui::VSliderFloat("##sfx", ImVec2(50, 160), &floatValueSfx, 0.0f, 1.0f);
			Engine::SetChannelVolume(Engine::Channel::SFX, floatValueSfx);
			ImGui::PopStyleColor(4);
			ImGui::PopID();

			ImGui::PopStyleVar();

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
	
	void AudioEditorPanel::FileContents(const std::filesystem::path& path, std::vector<std::string>& items)
	{
		// Open the file
		// Note that we have to use binary mode as we want to return a string
		// representing matching the bytes of the file on the file system.
		for (const auto& entry : std::filesystem::directory_iterator(path))
		{
			std::string const& fileNamestring = std::filesystem::relative(entry.path(), path).filename().string();
			//std::string const& animatorName = entry.path().string();
			//u64 lastOfSlash = animatorName.find_last_of("/\\") + 1;

			if (fileNamestring.find(".meta") != std::string::npos)
			{
				continue;
			}

			items.push_back(fileNamestring);
		}
	}
}

#endif