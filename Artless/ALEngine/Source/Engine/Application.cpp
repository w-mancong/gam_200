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

		AL_CORE_TRACE("THIS IS A TRACE MESSAGE");
		AL_CORE_DEBUG("THIS IS A DEBUG MESSAGE");
		AL_CORE_INFO("THIS IS A INFO MESSAGE");
		AL_CORE_WARN("THIS IS A WARNING MESSAGE");
		AL_CORE_ERROR("THIS IS AN ERROR MESSAGE");
		AL_CORE_CRITICAL("THIS IS A CRITICAL MESSAGE");

		appStatus = 1;
		RunFileWatcherThread();

		Transform trans{ {}, { 200.0f, 200.0f }, 0 };
		entity = CreateSprite(trans);
		Animator animator = CreateAnimator("Test");
		AttachAnimator(entity, animator);

		AudioManagerInit();

		fmod::System* const& system = GetAudioSystem();
		system->createSound("Assets/Audio/bgm.wav", FMOD_DEFAULT, nullptr, &bgm.sound);
		system->createSound("Assets/Audio/sfx.wav", FMOD_DEFAULT, nullptr, &sfx.sound);

		bgm.loop = true;
		bgm.channel = Channel::BGM;
		bgm.Play();

		sfx.channel = Channel::SFX;

		// Using c++ code to create animation, will be porting it over to allow editor to create clips
		//CreateAnimationClip("Assets/Images/test_spritesheet2.png", "PlayerRunning", 82, 95, 12, 8);
		//AddAnimationToAnimator(animator, "PlayingGuitar");
		//AddAnimationToAnimator(animator, "PlayerRunning");
		//SaveAnimator(animator);
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

			// ImGui Editor
			{
				PROFILER_TIMER("Editor");
				// Begin new ImGui frame
				ALEditor::Instance()->Begin();
			}

			// Normal Update
			{
				PROFILER_TIMER("Update");
				// Normal Update
				Engine::Update();
			}

			// Physics
			{
				PROFILER_TIMER("Physics");

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
					// AL_CORE_DEBUG(Time::m_FPS);
				}
			}

			// Render
			{
				PROFILER_TIMER("Render");
				Render();

				std::ostringstream oss;
				oss << OpenGLWindow::title << " | FPS: " << Time::m_FPS;
				glfwSetWindowTitle(OpenGLWindow::Window(), oss.str().c_str());
			}

			// Wait Time
			{
				PROFILER_TIMER("FPS Wait");
				// Wait for next frame
				Time::WaitUntil();
			}

			// Marks the end of a frame loop, for tracy profiler
			FrameMark
		}
	}

	void Application::Exit(void)
	{
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

		if (Input::KeyTriggered(KeyCode::MouseRightButton))
		{
			Math::vec2 john = Input::GetMouseWorldPos();

			AL_CORE_DEBUG("John Pos: {}, {}", john.x, john.y);
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
		// Raycast2DCollision({ -25, 25 }, { 25, 25 });
		UpdateRigidbodySystem();
		UpdateColliderSystem();
		UpdatePostRigidbodySystem();

		DebugDrawRigidbody();
		DebugDrawCollider();
	}

	int GetAppStatus(void)
	{
		return appStatus;
	}
}