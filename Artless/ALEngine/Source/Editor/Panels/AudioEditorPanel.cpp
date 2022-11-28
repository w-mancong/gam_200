/*!
file: AudioEditorPanel.cpp
author: Chan Jie Ming Stanley
email: c.jiemingstanley\@digipen.edu
brief: This file contains the function declaration for AudioEditorPanel.
	   AudioEditorPanel handles the panel that display the audio volume mixer panel
All content :copyright: 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#include "pch.h"

#if EDITOR
#include "imgui.h"
#include "imgui_internal.h"


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
			Engine::SetChannelVolume(Engine::Channel::Master, floatValueMaster); //set Master volume
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
			Engine::SetChannelVolume(Engine::Channel::BGM, floatValueBgm); //set BGM volume
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
			Engine::SetChannelVolume(Engine::Channel::SFX, floatValueSfx); //set SFX volume
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

}

#endif