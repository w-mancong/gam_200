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