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

	registration::class_<NewGameButton>("NewGameButton")
		.constructor<>()
		.method("DeserializeComponent", &UniBehaviour::DeserializeComponent);

	registration::class_<AlphaFadeIn>("AlphaFadeIn")
		.constructor<>()
		.method("DeserializeComponent", &UniBehaviour::DeserializeComponent);

	registration::class_<SceneChangeHelper>("SceneChangeHelper")
		.constructor<>()
		.method("DeserializeComponent", &UniBehaviour::DeserializeComponent);

	registration::class_<QuitButton>("QuitButton")
		.constructor<>()
		.method("DeserializeComponent", &UniBehaviour::DeserializeComponent);

	registration::class_<MainMenuButton>("MainMenuButton")
		.constructor<>()
		.method("DeserializeComponent", &UniBehaviour::DeserializeComponent);

	registration::class_<CubeFloat>("CubeFloat")
		.constructor<>()
		.method("DeserializeComponent", &UniBehaviour::DeserializeComponent);

	registration::class_<OptionButton>("OptionButton")
		.constructor<>()
		.method("DeserializeComponent", &UniBehaviour::DeserializeComponent);

	registration::class_<AudioSetting>("AudioSetting")
		.constructor<>()
		.method("DeserializeComponent", &UniBehaviour::DeserializeComponent);

	registration::class_<CutsceneObject>("CutsceneObject")
		.constructor<>()
		.method("DeserializeComponent", &UniBehaviour::DeserializeComponent);

	registration::class_<FPSDisplay>("FPSDisplay")
		.constructor<>()
		.method("DeserializeComponent", &UniBehaviour::DeserializeComponent);

	registration::class_<WaterGenerator>("WaterGenerator")
		.constructor<>()
		.method("DeserializeComponent", &UniBehaviour::DeserializeComponent);

	registration::class_<PauseButton>("PauseButton")
		.constructor<>()
		.method("DeserializeComponent", &UniBehaviour::DeserializeComponent);
}