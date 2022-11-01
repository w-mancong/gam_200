#include "pch.h"

#include "imgui.h"
#include "imgui_internal.h"

namespace ALEngine::Editor
{
	GamePanel::GamePanel(void)
	{
	}

	GamePanel::~GamePanel(void)
	{
	}

	void GamePanel::OnImGuiRender(void)
	{
		// Set position & size of Game Scene
		//ImGui::SetNextWindowPos(ImVec2(0.f, 0.f));

		// Begin ImGui Panel
		if (!ImGui::Begin("Game"))
		{
			ImGui::End();
			return;
		}

		if (m_SceneWidth != ImGui::GetContentRegionAvail().x)
			m_SceneWidth = ImGui::GetContentRegionAvail().x;
		if (m_SceneHeight != ImGui::GetContentRegionAvail().y)
			m_SceneHeight = ImGui::GetContentRegionAvail().y;

		u64 tex = (u64)ECS::GetFBTexture();
		ImGui::Image((void*)tex, ImGui::GetContentRegionAvail(), ImVec2(0, 1), ImVec2(1, 0));

		// End ImGui Panel
		ImGui::End();
	}

	Math::Vec2 GamePanel::GetMouseWorldPos()
	{
		ImGuiStyle style = ImGui::GetStyle();
		// Set mouse position
		m_ImGuiMousePos = ImGui::GetMousePos();

		// Find the Editor panel
		ImGuiWindow* win = ImGui::FindWindowByName("Game");

		// Get panel position
		m_ImGuiPanelPos = win->DC.CursorPos;

		// Convert mouse pos from ImGui space to screen space		
		Math::vec4 mousePos{ m_ImGuiMousePos.x - m_ImGuiPanelPos.x,
			-m_ImGuiMousePos.y + m_ImGuiPanelPos.y - style.WindowPadding.y * 0.75f, 0.f, 1.f };

		// Get NDC coords of mouse pos
		mousePos.x = 2.f * (mousePos.x / m_SceneWidth) - 1.f;
		mousePos.y = 2.f * (mousePos.y / m_SceneHeight) - 1.f;
		
		// Check if within range of scene
		if (mousePos.x >= -1.f && mousePos.x <= 1.f &&
			mousePos.y >= -1.f && mousePos.y <= 1.f)
		{
			using namespace Math;

			// Convert mouse pos from screen space to world space
			// Projection mtx
			Mat4 inv_proj = ECS::GetProjection();
			inv_proj = mat4::InverseT(inv_proj);

			// View matrix
			Mat4 inv_view = ECS::GetView();
			inv_view = Mat4::InverseT(inv_view);

			mousePos = inv_view * inv_proj * mousePos;

			return Math::Vec2(mousePos.x, mousePos.y);
		}

		return Math::Vec2(std::numeric_limits<f32>::max(), std::numeric_limits<f32>::max());
	}
}