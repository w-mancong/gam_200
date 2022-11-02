#include "pch.h"

namespace ALEngine::Engine
{
	using namespace Math;
	using namespace Graphics;
	using namespace ECS;
	using namespace Editor;
	namespace
	{
		std::atomic<int> appStatus;
	}

	class Application
	{
	public:
		void Init(void);
		void Update(void);
		void Exit(void);
	};

	namespace
	{
		Entity entity;
		Audio bgm{}, sfx{};
		f32 masterVolume{ 1.0f };
		Entity player, floor, coin, pathfinder, button;
	}

	void CollectCoint(Entity current, Entity other) {
		if (Coordinator::Instance()->HasComponent<CharacterController>(other)) {
			AL_CORE_INFO("Coin Collected");
			Coordinator::Instance()->GetComponent<Collider2D>(current).isEnabled = false;
			Coordinator::Instance()->GetComponent<Transform>(current).position = { 10000,10000 };
		}
	}
	void START() {
		AL_CORE_INFO("START");
	}
	void STAY() {
		AL_CORE_INFO("STAY");
	}

	void CLICK() {
		AL_CORE_INFO("CLICK");
	}

	void EXIT() {
		AL_CORE_INFO("EXIT");
	}

	void Application::Init(void)
	{
		OpenGLWindow::InitGLFWWindow();
		ECS::InitSystem();

		// Initialize Time (Framerate Controller)
		Time::Init();

		// Init Logger
		ALEngine::Exceptions::Logger::Init();

		//// Init ImGui
		ALEditor::Instance()->SetImGuiEnabled(true);
		ALEditor::Instance()->SetDockingEnabled(true);

		Engine::AssetManager::Instance()->Init();

		appStatus = 1;
		RunFileWatcherThread();

		Tree::BinaryTree& sceneGraph = ECS::GetSceneGraph();

		player = Coordinator::Instance()->CreateEntity();
		sceneGraph.Push(-1, player);
		floor= Coordinator::Instance()->CreateEntity();
		sceneGraph.Push(-1, floor);
		coin = Coordinator::Instance()->CreateEntity();
		sceneGraph.Push(-1, coin);
		pathfinder = Coordinator::Instance()->CreateEntity();
		sceneGraph.Push(-1, pathfinder);

		Transform trans;
		Serializer::Serializer level{ "Assets/Dev/Objects/Level.json" };

		trans.position = level.GetVec2("btn_pos", Math::Vec2());
		trans.scale = level.GetVec2("btn_size", Math::Vec2(1.f, 1.f));
		button = CreateSprite(trans, level.GetString("btn_image", "").c_str());
		sceneGraph.Push(-1, button);
		CreateCollider(button);
		CreateEventTrigger(button);
		Subscribe(button, EVENT_TRIGGER_TYPE::ON_POINTER_CLICK, CLICK);

		// Initialize player
		trans.position = level.GetVec2("player_pos", Math::Vec2());
		trans.scale = level.GetVec2("player_size", Math::Vec2());
		Coordinator::Instance()->AddComponent(player, trans);
		CreateSprite(player);
		CreateCollider(player);
		CreateCharacterController(player);
		CreateEventTrigger(player);
		Subscribe(player, EVENT_TRIGGER_TYPE::ON_POINTER_ENTER, START);
		//Subscribe(player, EVENT_TRIGGER_TYPE::ON_POINTER_STAY, STAY);
		Subscribe(player, EVENT_TRIGGER_TYPE::ON_POINTER_EXIT, EXIT);
		Subscribe(player, EVENT_TRIGGER_TYPE::ON_POINTER_CLICK, CLICK);

		trans.position = level.GetVec2("floor_pos", Math::Vec2());
		trans.scale = level.GetVec2("floor_size", Math::Vec2());
		Coordinator::Instance()->AddComponent(floor, trans);
		CreateSprite(floor);
		CreateCollider(floor);
		
		trans.position = level.GetVec2("coin_pos", Math::Vec2());
		trans.scale = level.GetVec2("coin_size", Math::Vec2());
		Coordinator::Instance()->AddComponent(coin, trans);
		CreateSprite(coin);
		CreateCollider(coin);
		Subscribe(Coordinator::Instance()->GetComponent<EventCollisionTrigger>(coin), EVENT_COLLISION_TRIGGER_TYPE::ON_COLLISION_ENTER, CollectCoint);

		trans.position = { 500, 500 };
		trans.scale = { 50, 50 };
		Coordinator::Instance()->AddComponent(pathfinder, trans);
		CreateSprite(pathfinder);
		CreateEnemyUnit(pathfinder);

		AudioManagerInit();

		fmod::System* const& system = GetAudioSystem();
		system->createSound("Assets/Audio/bgm.wav", FMOD_DEFAULT, nullptr, &bgm.sound);
		system->createSound("Assets/Audio/sfx.wav", FMOD_DEFAULT, nullptr, &sfx.sound);

		bgm.loop = true;
		bgm.channel = Channel::BGM;
		bgm.Play();

		sfx.channel = Channel::SFX;

		Transform t1{ {}, { 775.0f, 335.0f }, 0 };
		entity = CreateSprite(t1);
		Animator animator = CreateAnimator("Test");
		AttachAnimator(entity, animator);
		sceneGraph.Push(-1, entity);

		// Using c++ code to create animation, will be porting it over to allow editor to create clips
		//CreateAnimationClip("Assets/Images/test_spritesheet2.png", "PlayerRunning", 82, 95, 12, 8);
		//AddAnimationToAnimator(animator, "PlayingGuitar");
		//AddAnimationToAnimator(animator, "PlayerRunning");
		//SaveAnimator(animator);

		StartGameplaySystem();
	}

