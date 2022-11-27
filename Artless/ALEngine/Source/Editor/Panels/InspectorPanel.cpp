/*!
file:	InspectorPanel.cpp
author: Lucas Nguyen
email:	l.nguyen\@digipen.edu
brief:	This file contains function definitions for the InspectorPanel class.
		The InspectorPanel class contains information and functions necessary for
		the Inspector Panel of the editor to be displayed.

		All content � 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#include "pch.h"

#if EDITOR

#include "imgui_internal.h"

namespace ALEngine::Editor
{
	namespace
	{
		// Commands namespace
		using namespace Commands;

		// File buffer size
		const u32 FILE_BUFFER_SIZE{ 1024 };

		enum class TRANSFORM_MODE {
			TRANSLATE = 0,
			ROTATE,
			SCALE
		};
	}

	InspectorPanel::InspectorPanel(void)
	{
		m_SelectedEntity = ECS::MAX_ENTITIES;
		m_SelectedComponent = InspectorComponents::InComp_Total;
	}

	InspectorPanel::~InspectorPanel(void)
	{
	}

	void InspectorPanel::OnImGuiRender(void)
	{
		// Set constraints
		ImGui::SetNextWindowSizeConstraints(m_PanelMin, ImGui::GetMainViewport()->WorkSize);

		// Begin ImGui
		if (!ImGui::Begin("Inspector"))
		{
			ImGui::End();
			return;
		}

		// Exit if no entity
		if (!HasSelectedEntity())
		{
			/*ImGui::NewLine();
			ImVec2 textSize = ImGui::CalcTextSize("Click on an Entity to view it's components");
			ImGui::SameLine((ImGui::GetWindowContentRegionMax().x * 0.5f) - (textSize.x * 0.5f));*/
			ImGui::AlignTextToFramePadding();
			ImGui::TextWrapped("Click on an Entity to view it's components");
			ImGui::End();
			return;
		}

		// Inspector Menu
		InspectorMenu();

		// Window size
		ImVec2 winSize = ImGui::GetWindowSize();

		// Check if entity has EntityData component
		if (Coordinator::Instance()->HasComponent<EntityData>(m_SelectedEntity))
			DisplayEntityData();

		// Check if has transform component
		if (Coordinator::Instance()->HasComponent<Transform>(m_SelectedEntity))
			DisplayTransform();

		// Check if there is sprite component
		if (Coordinator::Instance()->HasComponent<Sprite>(m_SelectedEntity))
			DisplaySprite();

		// Check if there is sprite component
		if (Coordinator::Instance()->HasComponent<Rigidbody2D>(m_SelectedEntity))
			DisplayRigidBody();

		// Check if there is sprite component
		if (Coordinator::Instance()->HasComponent<Collider2D>(m_SelectedEntity))
			DisplayCollider();

		// Check if there is sprite component
		if (Coordinator::Instance()->HasComponent<ParticleProperties>(m_SelectedEntity))
			DisplayParticleProperty();

		// Check if there is text component
		if (Coordinator::Instance()->HasComponent<Text>(m_SelectedEntity))
			DisplayTextProperty();

		//// Check if there is Audio component
		//if (Coordinator::Instance()->HasComponent<______>(m_SelectedEntity))
		//	DisplayAudio();
		// Check if there is Audio component
		if (Coordinator::Instance()->HasComponent<Engine::AudioSource>(m_SelectedEntity))
			DisplayAudio();

		//// Check if there is Animator component
		//if (Coordinator::Instance()->HasComponent<______>(m_SelectedEntity))
		//	DisplayAnimator();

		//// Check if there is Script component
		//if (Coordinator::Instance()->HasComponent<EntityScript>(m_SelectedEntity))
		//	DisplayEntityScript();

		// Add component button
		AddComponentButton();

		ImGui::End();
	}

	void InspectorPanel::SetSelectedEntity(ECS::Entity setter)
	{
		m_SelectedEntity = setter;
	}

	const ECS::Entity InspectorPanel::GetSelectedEntity(void)
	{
		return m_SelectedEntity;
	}

	b8 InspectorPanel::HasSelectedEntity(void)
	{
		return !(m_SelectedEntity == ECS::MAX_ENTITIES);
	}

	void InspectorPanel::InspectorMenu(void)
	{
		if (ImGui::BeginMenuBar())
		{
			// File
			if (ImGui::BeginMenu("Add"))
			{
				ImGui::EndMenu();
			}

			// Edit
			if (ImGui::BeginMenu("Remove"))
			{
				ImGui::EndMenu();
			}

			ImGui::EndMainMenuBar();
		}
	}

	void InspectorPanel::DisplayEntityData(void)
	{
		ImGui::Text("Entity Data");

		// Get entity data
		EntityData& data = Coordinator::Instance()->GetComponent<EntityData>(m_SelectedEntity);

		// Entity active
		if (ImGui::Checkbox("##active", &data.selfActive))
		{
			Tree::BinaryTree const& sceneGraph = ECS::GetSceneGraph(0);
			Tree::BinaryTree::NodeData const& node = sceneGraph.GetMap()[m_SelectedEntity];

			data.active = data.selfActive;
			sceneGraph.SetParentChildActive(node, data.active);
		}

		ImGui::SameLine();

		// Tag
		c8* tag = const_cast<c8*>(data.tag.c_str());
		// Entity Tag
		ImGui::InputText("Tag", tag, 20);

		ImGui::Separator();

		data.tag = tag;
	}

	void InspectorPanel::DisplayTransform(void)
	{
		// Get transform
		Transform& xform = Coordinator::Instance()->GetComponent<Transform>(m_SelectedEntity);

		// Select between the 3 Gizmos Operations by keypress
		if (Input::KeyTriggered(KeyCode::W))
			ALEditor::Instance()->SetCurrentGizmoOperation(ImGuizmo::TRANSLATE);
		if (Input::KeyTriggered(KeyCode::E))
			ALEditor::Instance()->SetCurrentGizmoOperation(ImGuizmo::ROTATE);
		if (Input::KeyTriggered(KeyCode::R))
			ALEditor::Instance()->SetCurrentGizmoOperation(ImGuizmo::SCALE);

		// Transform
		if (ImGui::CollapsingHeader("Transform Component##Inspector"))
		{
			// Rotate
			if (ImGui::RadioButton("Translate", ALEditor::Instance()->GetCurrentGizmoOperation() == ImGuizmo::TRANSLATE))
				ALEditor::Instance()->SetCurrentGizmoOperation(ImGuizmo::TRANSLATE);
			ImGui::SameLine();
			// Rotate
			if (ImGui::RadioButton("Rotate", ALEditor::Instance()->GetCurrentGizmoOperation() == ImGuizmo::ROTATE))
				ALEditor::Instance()->SetCurrentGizmoOperation(ImGuizmo::ROTATE);
			ImGui::SameLine();
			// Scale
			if (ImGui::RadioButton("Scale", ALEditor::Instance()->GetCurrentGizmoOperation() == ImGuizmo::SCALE))
				ALEditor::Instance()->SetCurrentGizmoOperation(ImGuizmo::SCALE);

			// Translate and Scale matrix
			// 1) Get parent global
			// 2) Temp Variable "offset" = xform.position.x * parent Global Scale
			// 3) Display this offset in inspector
			// 4) Calculate new Local (Parent Inverse Global Scale * offset

			f32 mtx_translate[3]{ xform.localPosition.x, xform.localPosition.y, 0.f },
				mtx_scale[3]{ xform.localScale.x, xform.localScale.y, 0.f },
				mtx_rotation{ xform.localRotation };

			f32 const v_speed = ECS::GetSceneGraph().GetParent(m_SelectedEntity) == -1 ? 0.1f : 0.008f;

			// Float inputs
			ImGui::DragFloat2("Tr", mtx_translate, v_speed);		// Translate
			//EDITOR_KEYBOARD_CHECK

			ImGui::DragFloat("Rt", &mtx_rotation, 1.f, 0.f, 360.f);	// Rotate
			//EDITOR_KEYBOARD_CHECK

			ImGui::DragFloat2("Sc", mtx_scale, v_speed);			// Scale
			//EDITOR_KEYBOARD_CHECK

				// Set changes
				Transform a(xform);
			a.localPosition.x = mtx_translate[0];
			a.localPosition.y = mtx_translate[1];

			a.localRotation = mtx_rotation;

			a.localScale.x = mtx_scale[0];
			a.localScale.y = mtx_scale[1];

			// If there are any differences in transform, run command
			if (xform.localPosition.x != a.localPosition.x || xform.localPosition.y != a.localPosition.y ||
				xform.localRotation != a.localRotation ||
				xform.localScale.x != a.localScale.x || xform.localScale.y != a.localScale.y)
			{
				if (Commands::EditorCommandManager::CanAddCommand())
				{
					utils::Ref<COMP_CMD<Transform>> cmd = utils::CreateRef<COMP_CMD<Transform>>(xform, a);
					EditorCommandManager::AddCommand(cmd);
				}
			}
		}
	}

	void InspectorPanel::DisplaySprite(void)
	{
		// Get Sprite
		Sprite& spr = Coordinator::Instance()->GetComponent<Sprite>(m_SelectedEntity);
		if (ImGui::CollapsingHeader("Sprite Component##Inspector"))
		{
			ImGui::PushItemWidth(-(ImGui::GetContentRegionAvail().x / 2.5f));
			// Image
			if (spr.filePath != "")
			{
				Guid id = Engine::AssetManager::Instance()->GetGuid(spr.filePath.c_str());
				u64 texture = (u64)Engine::AssetManager::Instance()->GetButtonImage(id);
				ImVec2 winSize = ImGui::GetWindowSize();
				ImGui::Image(reinterpret_cast<ImTextureID>(texture), { winSize.x * 0.5f, winSize.x * 0.5f }, { 0, 1 }, { 1, 0 });
			}

			// File path
			c8* fp = const_cast<c8*>(spr.filePath.c_str());
			ImGui::PushID("FilePath");
			ImGuiInputTextFlags input_flag = ImGuiInputTextFlags_ReadOnly;
			ImGui::InputTextWithHint("##FilePath123123", "File Path", fp, FILE_BUFFER_SIZE, input_flag);

			// Drag Drop!
			if (ImGui::BeginDragDropTarget())
			{
				// Payload flag
				ImGuiDragDropFlags payload_flag{ 0 };
				//payload_flag |= ImGuiDragDropFlags_AcceptNoDrawDefaultRect;

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
						// Sprite copy
						Sprite cpy{ spr };

						// Set Filepath
						cpy.filePath = filePath;

						cpy.id = Engine::AssetManager::Instance()->GetGuid(filePath);

						// Run Command
						utils::Ref<COMP_CMD<Sprite>> cmd = utils::CreateRef<COMP_CMD<Sprite>>(spr, cpy);
						EditorCommandManager::AddCommand(cmd);
					}
					else
					{
						AL_CORE_ERROR("A .jpg or .png file is required!");
					}
				}
				ImGui::EndDragDropTarget();
			}
			else
				spr.filePath = fp;

			ImGui::PopID();

			// Render Layer
			s32 renderLayer{ static_cast<s32>(spr.layer) };
			ImGui::DragInt("Render Layer", &renderLayer, 1.f, 0, 256);
			spr.layer = static_cast<u32>(renderLayer);

			// Color wheel
			ImGuiColorEditFlags clr_flags = ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_PickerHueBar;
			f32 clr[4] = { spr.color.r, spr.color.g, spr.color.b, spr.color.a };
			//ImGui::ColorEdit4("Color", clr, clr_flags);
			ImGui::ColorPicker4("Color", clr, clr_flags);
			// Set new color
			Sprite cpy{ spr };
			cpy.color.r = clr[0];
			cpy.color.g = clr[1];
			cpy.color.b = clr[2];
			cpy.color.a = clr[3];
			cpy.layer = renderLayer;

			if (cpy.color.r != spr.color.r || cpy.color.g != spr.color.g ||
				cpy.color.b != spr.color.b || cpy.color.a != spr.color.a)
			{
				if (Commands::EditorCommandManager::CanAddCommand())
				{
					utils::Ref<COMP_CMD<Sprite>> cmd = utils::CreateRef<COMP_CMD<Sprite>>(spr, cpy);
					EditorCommandManager::AddCommand(cmd);
				}
			}

			ImGui::PopItemWidth();
		}

		// Drag Drop for Selectable
		if (ImGui::BeginDragDropTarget())
		{
			// Payload flag
			ImGuiDragDropFlags payload_flag{ 0 };
			//payload_flag |= ImGuiDragDropFlags_AcceptNoDrawDefaultRect;

			// Get Drag and Drop Payload
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ASSET_ITEM", payload_flag))
			{
				// Get filepath
				size_t fileLen;	c8 filePath[FILE_BUFFER_SIZE];
				wcstombs_s(&fileLen, filePath, FILE_BUFFER_SIZE, (const wchar_t*)payload->Data, payload->DataSize);

				// Check if image (png or jpg)
				std::string fileString = filePath;
				if (fileString.find(".jpg") != std::string::npos ||
					fileString.find(".png") != std::string::npos)
				{
					// Sprite copy
					Sprite cpy{ spr };

					// Set Filepath
					cpy.filePath = filePath;

					cpy.id = Engine::AssetManager::Instance()->GetGuid(filePath);

					// Run Command
					utils::Ref<COMP_CMD<Sprite>> cmd = utils::CreateRef<COMP_CMD<Sprite>>(spr, cpy);
					EditorCommandManager::AddCommand(cmd);
				}
				else
				{
					AL_CORE_ERROR("A .jpg or .png file is required!");
				}
			}
			ImGui::EndDragDropTarget();
		}

		// Right click!
		if (ImGui::IsItemClicked(1))
		{
			ImGui::OpenPopup("spritecomp_rightclick");
		}

		if (ImGui::BeginPopup("spritecomp_rightclick"))
		{
			if (ImGui::Selectable("Remove Component"))
			{
				ECS::Coordinator::Instance()->RemoveComponent<Sprite>(m_SelectedEntity);
			}
			ImGui::EndPopup();
		}
	}

	void InspectorPanel::DisplayRigidBody(void)
	{
		// Mass, HasGravity, IsEnabled
		if (ImGui::CollapsingHeader("RigidBody Component##Inspector"))
		{
			Rigidbody2D& rb = ECS::Coordinator::Instance()->GetComponent<Rigidbody2D>(m_SelectedEntity);

			ImGui::DragFloat("Mass", &rb.mass, 1.f, 1.f, 100.f);

			ImGui::Checkbox("Has Gravity", &rb.hasGravity);

			ImGui::Checkbox("Is Enabled", &rb.isEnabled);

		}
	}

	void InspectorPanel::DisplayCollider(void)
	{
		// Enum ColliderType, Rotation, Array2 F32 Scale, IsTriggered, IsDebug, IsEnabled, Vec2 LocalPos, 
		if (ImGui::CollapsingHeader("Collider Component##Inspector"))
		{
			Collider2D& collider = ECS::Coordinator::Instance()->GetComponent<Collider2D>(m_SelectedEntity);

			const c8* typeList[1] = { "Rectangle2D_AABB" };
			s32 index = (s32)collider.colliderType;
			// Enum ColliderType
			ImGui::Combo("Collider Type##Collider", &index, typeList, IM_ARRAYSIZE(typeList));

			switch (index)
			{
			case (s32)ColliderType::Rectangle2D_AABB:
				collider.colliderType = ColliderType::Rectangle2D_AABB;
				break;
			case (s32)ColliderType::Rectangle2D_OOBB:
				collider.colliderType = ColliderType::Rectangle2D_OOBB;
				break;
			case (s32)ColliderType::Circle2D:
				collider.colliderType = ColliderType::Circle2D;
				break;
			}

			ImGui::DragFloat("Rotation##Collider", &collider.rotation, 1.f, 0.f, 360.f);

			ImGui::DragFloat2("Scale##Collider", collider.scale);

			ImGui::Checkbox("Is Triggered##Collider", &collider.isTrigger);

			ImGui::Checkbox("Is Enabled##Collider", &collider.isEnabled);

			f32 pos[2]{ collider.m_localPosition.x, collider.m_localPosition.y };
			ImGui::DragFloat("Local Position##Collider", pos);
			collider.m_localPosition = Math::Vec2(pos[0], pos[1]);

			ImGui::TreePop();
		}
	}

	void InspectorPanel::DisplayAudio(void)
	{
		using namespace Engine;
		AudioSource& audioSource = ECS::Coordinator::Instance()->GetComponent<AudioSource>(m_SelectedEntity);

		if (ImGui::CollapsingHeader("Audio Component##Inspector"))
		{
			static u32 toDelete{ ECS::MAX_ENTITIES };
			for (auto& a : audioSource.list)
			{
				Audio& ad = a.second;
				std::string treeName;
				if (ad.m_AudioName.empty())
					treeName = "Audio##Inspector" + std::to_string(ad.m_ID);
				else
				{
					u64 str_it = ad.m_AudioName.find_last_of("\\");
					u64 sizeName = ad.m_AudioName.find_last_of(".") - str_it - 1;

					treeName = ad.m_AudioName.substr(str_it + 1, sizeName);
				}

				if (ImGui::TreeNodeEx(treeName.c_str(), ImGuiTreeNodeFlags_DefaultOpen))
				{
					ImGui::Text("ID: %u", ad.m_ID);

					c8* stringName = const_cast<c8*>(ad.m_AudioName.c_str());
					ImGuiInputTextFlags flag = ImGuiInputTextFlags_ReadOnly;
					ImGui::InputTextWithHint("##AudioFileInputInspector", "Audio Clip Name", stringName, FILE_BUFFER_SIZE, flag);
					ad.m_AudioName = stringName;

					// Drag Drop for Selectable
					if (ImGui::BeginDragDropTarget())
					{
						// Payload flag
						ImGuiDragDropFlags payload_flag{ 0 };
						//payload_flag |= ImGuiDragDropFlags_AcceptNoDrawDefaultRect;

						// Get Drag and Drop Payload
						if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ASSET_ITEM", payload_flag))
						{
							// Get filepath
							size_t fileLen;	c8 filePath[FILE_BUFFER_SIZE];
							wcstombs_s(&fileLen, filePath, FILE_BUFFER_SIZE, (const wchar_t*)payload->Data, payload->DataSize);

							// Check if image (png or jpg)
							std::string fileString = filePath;
							if (fileString.find(".wav") != std::string::npos)
							{
								u32 id_cpy = ad.m_ID;
								ad = AssetManager::Instance()->GetAudio(AssetManager::Instance()->GetGuid(fileString));
								ad.m_ID = id_cpy;
							}
							else
							{
								AL_CORE_ERROR("A .wav file is required!");
							}
						}
						ImGui::EndDragDropTarget();
					}

					if (ImGui::DragFloat("Volume##Inspector", &ad.m_Volume, 0.001f, 0.f, 1.f))
						ad.SetVolume();

					if (ImGui::Checkbox("Loop##Inspector", &ad.m_Loop))
					{
						ad.SetLoop(ad.m_Loop);
					}

					if (ImGui::Checkbox("Mute##Inspector", &ad.m_Mute))
					{
						if (ad.m_Mute)
							ad.Mute();
						else
							ad.Unmute();
					}

					const c8* channelList[]{ "BGM", "SFX" };
					s32 currChannel = static_cast<s32>(ad.m_Channel);
					std::string comboName = "Channel Group##Inspector" + std::to_string(ad.m_ID);
					if (ImGui::BeginCombo(comboName.c_str(), currChannel < 0 ? "BGM" : channelList[currChannel]))
					{
						for (s32 i{ 0 }; i < IM_ARRAYSIZE(channelList); ++i)
						{
							const b8 is_selected = (currChannel == i);
							if (ImGui::Selectable(channelList[i], is_selected))
							{
								ad.m_Channel = static_cast<Channel>(i);
							}
						}

						ImGui::EndCombo();
					}

					b8 playing = ad.IsPlaying();
					if (playing)
					{
						ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
						ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
					}
					if (ImGui::Button("Play Audio##Inspector"))
					{
						ad.Play();
					}
					if (playing)
					{
						ImGui::PopStyleVar();
						ImGui::PopItemFlag();
					}

					ImGui::SameLine();

					playing = ad.IsPlaying();
					if (!playing)
					{
						ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
						ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
					}
					if (ImGui::Button("Stop Audio##Inspector"))
					{
						ad.Stop();
					}
					if (!playing)
					{
						ImGui::PopStyleVar();
						ImGui::PopItemFlag();
					}

					ImGui::TreePop();

					if (ImGui::IsMouseClicked(ImGuiMouseButton_Right))
					{
						ImGui::OpenPopup("audio_rightclick");
						toDelete = ad.m_ID;
					}
				}

			}

			if (ImGui::BeginPopup("audio_rightclick"))
			{
				if (ImGui::Selectable("Remove"))
				{
					if (toDelete != ECS::MAX_ENTITIES)
					{
						audioSource.list.erase(toDelete);
						toDelete = ECS::MAX_ENTITIES;
					}
				}
				ImGui::EndPopup();
			}

			if (ImGui::Button("Add##AudioInspector"))
			{
				Audio ad;
				ad.m_AudioName = "";
				ad.m_ID = audioSource.id;
				audioSource.list[audioSource.id++] = ad;
			}
		}
	}

	void InspectorPanel::DisplayAnimator(void)
	{
		if (ImGui::TreeNodeEx("Animator Component##Inspector"))
		{
			ImGui::TreePop();
		}
	}

	void InspectorPanel::DisplayParticleProperty(void)
	{
		// Get transform
		ParticleProperties& particleProperty = Coordinator::Instance()->GetComponent<ParticleProperties>(m_SelectedEntity);

		if (ImGui::CollapsingHeader("Particle Component"))
		{
			f32 startClr[4] = { particleProperty.colorStart.x, particleProperty.colorStart.y, particleProperty.colorStart.z, 1.f };
			f32 endClr[4] = { particleProperty.colorEnd.x, particleProperty.colorEnd.y, particleProperty.colorEnd.z, 1.f };
			f32 vel[2] = { particleProperty.velocity.x, particleProperty.velocity.y };
			f32 velVariation[2] = { particleProperty.velocityVariation.x, particleProperty.velocityVariation.y };

			ImGui::DragFloat("Start Size", &particleProperty.sizeStart, 0.1f, 0.0f, 1500.0f);
			ImGui::DragFloat("End Size", &particleProperty.sizeEnd, 0.1f, 0.0f, 1000.0f);
			ImGui::DragFloat("Size Variation", &particleProperty.sizeVariation, 0.1f, 0.0f, 1000.0f);
			ImGui::ColorEdit4("Start Color", startClr);
			ImGui::ColorEdit4("End Color", endClr);
			ImGui::DragFloat("Life Time", &particleProperty.lifeTime, 0.1f, 0.0f, 1000.0f);
			ImGui::DragFloat("Spawn Rate", &particleProperty.spawnRate, 0.001f, 0.0f, 10.0f);
			ImGui::DragFloat2("Velocity", vel, 0.02f);
			ImGui::DragFloat2("Velocity Variation", velVariation, 0.02f);
			ImGui::DragFloat("Rotation", &particleProperty.rotation, 0.1f, 0.0f, 1000.0f);

			particleProperty.velocity.x = vel[0];
			particleProperty.velocity.y = vel[1];

			particleProperty.velocityVariation.x = velVariation[0];
			particleProperty.velocityVariation.y = velVariation[1];

			particleProperty.colorStart.x = startClr[0];
			particleProperty.colorStart.y = startClr[1];
			particleProperty.colorStart.z = startClr[2];

			particleProperty.colorEnd.x = endClr[0];
			particleProperty.colorEnd.y = endClr[1];
			particleProperty.colorEnd.z = endClr[2];

			ImGui::Separator();
		}
	}

	void InspectorPanel::DisplayTextProperty(void)
	{
		// Get transform
		Text& prop = Coordinator::Instance()->GetComponent<Text>(m_SelectedEntity);

		if (ImGui::CollapsingHeader("Text Component"))
		{
			// String input field
			c8* str = const_cast<c8*>(prop.textString.c_str());	
			ImGui::InputText("String##InspectorTextComponent", str, 31); // exceeding 31 characters causes crash
			prop.textString = str;

			// font pop down menu
			ImVec2 winsize = ImGui::GetWindowSize();

			if (prop.currentFont.empty())
			{
				ImGui::Text("No Font Selected");
			}
			else
				ImGui::Text(prop.currentFont.c_str());

			if (ImGui::Button("Select Font"))
			{
				ImGui::OpenPopup("fontfamily_popup");
			}
			ImGui::SetNextWindowSize(ImVec2(winsize.x * 0.75f, 100.f));
			if (ImGui::BeginPopup("fontfamily_popup"))
			{
				ImVec2 textSize = ImGui::CalcTextSize("Available Fonts");
				ImGui::SameLine((ImGui::GetWindowContentRegionMax().x * 0.5f) - (textSize.x * 0.5f));
				ImGui::Separator();
				for (auto& font : Engine::AssetManager::Instance()->GetFontList())
				{
					if (ImGui::Selectable(font.second.fontName.c_str()) &&
						m_SelectedEntity != ECS::MAX_ENTITIES)
					{
						prop.currentFont = font.second.fontName;
					}
				}
				ImGui::EndPopup();
			}

			//const c8* currentFont{ nullptr };
			//if (ImGui::BeginCombo("Font##Inspector", currentFont))
			//{
			//	for (auto& i : Engine::AssetManager::Instance()->GetFontList())
			//	{
			//		bool isSelected = (currentFont == i.second.fontName.c_str());
			//		if (ImGui::Selectable(i.second.fontName.c_str()) &&
			//			m_SelectedEntity != ECS::MAX_ENTITIES, &isSelected)
			//		{
			//			prop.currentFont = i.second.fontName;
			//			currentFont = i.second.fontName.c_str();
			//		}

			//		if (isSelected)
			//			ImGui::SetItemDefaultFocus();
			//	}
			//	ImGui::EndCombo();
			//}

			f32 color[4] = { prop.colour.x, prop.colour.y, prop.colour.z , 1.f };
			f32 pos[2] = { prop.position.x, prop.position.y };

			ImGui::DragFloat("Size", &prop.scale, 0.05f, 0.0f, 5.0f);
			ImGui::DragFloat2("Pos", pos, 0.5f);

			// Color wheel
			ImGuiColorEditFlags clr_flags = ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_PickerHueBar;
			ImGui::ColorPicker4("Color", color, clr_flags);

			prop.colour.x = color[0];
			prop.colour.y = color[1];
			prop.colour.z = color[2];

			prop.position.x = pos[0];
			prop.position.y = pos[1];

			ImGui::Separator();
		}
	}

	void InspectorPanel::DisplayEntityScript(void)
	{
		if (ImGui::CollapsingHeader("Script Component##Inspector"))
		{
			EntityScript& es = ECS::Coordinator::Instance()->GetComponent<EntityScript>(m_SelectedEntity);
			/*u64 sizeInit{ es.Init.size() }, sizeUpdate{ es.Update.size() }, sizeExit{ es.Free.size() },
				sizeLoad{ es.Load.size() }, sizeUnload{ es.Unload.size() };*/
			std::string init_list{ "" }, update_list{ "" }, free_list{ "" },
				load_list{ "" }, unload_list{ "" };
			s32 init_select{ 0 }, update_select{ 0 }, free_select{ 0 },
				load_select{ 0 }, unload_select{ 0 };

			// Get list of Init Functions
			for (auto x : es.Init)
			{
				init_list += x.first;
				init_list += '\0';
			}
			init_list += '\0';

			ImGui::Combo("Init##Script", &init_select, init_list.c_str());

			// Get list of Init Functions
			for (auto x : es.Update)
			{
				update_list += x.first;
				update_list += '\0';
			}
			update_list += '\0';
			ImGui::Combo("Update##Script", &update_select, update_list.c_str());

			// Get list of Init Functions
			for (auto x : es.Free)
			{
				free_list += x.first;
				free_list += '\0';
			}
			update_list += '\0';
			ImGui::Combo("Free##Script", &free_select, free_list.c_str());

			// Get list of Init Functions
			for (auto x : es.Load)
			{
				load_list += x.first;
				load_list += '\0';
			}
			load_list += '\0';
			ImGui::Combo("Load##Script", &load_select, load_list.c_str());

			// Get list of Init Functions
			for (auto x : es.Unload)
			{
				unload_list += x.first;
				unload_list += '\0';
			}
			unload_list += '\0';
			ImGui::Combo("Unload##Script", &unload_select, unload_list.c_str());
		}
	}

	void InspectorPanel::AddComponentButton(void)
	{
		// Get Window size
		ImVec2 winsize = ImGui::GetWindowSize();

		// Add Component Button
		f32 addCompSize = ImGui::CalcTextSize("Add Component").x;
		ImGui::NewLine(); ImGui::NewLine(); ImGui::SameLine((winsize.x - addCompSize) * 0.5f);
		if (ImGui::Button("Add Component"))
		{
			ImGui::OpenPopup("addcomponent_popup");
		}

		ImGui::SetNextWindowSize(ImVec2(winsize.x * 0.75f, 100.f));
		// Popup for component
		if (ImGui::BeginPopup("addcomponent_popup"))
		{
			ImVec2 textSize = ImGui::CalcTextSize("Available Components");
			ImGui::SameLine((ImGui::GetWindowContentRegionMax().x * 0.5f) - (textSize.x * 0.5f));
			ImGui::Text("Available Components"); ImGui::Separator();
			u32 count = 0;
			for (u32 i = 1; i < (u32)InspectorComponents::InComp_Total; ++i)
			{
				// Check which component
				switch ((InspectorComponents)i)
				{
				case InspectorComponents::InComp_Transform:
					// Check if has component
					if (!ECS::Coordinator::Instance()->HasComponent<Transform>(m_SelectedEntity))
					{
						if (ImGui::Selectable("Transform Component") &&
							m_SelectedEntity != ECS::MAX_ENTITIES)
						{
							// Add Transform Component
							ECS::Coordinator::Instance()->AddComponent<Transform>(m_SelectedEntity, Transform());
						}
						++count;
					}
					break;
				case InspectorComponents::InComp_Sprite:
					// Check if has component
					if (!ECS::Coordinator::Instance()->HasComponent<Sprite>(m_SelectedEntity))
					{
						if (ImGui::Selectable("Sprite Component") &&
							m_SelectedEntity != ECS::MAX_ENTITIES)
						{
							// Add Sprite Component
							ECS::Coordinator::Instance()->AddComponent<Sprite>(m_SelectedEntity, Sprite());
						}
						++count;
					}
					break;
				case InspectorComponents::InComp_RigidBody:
					// Check if has component
					if (!ECS::Coordinator::Instance()->HasComponent<Rigidbody2D>(m_SelectedEntity))
					{
						if (ImGui::Selectable("RigidBody Component") &&
							m_SelectedEntity != ECS::MAX_ENTITIES)
						{
							// Add RigidBody Component
							ECS::Coordinator::Instance()->AddComponent<Rigidbody2D>(m_SelectedEntity, Rigidbody2D());
						}
						++count;
					}
					break;
				case InspectorComponents::InComp_Collider:
					// Check if has component
					if (!ECS::Coordinator::Instance()->HasComponent<Collider2D>(m_SelectedEntity))
					{
						if (ImGui::Selectable("Collider Component") &&
							m_SelectedEntity != ECS::MAX_ENTITIES)
						{
							// Add Collider Component
							ECS::Coordinator::Instance()->AddComponent<Collider2D>(m_SelectedEntity, Collider2D());
						}
						++count;
					}
					break;
				case InspectorComponents::InComp_Particles:
					// Check if has component
					if (!ECS::Coordinator::Instance()->HasComponent<ParticleProperties>(m_SelectedEntity))
					{
						if (ImGui::Selectable("Particle Component") &&
							m_SelectedEntity != ECS::MAX_ENTITIES)
						{
							// Add Collider Component
							ECS::Coordinator::Instance()->AddComponent<ParticleProperties>(m_SelectedEntity, ParticleProperties());
						}
						++count;
					}
					break;
				case InspectorComponents::InComp_Text:
					// Check if has component
					if (!ECS::Coordinator::Instance()->HasComponent<Text>(m_SelectedEntity))
					{
						if (ImGui::Selectable("Text Component") &&
							m_SelectedEntity != ECS::MAX_ENTITIES)
						{
							// Add Collider Component
							ECS::Coordinator::Instance()->AddComponent<Text>(m_SelectedEntity, Text());
						}
						++count;
					}
					break;
				//case InspectorComponents::InComp_Script:
				//	// Check if has component
				//	if (!ECS::Coordinator::Instance()->HasComponent<EntityScript>(m_SelectedEntity))
				//	{
				//		if (ImGui::Selectable("Script Component") &&
				//			m_SelectedEntity != ECS::MAX_ENTITIES)
				//		{
				//			// Add Script Component
				//			ECS::Coordinator::Instance()->AddComponent<EntityScript>(m_SelectedEntity, EntityScript());
				//		}
				//		++count;
				//	}
				//	break;
				case InspectorComponents::InComp_Audio:
					// Check if has component
					if (!ECS::Coordinator::Instance()->HasComponent<Engine::AudioSource>(m_SelectedEntity))
					{
						if (ImGui::Selectable("Audio Component") &&
							m_SelectedEntity != ECS::MAX_ENTITIES)
						{
							// Add Collider Component
							ECS::Coordinator::Instance()->AddComponent<Engine::AudioSource>(m_SelectedEntity, Engine::AudioSource());
						}
						++count;
					}
					break;
				}
			}

			// Check how many
			if (count == 0)
			{	// Print no components
				ImGui::NewLine();
				textSize = ImGui::CalcTextSize("No Components to Add!");
				ImGui::SameLine((ImGui::GetWindowContentRegionMax().x * 0.5f) - (textSize.x * 0.5f));
				ImGui::Text("No Components to Add!");
			}

			ImGui::EndPopup();
		}
	}

	void InspectorPanel::SetPanelMin(Math::Vec2 min)
	{
		m_PanelMin = ImVec2(min.x, min.y);
	}

	void InspectorPanel::SetDefaults(Math::Vec2 pos, Math::Vec2 size)
	{
		m_DefaultPos = ImVec2(pos.x, pos.y);
		m_DefaultSize = ImVec2(size.x, size.y);
	}

}

#endif