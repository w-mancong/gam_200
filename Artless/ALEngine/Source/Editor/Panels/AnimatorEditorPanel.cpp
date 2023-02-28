/*!
file: AudioEditorPanel.cpp
author: Chan Jie Ming Stanley
email: c.jiemingstanley\@digipen.edu
brief: This file contains the function declaration for AudioEditorPanel.
	   AudioEditorPanel handles the panel that display the create clip/animations panel
All content :copyright: 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#include "pch.h"

#if EDITOR
#include "imgui.h"
#include "imgui_internal.h"
#include "imgui/cpp/imgui_stdlib.h"

namespace ALEngine::Editor
{
	namespace
	{
		const std::filesystem::path animatorPath{ "Assets/Dev/Animator" };//base file 
		b8 popUpWindow{ false };
		Animator tempAnimator{};
		Animation tempAnimation{};
		f32 timer{};
		f32 clipNameErrorTimer{ 0.0f }, clipFilePathErrorTimer{ 0.0f }, clipAnimatorErrorTimer{ 0.0 };

		u64 constexpr FILE_BUFFER_SIZE{ 1024 };
	}

	std::unordered_map<std::string, Animator> AnimatorEditorPanel::animatorList{};

	AnimatorEditorPanel::AnimatorEditorPanel()
	{
		std::vector<std::string> items{};
		FileContents(animatorPath, items);
	}

	AnimatorEditorPanel::~AnimatorEditorPanel()
	{
	}

	void AnimatorEditorPanel::OnImGuiRender(b8& pOpen)
	{
		if (pOpen)
		{
			ImGui::OpenPopup("CreateClip");
		}

		std::vector<std::string> items{};
		FileContents(animatorPath, items);

		// used for creating animators
		static b8 animatorText{ false }, animatorError{ false };	
		static std::string animatorButtonStringName[2]{ "Create Animator##AnimatorPanelButton", "Close Animator##AnimatorPanelButton" };
		static u64 animatorButtonIndex{ 0 };

		auto ResetAnimatorValues = [](void)
		{
			animatorError = animatorText = false;
			animatorButtonIndex = 0;
			timer = 0.0f;
			tempAnimator.animatorName.clear();
			tempAnimator.animations.clear();
		};

		// used for creating clips
		static b8 clipText{ false }, clipAnimatorError{ false }, clipNameError{ false }, clipFilePathError{ false };
		static std::string clipButtonStringName[2]{ "Create Clip##AnimatorPanelButton", "Close Clip##AnimatorPanelButton" };
		static u64 clipButtonIndex{ 0 };
		static std::string clipWidth{ "128" }, clipHeight{ "128" }, clipTotalSample{ "8" };

		auto ResetClipValues = [](void)
		{
			clipAnimatorError = clipNameError = clipFilePathError = clipText = false;
			clipButtonIndex = 0;
			clipWidth = clipHeight = "128";
			clipTotalSample = "8";
			clipFilePathErrorTimer = clipNameErrorTimer = clipAnimatorErrorTimer = 0.0f;

			memset(tempAnimation.filePath, 0, sizeof(tempAnimation.filePath));
			memset(tempAnimation.clipName, 0, sizeof(tempAnimation.clipName));
		};

		static b8 arrowButtonPressed{ false };

		ImGuiWindowFlags flag = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoDocking
			| ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize;

		//begining of window
		ImGui::Begin("Animations##Animatior/Clip Creation", &pOpen, flag);

		std::string temp;

		if (clipText && clipAnimatorError)
		{
			clipAnimatorErrorTimer += Time::m_DeltaTime;
			if (clipAnimatorErrorTimer >= 3.0f)
			{
				clipAnimatorErrorTimer = 0.0f;
				clipAnimatorError = false;
			}
			ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Attach an animator before creating a clip!");
		}

		//ImVec2 pos = ImGui::GetCursorPos();
		static std::string currentAnimatorName{ "" };
		ImGui::BeginDisabled();
		ImGui::InputText("Animator##Animator Editor current item", &currentAnimatorName);
		ImGui::EndDisabled();

		ImGui::SetItemAllowOverlap();
		f32 inputTextWidth = ImGui::CalcItemWidth();
		ImGui::SameLine(inputTextWidth - 17.0f);

		f32 const crossWidth = animatorText || clipText ? 22.5f : 38.5f;
		f32 const listBoxHeightSpacing = animatorText || clipText ? 0.35f : 0.7f;

		if (ImGui::ArrowButton("##Animator Arrow Button", ImGuiDir_Down))
		{
			arrowButtonPressed = !arrowButtonPressed;
		}
		//custom begincombo for panel as imgui begincombo does not work the way needed for panel.
		if (arrowButtonPressed)
		{
			
			ImGui::BeginListBox("##Animator List Box", ImVec2(0.f, ImGui::GetContentRegionAvail().y * listBoxHeightSpacing));
			for (u64 i{}; i < items.size(); ++i)
			{
				std::string const& str = items[i] + "##" + std::to_string(i); 
				bool isSelected = (currentAnimatorName == items[i]); // You can store your selection however you want, outside or inside your objects	
				if (ImGui::Selectable(str.c_str(), &isSelected))
				{
					currentAnimatorName = items[i];
				}
				ImGui::SameLine(inputTextWidth - crossWidth);
				ImGui::SetItemAllowOverlap();
				ImGui::PushID(str.c_str());
				if (ImGui::SmallButton("X"))
				{
					std::string fileName = "Assets\\Dev\\Animator\\" + items[i];
					std::remove(fileName.c_str());
				}
				ImGui::PopID();
			}

			ImGui::EndListBox();
		}

		if (animatorText)
		{
			if (animatorError)//check if animator name is empty and if empty display user feedback error message
			{
				timer += Time::m_DeltaTime;
				if (timer >= 3.0f)
				{
					timer = 0.0f;
					animatorError = false;
				}
				ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Animator Name cannot be empty!");
			}

			c8* buffer = const_cast<c8*>(tempAnimator.animatorName.c_str());
			ImGui::InputTextWithHint("##AnimatorNameInput", "Animator Name", buffer, 256);
			tempAnimator.animatorName = buffer;
			ImGui::SameLine(); HelpMarker("Name of the animator");

			if (ImGui::Button("Create!##AnimatorPanelButton"))
			{
				if (!tempAnimator.animatorName.empty())
				{
					ECS::SaveAnimator(tempAnimator);
					ResetAnimatorValues();
				}
				else
					animatorError = true;
			}
		}
		else if (clipText)
		{
			if (clipNameError)//check if clip name is empty and if empty display user feedback error message
			{
				clipNameErrorTimer += Time::m_DeltaTime;
				if (clipNameErrorTimer >= 3.0f)
				{
					clipNameErrorTimer = 0.0f;
					clipNameError = false;
				}
				ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Animation clip name cannot be empty!");
			}

			c8* clipName = tempAnimation.clipName;
			ImGui::InputTextWithHint("##ClipNameInput", "Name of Clip", clipName, sizeof(tempAnimation.clipName));
			strcpy_s(tempAnimation.clipName, sizeof(tempAnimation.clipName), clipName);
			ImGui::SameLine(); HelpMarker("Name of animation clip");

			if (clipFilePathError)//check if file path of spritesheet is empty and if empty display user feedback error message
			{
				clipFilePathErrorTimer += Time::m_DeltaTime;
				if (clipFilePathErrorTimer >= 3.0f)
				{
					clipFilePathErrorTimer = 0.0f;
					clipFilePathError = false;
				}
				ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Source of spritesheet cannot be empty!");
			}
			c8* animationFilePath = tempAnimation.filePath;
			ImGui::InputTextWithHint("##ClipFilePathInput", "Spritesheet Source", animationFilePath, sizeof(tempAnimation.filePath), ImGuiInputTextFlags_ReadOnly);

			// Drag Drop for Selectable
			if (ImGui::BeginDragDropTarget())
			{
				// Payload flag
				ImGuiDragDropFlags payload_flag{ 0 };
				payload_flag |= ImGuiDragDropFlags_AcceptNoDrawDefaultRect;

				// Get Drag and Drop Payload
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ASSET_ITEM", payload_flag))
				{
					// Get filepath
					size_t fileLen;	c8 filePath[FILE_BUFFER_SIZE];
					wcstombs_s(&fileLen, filePath, FILE_BUFFER_SIZE, (const wchar_t*)payload->Data, payload->DataSize);

					// Check if image (png or jpg)
					std::string fileString = filePath;
					if (fileString.find(".png") != std::string::npos)
					{
						strcpy_s(tempAnimation.filePath, sizeof(tempAnimation.filePath), filePath);
					}
					else
					{
						AL_CORE_ERROR("A .png file is required!");
					}
				}
				ImGui::EndDragDropTarget();
			}
			ImGui::SameLine(); HelpMarker("Path to the spritesheet. (Click and drag from assets panel)");

			ImGui::InputTextWithHint("##Clip Width", "Clip Width", &clipWidth, ImGuiInputTextFlags_CharsDecimal);
			ImGui::SameLine(); HelpMarker("Value used to determine how wide each sample from the spritesheet should be");

			ImGui::InputTextWithHint("##Clip Height", "Clip Height", &clipHeight, ImGuiInputTextFlags_CharsDecimal);
			ImGui::SameLine(); HelpMarker("Value used to determine how tall each sample from the spritesheet should be");

			ImGui::InputTextWithHint("##Total Sprite", "Total Sprites", &clipTotalSample, ImGuiInputTextFlags_CharsDecimal);
			ImGui::SameLine(); HelpMarker("Total number of sample there are in the spritesheet");

			if (ImGui::Button("Create!##Create the animation clip"))
			{
				// Did not select any animator name
				if (currentAnimatorName == "")
				{
					clipAnimatorError = true;
				}
				// tempAnimation clip name is empty
				if (!strcmp(tempAnimation.clipName, ""))
				{
					clipNameError = true;
				}
				// tempAnimation file path is empty
				if (!strcmp(tempAnimation.filePath, ""))
				{
					clipFilePathError = true;
				}

				if (!clipNameError && !clipFilePathError)
				{
					s32 width = atoi(clipWidth.c_str());
					s32 height = atoi(clipHeight.c_str());
					u32 totalSample = static_cast<u32>(atoi(clipTotalSample.c_str()));

					Animator animator = ECS::CreateAnimator(currentAnimatorName.c_str());
					ECS::CreateAnimationClip(tempAnimation.filePath, tempAnimation.clipName, width, height, 12, totalSample);
					ECS::AddAnimationToAnimator(animator, tempAnimation.clipName);
					ECS::SaveAnimator(animator);

					ResetClipValues();
				}
			}
		}

		if (clipText)
		{
			ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
			ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
		}
		if (ImGui::Button(animatorButtonStringName[animatorButtonIndex].c_str()))//create animator button
		{
			if (!animatorText)
			{
				animatorText = true;
			}
			else
			{
				timer = 0.0f;
				animatorError = false;
				animatorText = false;
			}
			animatorError = false;
			(++animatorButtonIndex) %= 2;
		}
		if (clipText)
		{
			ImGui::PopStyleVar();
			ImGui::PopItemFlag();
		}

		ImGui::SameLine();

		if (animatorText)
		{
			ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
			ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
		}
		if (ImGui::Button(clipButtonStringName[clipButtonIndex].c_str()))//create clip button
		{
			if (!clipText)
			{
				clipText = true;
			}
			else
			{
				clipFilePathErrorTimer = clipNameErrorTimer = 0.0f;
				clipText = false;
			}
			clipAnimatorError = clipNameError = clipFilePathError = false;
			(++clipButtonIndex) %= 2;
		}
		if (animatorText)
		{
			ImGui::PopStyleVar();
			ImGui::PopItemFlag();
		}

		ImGui::End();

		if (!pOpen)
		{
			ResetAnimatorValues();
			ResetClipValues();
		}
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

	std::unordered_map<std::string, Animator> const& AnimatorEditorPanel::GetListOfAnimators(void)
	{
		return animatorList;
	}

	void AnimatorEditorPanel::FileContents(const std::filesystem::path& path, std::vector<std::string>& items)
	{
		// Open the folder directory and push the names of file into a vector
		for (const auto& entry : std::filesystem::directory_iterator(path))
		{
			std::string const& fileNamestring = std::filesystem::relative(entry.path(), path).filename().string();

			items.push_back(fileNamestring);
			if (animatorList.count(fileNamestring) == 0)
				animatorList[fileNamestring] = ECS::CreateAnimator(fileNamestring.c_str());
		}
	}
}
#endif