/*!
file:	AnimatorPanel.cpp
author: Lucas Nguyen 
email:	l.nguyen\@digipen.edu
brief:	This file contains function definitions for the AnimatorPanel class.
		The AnimatorPanel class contains an Animator Editor that edits the number of
		frames for each frame within the Animation

		All content � 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#include "pch.h"

#if EDITOR
#define SELECTOR_IMAGE_PATH "Assets/Images/InitialTile_v04.png"

namespace ALEngine::Editor
{
	namespace
	{
		u64 SelectorImageGuid{};
		const u32 MAX_SPRITES_ROW{ 4 };
	}

	AnimatorPanel::AnimatorPanel(void)
	{
		m_SelectedAnimator = AnimatorEditorPanel::GetListOfAnimators().begin()->second;
		m_SelectedAnimation = m_SelectedAnimator.animations.begin()->second;
		m_SelectedAnimatorIndex = 0;
		m_SelectedAnimationIndex = 0;
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
		if (m_SelectedAnimation.filePath != "" && !m_SelectedAnimator.animations.empty()) 
		{
			ImVec2 animationsArea = ImVec2(ImGui::GetContentRegionAvail().x * 0.6f, ImGui::GetContentRegionAvail().x * 0.6f);

			// Up
			if (Input::KeyTriggered(KeyCode::Up))
			{
				u32 nextY{ (m_SelectedFrame.y + 1) * MAX_SPRITES_ROW + m_SelectedFrame.x };
				m_SelectedFrame.y = nextY > (m_SelectedAnimation.totalSprites - 1) ? m_SelectedFrame.y : m_SelectedFrame.y + 1;
			}

			// Down
			if(Input::KeyTriggered(KeyCode::Down))
				m_SelectedFrame.y = m_SelectedFrame.y <= 0 ? 0 : m_SelectedFrame.y - 1;

			// Right
			if (Input::KeyTriggered(KeyCode::Right))
			{
				u32 nextX{ (m_SelectedFrame.y) * MAX_SPRITES_ROW + (m_SelectedFrame.x + 1) };
				m_SelectedFrame.x = (nextX < (m_SelectedAnimation.totalSprites) && m_SelectedFrame.x + 1 < MAX_SPRITES_ROW) ? m_SelectedFrame.x + 1 :  m_SelectedFrame.x;
			}

			// Left
			if (Input::KeyTriggered(KeyCode::Left))
				m_SelectedFrame.x = m_SelectedFrame.x <= 0 ? 0 : m_SelectedFrame.x - 1;

			if (ImGui::BeginChild("##AnimatorPanel_AnimationsArea", animationsArea, true))
			{
				// Render the Animation
				std::string fp = m_SelectedAnimation.filePath;

				Guid id = Engine::AssetManager::Instance()->GetGuid(fp);
				u64 tex = (u64)Engine::AssetManager::Instance()->GetButtonImage(id);
				ImGui::Image(reinterpret_cast<ImTextureID>(tex), { animationsArea.x * 0.9f, animationsArea.y * 0.9f }, { 0, 1 }, { 1, 0 });

				// X and Y size is the same
				ImVec2 selectorSize = { animationsArea.x * 0.9f / static_cast<f32>(m_SelectedAnimation.width), animationsArea.x * 0.9f / static_cast<f32>(m_SelectedAnimation.height) };
				selectorSize = { (animationsArea.x * 0.9f) / 4.f, (animationsArea.x * 0.9f) / 4.f };

				id = Engine::AssetManager::Instance()->GetGuid(SELECTOR_IMAGE_PATH);
				SelectorImageGuid = (u64)Engine::AssetManager::Instance()->GetButtonImage(id);

				ImVec2 selectorPos = { ImGui::GetCursorScreenPos().x + selectorSize.x * m_SelectedFrame.x,
					ImGui::GetCursorScreenPos().y - selectorSize.y * (m_SelectedFrame.y + 1)};

				ImGui::GetWindowDrawList()->AddImage(reinterpret_cast<ImTextureID>(SelectorImageGuid), selectorPos,
					{ selectorPos.x + selectorSize.x, selectorPos.y + selectorSize.y }, ImVec2(0, 1), ImVec2(1, 0));

				ImGui::EndChild();

				ImGui::SameLine();
			}
		}

		auto animator_list = AnimatorEditorPanel::GetListOfAnimators();
		//ECS::ChangeAnimationFramesCount();
		if (ImGui::BeginChild("##AnimatorPanel_InfoArea", ImGui::GetContentRegionAvail()))
		{
			const char* animator_preview = m_SelectedAnimator.animatorName.empty() ? animator_list.begin()->first.c_str() : m_SelectedAnimator.animatorName.c_str();
			if (ImGui::BeginCombo("Animator##AnimatorPanel", animator_preview))
			{
				u32 counter{ 0 };
				for (auto i : animator_list)
				{
					if (ImGui::Selectable(i.first.c_str(), m_SelectedAnimatorIndex == counter))
					{
						m_SelectedAnimatorIndex = counter;
						m_SelectedAnimator = i.second;

						// Set default to first animations
						m_SelectedAnimation = m_SelectedAnimator.animations.begin()->second;
						m_SelectedAnimationIndex = 0;
					}
				}

				ImGui::EndCombo();
			}

			if (m_SelectedAnimation.clipName != "")
			{
				const char* animation_preview = m_SelectedAnimation.clipName;
				if (ImGui::BeginCombo("Animation##AnimatorPanel", animation_preview))
				{
					u32 counter{ 0 };
					for (auto i : m_SelectedAnimator.animations)
					{
						if (ImGui::Selectable(i.first.c_str(), m_SelectedAnimationIndex == counter))
						{
							m_SelectedAnimation = i.second;
							m_SelectedAnimationIndex = counter;
							m_SelectedFrame = { 0, 0 };
						}
					}

					ImGui::EndCombo();
				}
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