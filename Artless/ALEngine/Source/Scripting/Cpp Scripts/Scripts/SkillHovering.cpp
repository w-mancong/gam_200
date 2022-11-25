#include <pch.h>
#include <Scripting/Cpp Scripts/Scripts/SkillHovering.h>

namespace ALEngine
{
	namespace
	{
		using namespace ECS;

		b8 found_skill_tooltip{ false };
		Entity en_tooltip{ MAX_ENTITIES }, en_skillicon{ MAX_ENTITIES };
	}

	void WhenSkillHover(Entity en)
	{
		std::cout << "Pointer stay: " <<  en << std::endl;
	}

	void WhenSkillPointerExit(Entity en)
	{
		std::cout << "Pointer exit: " << en << std::endl;
	}

	void SkillInit([[maybe_unused]] Entity en)
	{
		if (!found_skill_tooltip)
		{
			en_tooltip = Coordinator::Instance()->GetEntityByTag("tooltip_skills");
			en_skillicon = Coordinator::Instance()->GetEntityByTag("skill_icon");
			found_skill_tooltip = true;
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