	void Application::Update(void)
	{
		// Accumulator for fixed delta time
		f32 accumulator{ 0.f };

		// should do the game loop here
		while (!glfwWindowShouldClose(OpenGLWindow::Window()) && appStatus)
		{
			// Get Current Time
			Time::ClockTimeNow();

			appStatus = !Input::KeyTriggered(KeyCode::Escape);

			// Begin new ImGui frame
			ALEditor::Instance()->Begin();
			
			// Normal Update
			Engine::Update();

			if (ALEditor::Instance()->GetGameActive())
			{
				// Physics
				// Fixed Update (Physics)
				accumulator += Time::m_DeltaTime;

				// Steps to limit num times physics will run per frame
				int currNumSteps{ 0 };

				while (accumulator >= Time::m_FixedDeltaTime)
				{
					// Exit if physics happen more than limit
					if (currNumSteps++ >= Utility::MAX_STEP_FIXED_DT)
						break;

					Engine::FixedUpdate();
					accumulator -= Time::m_FixedDeltaTime;
				}
			}

			// Render
			Render();

			std::ostringstream oss;
			oss << OpenGLWindow::title << " | FPS: " << Time::m_FPS;
			glfwSetWindowTitle(OpenGLWindow::Window(), oss.str().c_str());

			// Wait for next frame
			Time::WaitUntil();
			
			// Marks the end of a frame loop, for tracy profiler
			FrameMark
		}
	}

	void Application::Exit(void)
	{
		ExitGameplaySystem();
		ALEditor::Instance()->Exit();		// Exit ImGui
		AssetManager::Instance()->Exit();	// Clean up all Assets
		AudioManagerExit();
		glfwTerminate();					// clean/delete all GLFW resources
	}

	void Run(void)
	{		
		Application app;
		app.Init();
		app.Update();
		app.Exit();
	}

	void Engine::Update(void)
	{
		ZoneScopedN("Normal Update")
		Input::Update();
		AssetManager::Instance()->Update();
		AudioManagerUpdate();

		if (!ALEditor::Instance()->GetGameActive())
			return;
		UpdateCharacterControllerSystem();
		UpdateEventTriggerSystem();
		UpdateGameplaySystem();

		if (Input::KeyTriggered(KeyCode::MouseRightButton))
		{
			Math::vec2 john = Input::GetMouseWorldPos();

			AL_CORE_DEBUG("Mouse Pos: {}, {}", john.x, john.y);
		}

		Animator& animator = Coordinator::Instance()->GetComponent<Animator>(entity);

		if (Input::KeyTriggered(KeyCode::A))
			ChangeAnimation(animator, "PlayingGuitar");
		if (Input::KeyTriggered(KeyCode::D))
			ChangeAnimation(animator, "PlayerRunning");
		if (Input::KeyTriggered(KeyCode::X))
			sfx.Play();
		if (Input::KeyDown(KeyCode::Z))
		{
			masterVolume -= 0.1f;
			if (masterVolume <= 0.0f)
				masterVolume = 0.0f;	
			SetChannelVolume(Channel::Master, masterVolume);
		}
		if (Input::KeyDown(KeyCode::C))
		{
			masterVolume += 0.1f;
			if (masterVolume <= 1.0f)
				masterVolume = 1.0f;
			SetChannelVolume(Channel::Master, masterVolume);
		}
		if (Input::KeyTriggered(KeyCode::P))
			TogglePauseChannel(Channel::Master);
		if (Input::KeyTriggered(KeyCode::M))
			ToggleMuteChannel(Channel::Master);
		
	}

	void Engine::FixedUpdate(void)
	{
		UpdateRigidbodySystem();
		UpdateColliderSystem();
		UpdatePostRigidbodySystem();
		
		UpdateEventCollisionTriggerSystem();

		DebugDrawRigidbody();
		DebugDrawCollider();
	}

	int GetAppStatus(void)
	{
		return appStatus;
	}
}