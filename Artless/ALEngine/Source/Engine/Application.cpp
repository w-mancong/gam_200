#include "pch.h"

namespace ALEngine::Engine
{
	using namespace Math;
	using namespace Graphics;
	using namespace ECS;
	using namespace Editor;
	class Application
	{
	public:
		void Init(void);
		void Update(void);
		void Exit(void);
	};

	namespace
	{
		Entity player;
		Entity walls[4]; // btm, left, right, up
		// add function pointers here
		u64 constexpr MAX_BATCH{ 3'000 };
		f32 constexpr ROT_SPEED{ 50.0f };
		Entity batchShowCase[MAX_BATCH];

		void PhysicShowcase(void)
		{
			UpdateCharacterControllerSystem();
		}

		void BatchShowcase(void)
		{
			for (u64 i{}; i < MAX_BATCH; ++i)
			{
				if (!Coordinator::Instance()->HasComponent<EntityData>(*batchShowCase + i))
					continue;
				Transform& t = Coordinator::Instance()->GetComponent<Transform>(*(batchShowCase + i));
				t.rotation += Time::m_DeltaTime * ROT_SPEED;
			}
		}

		std::function<void(void)> showcase[2]{ PhysicShowcase, BatchShowcase };
		u64 showcaseIndex{ 0 };
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
		ALEditor::Instance()->SetDockingEnabled(false);

		Engine::AssetManager::Instance()->Init();		

		AL_CORE_TRACE("THIS IS A TRACE MESSAGE");
		AL_CORE_DEBUG("THIS IS A DEBUG MESSAGE");
		AL_CORE_INFO("THIS IS A INFO MESSAGE");
		AL_CORE_WARN("THIS IS A WARNING MESSAGE");
		AL_CORE_ERROR("THIS IS AN ERROR MESSAGE");
		AL_CORE_CRITICAL("THIS IS A CRITICAL MESSAGE");

		// Showcasing of de-serialisation
		{
			Serializer::ObjectJson oj{ "../ALEngine/Resources/Objects Files/Player1.json" };
			Transform t{ { oj.GetPosX()  , oj.GetPosY(),  0.0f	},
						 { oj.GetScaleX(), oj.GetScaleY()		},
						   static_cast<f32>(oj.GetRotX()) };
			player = CreateSprite(t, oj.GetSprite().c_str(), "Player");
			Sprite& s = Coordinator::Instance()->GetComponent<Sprite>(player);
			s.color.r = static_cast<f32>(oj.GetRed()) / 255.0f;
			s.color.g = static_cast<f32>(oj.GetGreen()) / 255.0f;
			s.color.b = static_cast<f32>(oj.GetBlue()) / 255.0f;
			s.color.a = static_cast<f32>(oj.GetAlpha()) / 255.0f;
			s.layer = RenderLayer::UI;

			//CreateCollider(player);
			//CreateRigidbody(player);
			CreateCharacterController(player);

			if (Coordinator::Instance()->HasComponent<Collider2D>(player)) {
				Collider2D& col = Coordinator::Instance()->GetComponent<Collider2D>(player);
				col.scale[0] = oj.GetScaleOffsetX(), col.scale[1] = oj.GetScaleOffsetY();
			}
		}

		for (u64 i = 0; i < 4; ++i)
		{
			std::ostringstream oss;
			oss << "../ALEngine/Resources/Objects Files/wall" << i << ".json";
			Serializer::ObjectJson oj{ oss.str() };
			Transform t{ { oj.GetPosX()  , oj.GetPosY(),  0.0f },
							 { oj.GetScaleX(), oj.GetScaleY()	 	 },
							   static_cast<f32>(oj.GetRotX())		 };

			Coordinator::Instance()->GetComponent<EntityData>(*(walls + i)).tag = "wall" + i;

			*(walls + i) = CreateSprite(t);
			CreatePhysics2D(*(walls + i));
			Collider2D& col = Coordinator::Instance()->GetComponent<Collider2D>(*(walls + i));
			Rigidbody2D& rb = Coordinator::Instance()->GetComponent<Rigidbody2D>(*(walls + i));
			col.scale[0] = oj.GetScaleOffsetX(), col.scale[1] = oj.GetScaleOffsetY();
			col.m_localPosition.x = oj.GetPositionOffsetX(), col.m_localPosition.y = oj.GetPositionOffsetY();
			rb.isEnabled = oj.GetRigidBodyEnabled();

			Sprite& sprite = Coordinator::Instance()->GetComponent<Sprite>(*(walls + i));
			sprite.layer = RenderLayer::UI;
		}

		f32 const HALF_WIDTH{ static_cast<f32>(OpenGLWindow::width >> 1) * 0.85f }, HALF_HEIGHT{ static_cast<f32>(OpenGLWindow::height >> 1) * 0.85f };
		for (u64 i = 0; i < MAX_BATCH; ++i)
		{
			Transform trans{ { Random::Range(-HALF_WIDTH, HALF_WIDTH), Random::Range(-HALF_HEIGHT, HALF_HEIGHT), 0.0f },
							 { Random::Range(30.0f, 60.0f), Random::Range(30.0f, 60.0f)},
							   Random::Range(0.0f, 360.0f) };
			Entity& en = *(batchShowCase + i);
			if (!(i % 2))
				en = CreateSprite(trans, "Assets/Images/awesomeface.png");
			else
				en = CreateSprite(trans, "Assets/Images/container.jpg");
			Coordinator::Instance()->GetComponent<EntityData>(en).active = false;
			Sprite& sprite = Coordinator::Instance()->GetComponent<Sprite>(en);
			sprite.color.r = Random::Range(0.0f, 1.0f);
			sprite.color.g = Random::Range(0.0f, 1.0f);
			sprite.color.b = Random::Range(0.0f, 1.0f);
			sprite.color.a = Random::Range(0.0f, 1.0f);
			sprite.layer = RenderLayer::UI;
		}

		{
			Serializer::ObjectJson oj{ "../ALEngine/Resources/Objects Files/background.json" };
			Transform t{ { oj.GetPosX()  , oj.GetPosY(),  0.0f	},
						 { oj.GetScaleX(), oj.GetScaleY()		},
						   static_cast<f32>(oj.GetRotX()) };
			Entity en = CreateSprite(t, oj.GetSprite().c_str(), "background");
			Sprite& s = Coordinator::Instance()->GetComponent<Sprite>(en);
			s.color.r = static_cast<f32>(oj.GetRed()) / 255.0f;
			s.color.g = static_cast<f32>(oj.GetGreen()) / 255.0f;
			s.color.b = static_cast<f32>(oj.GetBlue()) / 255.0f;
			s.color.a = static_cast<f32>(oj.GetAlpha()) / 255.0f;
			s.layer = RenderLayer::Background;
		}
	}

