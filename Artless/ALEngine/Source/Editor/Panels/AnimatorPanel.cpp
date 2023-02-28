/*!
file:	AnimatorPanel.cpp
author: Lucas Nguyen 
email:	l.nguyen\@digipen.edu
brief:	This file contains function definitions for the AnimatorPanel class.
		The AnimatorPanel class contains an Animator Editor that edits the number of
		frames for each frame within the Animation

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#include "pch.h"

#if EDITOR
#define SELECTOR_IMAGE_PATH "Assets/Images/InitialTile_v04.png"

namespace ALEngine::Editor
{
	namespace
	{
		u64 SelectorImageGuid{};
		const std::filesystem::path AnimatorPath{ "Assets/Dev/Animator" };
	}

	AnimatorPanel::AnimatorPanel(void)
	{
	}

	AnimatorPanel::~AnimatorPanel(void)
	{
	}

	void AnimatorPanel::OnImGuiRender(void)
	{
		
		// Set constraints
		ImGui::SetNextWindowSizeConstraints(ImVec2(static_cast<f32>(ALEditor::Instance()->GetSceneWidth()),
			static_cast<f32>(ALEditor::Instance()->GetSceneHeight())), ImGui::GetMainViewport()->WorkSize);

		ImGuiWindowFlags winFlags{ 0 };

		if (!ImGui::Begin("Animator Panel", &m_PanelIsOpen, winFlags))
		{
			ImGui::End();
			return;
		}

		// Updates!
		UpdateMenuBar();
		Update();


		ImGui::End();
	}

	void AnimatorPanel::SetSelectedAnimator(Animator& animatorSelected)
	{
		m_SelectedAnimator = animatorSelected;
	}

	void AnimatorPanel::SetSelectedAnimation(Animation& animationSelected)
	{
		m_SelectedAnimation = animationSelected;
	}
	
	void AnimatorPanel::SetPanelIsOpen(b8 panelIsOpen)
	{
		m_PanelIsOpen = panelIsOpen;
	}
	
	b8 AnimatorPanel::GetPanelIsOpen(void)
	{
		return m_PanelIsOpen;
	}
	
	void AnimatorPanel::SetPanelMin(Math::Vec2 min)
	{
		m_PanelMin = { min.x, min.y };
	}

	void AnimatorPanel::Update(void)
	{
		// Check if no animations or animation has no filepath
	//	if (m_SelectedAnimation.filePath != "" && !m_SelectedAnimator.animations.empty()) 
		{
			ImVec2 animationsArea = ImVec2(ImGui::GetContentRegionAvail().x * 0.6f, ImGui::GetContentRegionAvail().y);

			if (ImGui::BeginChild("##AnimatorPanel_AnimationsArea", animationsArea, true))
			{
				// Render the Animation
				std::string fp = m_SelectedAnimation.filePath;

				Guid id = Engine::AssetManager::Instance()->GetGuid("Assets\\Images\\Spritesheet_Guard_Hurt.png");
				u64 tex = (u64)Engine::AssetManager::Instance()->GetButtonImage(id);
				ImGui::Image(reinterpret_cast<ImTextureID>(tex), { animationsArea.x * 0.9f, animationsArea.y * 0.9f }, { 0, 1 }, { 1, 0 });

				// X and Y size is the same
				ImVec2 selectorSize = { animationsArea.x * 0.9f / static_cast<f32>(m_SelectedAnimation.width), animationsArea.x * 0.9f / static_cast<f32>(m_SelectedAnimation.height) };
				selectorSize = { (animationsArea.x * 0.9f) / 4.f, (animationsArea.x * 0.9f) / 4.f };

				id = Engine::AssetManager::Instance()->GetGuid(SELECTOR_IMAGE_PATH);
				SelectorImageGuid = (u64)Engine::AssetManager::Instance()->GetButtonImage(id);

				ImVec2 selectorPos = { ImGui::GetCursorScreenPos().x, 
					ImGui::GetCursorScreenPos().y - selectorSize.y * 4.f};

				ImGui::GetWindowDrawList()->AddImage(reinterpret_cast<ImTextureID>(SelectorImageGuid), selectorPos,
					{ selectorPos.x + selectorSize.x, selectorPos.y + selectorSize.y }, ImVec2(0, 1), ImVec2(1, 0));

				ImGui::EndChild();

				ImGui::SameLine();
			}
		}

		std::vector<std::string> items{};
		for (const auto& entry : std::filesystem::directory_iterator(AnimatorPath))
		{
			//std::string const& animatorName = entry.path().string();
			//u64 lastOfSlash = animatorName.find_last_of("/\\") + 1;

			//items.push_back(animatorName.substr(lastOfSlash));

			std::string const& fileNamestring = std::filesystem::relative(entry.path(), AnimatorPath).filename().string();

			items.push_back(fileNamestring);
		}

		if (ImGui::BeginChild("##AnimatorPanel_InfoArea", ImGui::GetContentRegionAvail()))
		{
			const char* animator_preview = m_SelectedAnimator.animatorName.empty() ? items.begin()->c_str() : m_SelectedAnimator.animatorName.c_str();
			if (ImGui::BeginCombo("Animator##AnimatorPanel", animator_preview))
			{
				for (u32 i{ 0 }; i < items.size(); ++i)
				{
					if (ImGui::Selectable(items[i].c_str(), m_SelectedAnimatorIndex == i))
					{
						m_SelectedAnimatorIndex = i;
						//m_SelectedAnimator = ;
					}
				}

				ImGui::EndCombo();
			}



			ImGui::EndChild();
		}
	}
	
	void AnimatorPanel::UpdateMenuBar(void)
	{
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("Setting##AnimatorPanelMenuBar"))
			{
				// Save
				if (ImGui::MenuItem("Save##AnimatorPanelMenuBar"))
				{
				}

				// Save As
				if (ImGui::MenuItem("Save As...##AnimatorPanelMenuBar"))
				{
				}

				// Open File
				if (ImGui::MenuItem("Open File##AnimatorPanelMenuBar"))
				{

				}
				ImGui::EndMenu();
			}
		}
	}
}

#endif