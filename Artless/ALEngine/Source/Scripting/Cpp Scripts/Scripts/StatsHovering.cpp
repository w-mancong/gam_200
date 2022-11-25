#include <pch.h>
#include <Scripting/Cpp Scripts/Scripts/StatsHovering.h>

namespace ALEngine
{
	namespace
	{
		using namespace ECS;

		Entity en_tooltip{ MAX_ENTITIES };
	}

	void WhenStatsHover(Entity en)
	{
		if (en_tooltip >= MAX_ENTITIES)
			return;

		SetActive(true, en_tooltip);
	}

	void WhenStatsPointerExit(Entity en)
	{
		if (en_tooltip >= MAX_ENTITIES)
			return;

		SetActive(false, en_tooltip);
	}

	void StatsInit(ECS::Entity en)
	{
		en_tooltip = Coordinator::Instance()->GetEntityByTag("tooltip_stats");

		CreateEventTrigger(en);
		Subscribe(en, Component::EVENT_TRIGGER_TYPE::ON_POINTER_STAY, WhenStatsHover);
		Subscribe(en, Component::EVENT_TRIGGER_TYPE::ON_POINTER_EXIT, WhenStatsPointerExit);
	}

	void StatsReset(ECS::Entity en)
	{
		en_tooltip = MAX_ENTITIES;
	}
}