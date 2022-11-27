/*!
file:	SkillHovering.cpp
author:	Wong Man Cong
email:	w.mancong\@digipen.edu
brief:	This file contain function declaration that handles hovering over skill icons

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#include <pch.h>
#include <Scripting/Cpp Scripts/Scripts/SkillHovering.h>

namespace ALEngine
{
	namespace
	{
		using namespace ECS;

		b8 found_skill_tooltip{ false };
		Entity en_tooltip{ MAX_ENTITIES }, en_skillicon{ MAX_ENTITIES }, en_textskill{ MAX_ENTITIES },
			en_hard_drop{ MAX_ENTITIES }, en_life_drain{ MAX_ENTITIES };
		math::vec3 const original_position{ 642.0f, 353.0f, 0.0f };
	}

	void WhenSkillHover([[maybe_unused]] Entity en)
	{
		if (en_tooltip >= ECS::MAX_ENTITIES)
			return;

		// tool tip skills
		math::vec3& position = Coordinator::Instance()->GetComponent<Transform>(en_tooltip).position;

		// skill icon
		Sprite& sprite = Coordinator::Instance()->GetComponent<Sprite>(en_skillicon);

		// text skill entity
		Text& text = Coordinator::Instance()->GetComponent<Text>(en_textskill);
		EntityData& text_ed = Coordinator::Instance()->GetComponent<EntityData>(en_textskill);

		std::string const& tag = Coordinator::Instance()->GetComponent<EntityData>(en).tag;
		// Geting the index of the substring
		u64 const index = static_cast<u64>( std::atoi( tag.substr( tag.find_first_of("123456") ).c_str() ) );

		switch (index)
		{
			case 1:
				position = { 400.0f, 190.0f, 0.0f };

				text.position = { 185.0f, 130.0f, 0.0f };
				text.textString = "Hard Drop";
				text_ed.selfActive = true;

				sprite.id = Engine::AssetManager::Instance()->GetGuid("Assets\\Images\\HardDrop.png");

				SetActive(true, en_hard_drop);
				break;
			case 2:
				position = { 475.0f, 190.0f, 0.0f };

				text.position = { 227.0f, 130.0f, 0.0f };
				text.textString = "Life Drain";
				text_ed.selfActive = true;

				sprite.id = Engine::AssetManager::Instance()->GetGuid("Assets\\Images\\LifeDrain.png");

				SetActive(true, en_life_drain);
				break;
			case 3:
				position = { 535.0f, 190.0f, 0.0f };
				break;
			case 4:
				position = { 610.0f, 190.0f, 0.0f };
				break;
			case 5:
				position = { 665.0f, 190.0f, 0.0f };
				break;
			case 6:
				position = { 725.0f, 190.0f, 0.0f };
				break;
		}
		SetActive(true, en_tooltip);
	}

	void WhenSkillPointerExit([[maybe_unused]] Entity en)
	{
		if (en_tooltip >= ECS::MAX_ENTITIES)
			return;

		SetActive(false, en_tooltip);
		SetActive(false, en_hard_drop);
		SetActive(false, en_life_drain);
		SetActive(false, en_textskill);

		// Reset tooltip to original position
		math::vec3& position = Coordinator::Instance()->GetComponent<Transform>(en_tooltip).position;
		position = original_position;

		// text skillname entity
		Coordinator::Instance()->GetComponent<EntityData>(en_textskill).selfActive = false;

		// Reset tooltip skill icon to be X
		Sprite& sprite = Coordinator::Instance()->GetComponent<Sprite>(en_skillicon);
		sprite.id = Engine::AssetManager::Instance()->GetGuid("Assets\\Images\\cross.png");
	}

	void SkillInit([[maybe_unused]] Entity en)
	{
		if (!found_skill_tooltip)
		{
			en_tooltip	  = Coordinator::Instance()->GetEntityByTag("tooltip_skills");
			en_skillicon  = Coordinator::Instance()->GetEntityByTag("skill_icon");
			en_textskill  = Coordinator::Instance()->GetEntityByTag("text_skillname");
			en_hard_drop  = Coordinator::Instance()->GetEntityByTag("hard_drop_des1");
			en_life_drain = Coordinator::Instance()->GetEntityByTag("life_drain_des1");
			found_skill_tooltip = true;

			Coordinator::Instance()->GetComponent<Transform>(en_tooltip).position = original_position;
		}
		Subscribe(en, Component::EVENT_TRIGGER_TYPE::ON_POINTER_STAY, WhenSkillHover);
		Subscribe(en, Component::EVENT_TRIGGER_TYPE::ON_POINTER_EXIT, WhenSkillPointerExit);
	}

	void SkillReset([[maybe_unused]] Entity en)
	{
		found_skill_tooltip = false;
		en_tooltip = en_skillicon = MAX_ENTITIES;
	}
}