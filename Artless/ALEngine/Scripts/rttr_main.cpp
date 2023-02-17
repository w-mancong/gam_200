#include <pch.h>
#include <../MyLogicComponents.h>

namespace
{
	RTTR_REGISTRATION
	{
		using namespace rttr; using namespace ALEngine::Script;
		registration::class_<GameplayCamera>("GameplayCamera")
			.method("DeserializeComponent", &UniBehaviour::DeserializeComponent);

		registration::class_<GameplaySystem>("GameplaySystem")
			.method("DeserializeComponent", &UniBehaviour::DeserializeComponent);

		registration::class_<PauseLogic>("PauseLogic")
			.method("DeserializeComponent", &UniBehaviour::DeserializeComponent);
	}
}