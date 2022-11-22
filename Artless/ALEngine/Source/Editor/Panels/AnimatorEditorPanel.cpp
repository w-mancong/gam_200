#include "pch.h"

#include "imgui.h"
#include "imgui_internal.h"
#ifdef EDITOR

namespace ALEngine::Editor
{
	namespace
	{
		const std::filesystem::path amimatorPath{ "Assets/Dev/Animator" };//base file 
		b8 popUpWindow{ false };
		Animator toSave{};
		f32 timer{};
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
		static b8 animatorText{ false }, animatorError{ false };
		static std::string animatorButtonStringName[2]{ "Create Animator##AnimatorPanelButton", "Close Animator##AnimatorPanelButton" };
		static u64 animatorButtonIndex{ 0 };

		ImGuiWindowFlags flag = 0;

		if (ImGui::BeginPopupModal("CreateClip", nullptr, ImGuiWindowFlags_MenuBar))
		{
			static const char* currentItem = nullptr;
			if (ImGui::BeginCombo("Animators##", currentItem)) // The second parameter is the label previewed before opening the combo.
			{
				for (size_t n = 0; n < items.size(); n++)
				{
					bool isSelected = (currentItem == items[n].c_str()); // You can store your selection however you want, outside or inside your objects
					if (ImGui::Selectable(items[n].c_str(), isSelected))
					{
						currentItem = items[n].c_str();
					}
					if (isSelected)
					{
						ImGui::SetItemDefaultFocus();   // You may set the initial focus when opening the combo (scrolling + for keyboard navigation support)
					}
				}
				ImGui::EndCombo();
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

				c8* buffer = const_cast<c8*>(toSave.animatorName.c_str());
				ImGui::InputTextWithHint("##AnimatorNameInput", "Animator Name", buffer, 256);
				toSave.animatorName = buffer;

				if (ImGui::Button("Create!##AnimatorPanelButton"))
				{
					if (!toSave.animatorName.empty())
					{
						ECS::SaveAnimator(toSave);
						toSave.animatorName = "";
						animatorError = animatorText = false;
						timer = 0.0f;
					}
					else
						animatorError = true;
				}
			}

			if (ImGui::Button(animatorButtonStringName[animatorButtonIndex].c_str()))//create animator button
			{
				if (!animatorText)
				{
					animatorText = true;
					animatorError = false;
				}
				else
				{
					timer = 0.0f;
					animatorError = false;
					animatorText = false;
				}
				(++animatorButtonIndex) %= 2;
			}

			ImGui::SameLine();

			if (animatorText)
			{
				ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
				ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
			}
			if (ImGui::Button("Create Clip##AnimatorPanelButton"))//create clip button
			{
				
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
				pOpen = false;
			}

			ImGui::EndPopup();
		}

		//if (ImGui::Begin("Animator Panel##", &pOpen))
		//{
		
			//if (ImGui::Button("close"))
			//{
				//pOpen = false;
			//}

			//static int item_type = 4;
            //const char* items[] = { "Item1", "Item2", "AAAA", "AAAB", "AABB", "ABBB", "ABBB" };



			//if (ImGui::Button("Create Clip##"))//create clip button
			//{
			//	popUpWindow = true;			
			//}

			//if (popUpWindow)
			//{
			//	ImGui::Begin("CreateClip");

			//	if (ImGui::Button("close##"))
			//	{
			//		popUpWindow = false;
			//	}
			//	ImGui::End();
			//}
		

			//ImGui::End();
		//}
		

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
			std::string const& animatorName = entry.path().string();
			u64 lastOfSlash = animatorName.find_last_of("/\\") + 1;

			items.push_back(animatorName.substr(lastOfSlash));
		}
	}
}
#endif