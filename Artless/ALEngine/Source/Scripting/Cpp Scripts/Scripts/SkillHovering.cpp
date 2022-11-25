#include <pch.h>
#include <Scripting/Cpp Scripts/Scripts/SkillHovering.h>

namespace ALEngine
{
	namespace
	{
		b8 found_skill_tooltip{ false };
		ECS::Entity en_tooltip{ ECS::MAX_ENTITIES }, en_skillicon{ ECS::MAX_ENTITIES };
	}

	void SkillInit([[maybe_unused]] ECS::Entity en)
	{
		if (!found_skill_tooltip)
		{
			en_tooltip = Coordinator::Instance()->GetEntityByTag("tooltip_skills");
			en_skillicon = Coordinator::Instance()->GetEntityByTag("skill_icon");
			found_skill_tooltip = true;
		}
		ECS::Subscribe(en, ECS::Component::EVENT_TRIGGER_TYPE::ON_POINTER_STAY, WhenSkillHover);
	}

	void WhenSkillHover(ECS::Entity en)
	{
		std::cout << en << std::endl;
	}

	void SkillReset([[maybe_unused]] ECS::Entity en)
	{
		found_skill_tooltip = false;
		en_tooltip = en_skillicon = ECS::MAX_ENTITIES;
	}
}