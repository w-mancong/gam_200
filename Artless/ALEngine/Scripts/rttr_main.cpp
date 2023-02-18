#include <pch.h>
#include <../MyLogicComponents.h>

RTTR_REGISTRATION
{
	using namespace rttr; using namespace ALEngine::Script;
	registration::class_<GameplayCamera>("GameplayCamera")
		.constructor<>()
		.method("DeserializeComponent", &UniBehaviour::DeserializeComponent);

	registration::class_<GameplaySystem>("GameplaySystem")
		.constructor<>()
		.method("DeserializeComponent", &UniBehaviour::DeserializeComponent);

	registration::class_<PauseLogic>("PauseLogic")
		.constructor<>()
		.method("DeserializeComponent", &UniBehaviour::DeserializeComponent);
}