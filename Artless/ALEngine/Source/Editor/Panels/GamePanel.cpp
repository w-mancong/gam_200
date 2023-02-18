/*!
file:	GamePanel.cpp
author: Lucas Nguyen
email:	l.nguyen\@digipen.edu
brief:	This file contains function definitions for the GamePanel class.
		The GamePanel class contains information and functions necessary for
		the Game Panel of the editor to be displayed.

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#include "pch.h"
#if EDITOR

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
		// Set flags 
		ImGuiWindowFlags flag = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse;
		
		// Begin ImGui Panel
		if (!ImGui::Begin("Game", nullptr, flag))
		{
			ImGui::End();
			return;
		}

		if (m_PanelWidth != ImGui::GetContentRegionAvail().x)
			m_PanelWidth = ImGui::GetContentRegionAvail().x;
		if (m_PanelHeight != ImGui::GetContentRegionAvail().y)
			m_PanelHeight = ImGui::GetContentRegionAvail().y;

		u64 tex = (u64)ECS::GetFBTexture();
		ImGui::Image((void*)tex, ImGui::GetContentRegionAvail(), ImVec2(0, 1), ImVec2(1, 0));

		// End ImGui Panel
		ImGui::End();
	}

	Math::Vec2 GamePanel::GetMouseWorldPos(void)
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
		mousePos.x = 2.f * (mousePos.x / m_PanelWidth) - 1.f;
		mousePos.y = 2.f * (mousePos.y / m_PanelHeight) - 1.f;
		
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
	
	Math::Vec2 GamePanel::GetMousePosWRTPanel(void)
	{
		ImGuiStyle style = ImGui::GetStyle();
		// Set mouse position
		m_ImGuiMousePos = ImGui::GetMousePos();

		// Find the Game panel
		ImGuiWindow* win = ImGui::FindWindowByName("Game");

		// Get panel position
		m_ImGuiPanelPos = win->DC.CursorPos;

		// Convert mouse pos from ImGui space to screen space		
		Math::vec4 mousePos{ m_ImGuiMousePos.x - m_ImGuiPanelPos.x,
			-m_ImGuiMousePos.y + m_ImGuiPanelPos.y - style.WindowPadding.y * 0.75f, 0.f, 1.f };

		return Math::Vec2(mousePos.x, mousePos.y);

		//// Get NDC coords of mouse pos
		//mousePos.x = 2.f * (mousePos.x / m_SceneWidth) - 1.f;
		//mousePos.y = 2.f * (mousePos.y / m_SceneHeight) - 1.f;

		//// Check if within range of scene
		//if (mousePos.x >= -1.f && mousePos.x <= 1.f &&
		//	mousePos.y >= -1.f && mousePos.y <= 1.f)
		//{	return Math::Vec2(mousePos.x, mousePos.y);	}

		//return Math::Vec2(std::numeric_limits<f32>::max(), std::numeric_limits<f32>::max());
	}

	Math::Vec2 GamePanel::GetVecScreenPos(Math::Vec2 pos)
	{
		using namespace Math;

		// Find the Game panel
		ImGuiWindow* win = ImGui::FindWindowByName("Game");

		// Get panel position
		m_ImGuiPanelPos = win->DC.CursorPos;

		// Convert pos from world space to screen space
		// Projection mtx
		Mat4 proj = ECS::GetProjection();

		// View matrix
		Mat4 view = ECS::GetView();

		Vec4 position = { pos.x, pos.y, 0.f, 1.f };

		// Pos
		position = proj * view * position;

		// Check if within range of panel
		if (position.x < -1.f || position.x > 1.f ||
			position.y < -1.f || position.y > 1.f)
		{
			return Math::Vec2(std::numeric_limits<f32>::max(), std::numeric_limits<f32>::max());
		}

		position.x = (position.x + 1.f) * (0.5f * m_PanelWidth);
		position.y = (position.y + 1.f) * (0.5f * m_PanelHeight);

		AL_CORE_CRITICAL("{}, {}", position.x, position.y);
		return Math::Vec2(position.x, position.y);
	}

	void GamePanel::SetPanelMin(Math::Vec2 min)
	{
		m_PanelMin = ImVec2(min.x, min.y);
	}
	
	void GamePanel::SetDefaults(Math::Vec2 pos, Math::Vec2 size)
	{
		m_DefaultPos = ImVec2(pos.x, pos.y);
		m_DefaultSize = ImVec2(size.x, size.y);
	}
	
	f64 GamePanel::GetGamePanelWidth(void)
	{
		return m_PanelWidth;
	}
	
	f64 GamePanel::GetGamePanelHeight(void)
	{
		return m_PanelHeight;
	}
}

#endif