	void Application::Update(void)
	{
		// Accumulator for fixed delta time
		f32 accumulator{ 0.f };

		// should do the game loop here
		while (!glfwWindowShouldClose(OpenGLWindow::Window()) && !Input::Input::KeyTriggered(KeyCode::Escape))
		{
			// Get Current Time
			Time::ClockTimeNow();

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
		}
	}

	void Application::Exit(void)
	{
		ALEditor::Instance()->Exit(); // Exit ImGui
		glfwTerminate();			  // clean/delete all GLFW resources
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
		if (Input::KeyTriggered(KeyCode::LeftControl))
		{
			(++showcaseIndex) %= 2;

			auto batch_status = [](b8 active)
			{
				for (u64 i = 0; i < MAX_BATCH; ++i)
				{
					if (!Coordinator::Instance()->HasComponent<EntityData>(*batchShowCase + i))
						continue;
					Coordinator::Instance()->GetComponent<EntityData>(*(batchShowCase + i)).active = active;
				}
			};

			auto physic_status = [](b8 active)
			{
				for (u64 i{}; i < 4; ++i)
				{
					if (!Coordinator::Instance()->HasComponent<EntityData>(*(walls + i)))
						continue;
					Coordinator::Instance()->GetComponent<EntityData>(*(walls + i)).active = active;
				}
				if (!Coordinator::Instance()->HasComponent<EntityData>(player))
					return;
				Coordinator::Instance()->GetComponent<EntityData>(player).active = active;
			};

			switch (showcaseIndex)
			{
				case 0: // physics showcase
				{
					physic_status(true);
					batch_status(false);
					break;
				}
				case 1: // batch showcase
				{
					physic_status(false);
					batch_status(true);
					break;
				}
			}
		}
		showcase[showcaseIndex]();
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
}