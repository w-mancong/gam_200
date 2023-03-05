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

	registration::class_<GameplaySystem_Interface_Management_Enemy>("GameplaySystem_Interface_Management_Enemy")
		.constructor<>()
		.method("DeserializeComponent", &UniBehaviour::DeserializeComponent);

	registration::class_<GameplaySystem_Interface_Management_GUI>("GameplaySystem_Interface_Management_GUI")
		.constructor<>()
		.method("DeserializeComponent", &UniBehaviour::DeserializeComponent);

	registration::class_<PauseLogic>("PauseLogic")
		.constructor<>()
		.method("DeserializeComponent", &UniBehaviour::DeserializeComponent);

	registration::class_<ResumeButton>("ResumeButton")
		.constructor<>()
		.method("DeserializeComponent", &UniBehaviour::DeserializeComponent);

	registration::class_<HtpButton>("HtpButton")
		.constructor<>()
		.method("DeserializeComponent", &UniBehaviour::DeserializeComponent);
}