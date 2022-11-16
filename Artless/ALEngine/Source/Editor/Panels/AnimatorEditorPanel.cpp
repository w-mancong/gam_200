#include "pch.h"

#include "imgui.h"
#include "imgui_internal.h"
#ifdef EDITOR

namespace ALEngine::Editor
{

	AnimatorEditorPanel::AnimatorEditorPanel()
	{
	}

	AnimatorEditorPanel::~AnimatorEditorPanel()
	{
	}

	void AnimatorEditorPanel::OnImGuiRender(void)
	{
		ImGui::Begin("Animator Panel");

		static int item_type = 4;
		const char* items[] = { "Item1", "Item2", "AAAA", "AAAB", "AABB", "ABBB", "ABBB" };

		ImGui::Combo("Testing", &item_type, items, IM_ARRAYSIZE(items), IM_ARRAYSIZE(items));

		ImGui::Button("Create Clip");

		ImGui::End();
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

}
#endif