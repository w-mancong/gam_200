#include "pch.h"

#include "imgui.h"
#include "imgui_internal.h"

#ifdef EDITOR

namespace ALEngine::Editor
{
	AudioEditorPanel::AudioEditorPanel()
	{
	}

	AudioEditorPanel::~AudioEditorPanel()
	{
	}

	void AudioEditorPanel::OnImGuiRender(void)
	{
		ImGui::Begin("Audio Panel");

        const float spacing = 4;

		static f32 float_value_master = 0.f;
		static f32 float_value_music = 0.f;
		static f32 float_value_reverb = 0.f;
		static f32 float_value_effects= 0.f;

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

		ImGui::End();
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