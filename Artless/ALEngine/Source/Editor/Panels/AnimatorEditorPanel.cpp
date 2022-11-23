#include "pch.h"

#include "imgui.h"
#include "imgui_internal.h"
#include "imgui/cpp/imgui_stdlib.h"
#ifdef EDITOR

namespace ALEngine::Editor
{
	namespace
	{
		const std::filesystem::path amimatorPath{ "Assets/Dev/Animator" };//base file 
		b8 popUpWindow{ false };
		Animator tempAnimator{};
		Animation tempAnimation{};
		f32 timer{};

		u64 constexpr FILE_BUFFER_SIZE{ 1024 };
	}

	AnimatorEditorPanel::AnimatorEditorPanel()
	{
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
		FileContents(amimatorPath, items);

		// used for creating animators
		static b8 animatorText{ false }, animatorError{ false };	
		static std::string animatorButtonStringName[2]{ "Create Animator##AnimatorPanelButton", "Close Animator##AnimatorPanelButton" };
		static u64 animatorButtonIndex{ 0 };

		// used for creating clips
		static b8 clipText{ false }, clipError{ false };			
		static std::string clipButtonStringName[2]{ "Create Clip##AnimatorPanelButton", "Close Clip##AnimatorPanelButton" };
		static u64 clipButtonIndex{ 0 };

		static b8 arrowButtonPressed{ false };

		ImGuiWindowFlags flag = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoDocking;

		ImGui::Begin("##Animatior/Clip Creation", &pOpen, flag);

		std::string temp;

		//ImVec2 pos = ImGui::GetCursorPos();
		static std::string currentItem{ "" };
		ImGui::InputText("##Animator Editor current item", &currentItem);
		ImGui::SetItemAllowOverlap();
		f32 width = ImGui::CalcItemWidth();
		ImGui::SameLine(width - 17.0f);

		if (ImGui::ArrowButton("##Animator Arrow Button", ImGuiDir_Down))
		{
			arrowButtonPressed = !arrowButtonPressed;
		}
		if (arrowButtonPressed)
		{
			
			ImGui::BeginListBox("##Animator List Box", ImVec2(0.f, ImGui::GetContentRegionAvail().y * 0.5f));
			for (u64 i{}; i < items.size(); ++i)
			{
				std::string const& str = items[i] + "##" + std::to_string(i); 
				bool isSelected = (currentItem == items[i]); // You can store your selection however you want, outside or inside your objects	
				if (ImGui::Selectable(str.c_str(), &isSelected))
				{
					currentItem = items[i];
				}
				ImGui::SameLine(width - 22.5f);
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
			if (animatorError)
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

			if (ImGui::Button("Create!##AnimatorPanelButton"))
			{
				if (!tempAnimator.animatorName.empty())
				{
					ECS::SaveAnimator(tempAnimator);
					tempAnimator.animatorName = "";
					animatorError = animatorText = false;
					timer = 0.0f;
				}
				else
					animatorError = true;
			}
		}
		else if (clipText)
		{
			if (clipError)
			{
				timer += Time::m_DeltaTime;
				if (timer >= 3.0f)
				{
					timer = 0.0f;
					animatorError = false;
				}
				ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Attach an animator before creating a clip!");
			}

			c8* clipName = tempAnimation.clipName;
			ImGui::InputTextWithHint("##ClipNameInput", "Name of Clip", clipName, sizeof(tempAnimation.clipName));
			strcpy_s(tempAnimation.clipName, sizeof(tempAnimation.clipName), clipName);

			c8* filePath = tempAnimation.filePath;
			ImGui::InputTextWithHint("##ClipFilePathInput", "Spritesheet Source", filePath, sizeof(tempAnimation.filePath), ImGuiInputTextFlags_ReadOnly);

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
				timer = 0.0f;
				clipText = false;
			}
			clipError = false;
			(++clipButtonIndex) %= 2;
		}
		if (animatorText)
		{
			ImGui::PopStyleVar();
			ImGui::PopItemFlag();
		}

		if (ImGui::Button("close"))
		{
			animatorError = animatorText = false;
			animatorButtonIndex = 0;

			clipError = clipText = false;
			clipButtonIndex = 0;

			tempAnimator.animatorName.clear();
			tempAnimator.animations.clear();

			pOpen = false;
		}

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

	void AnimatorEditorPanel::FileContents(const std::filesystem::path& path, std::vector<std::string>& items)
	{
		// Open the file
		// Note that we have to use binary mode as we want to return a string
		// representing matching the bytes of the file on the file system.
		//std::string resultstring;
		for (const auto& entry : std::filesystem::directory_iterator(path))
		{
			//std::string const& animatorName = entry.path().string();
			//u64 lastOfSlash = animatorName.find_last_of("/\\") + 1;

			//items.push_back(animatorName.substr(lastOfSlash));

			std::string const& fileNamestring = std::filesystem::relative(entry.path(), path).filename().string();

			items.push_back(fileNamestring);
		}
	}
}
#endif