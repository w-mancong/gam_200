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

	Entity player;
	Entity walls[4]; // btm, left, right, up
	// add function pointers here

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

		// Showcasing of de-serialisation
		Serializer::ObjectJson oj{ "../ALEngine/Resources/Objects Files/Player1.json" };
		Transform t{ { oj.GetPosX()  , oj.GetPosY(),  0.0f	}, 
					 { oj.GetScaleX(), oj.GetScaleY()		}, 
					   oj.GetRotX()							};
		player = CreateSprite(t, "Player", oj.GetSprite().c_str());
		Sprite& s = Coordinator::Instance()->GetComponent<Sprite>(player);
		s.color.r = static_cast<f32>(oj.GetRed())	/ 255.0f;
		s.color.g = static_cast<f32>(oj.GetGreen()) / 255.0f;
		s.color.b = static_cast<f32>(oj.GetBlue())	/ 255.0f;
		s.color.a = static_cast<f32>(oj.GetAlpha()) / 255.0f;
		s.layer = RenderLayer::UI;

		CreatePhysics2D(player);
		CreateCharacterController(player);
		Collider2D& playerCollider = Coordinator::Instance()->GetComponent<Collider2D>(player);
		playerCollider.scale[0] = t.scale.x;
		playerCollider.scale[1] = t.scale.y; 
		Coordinator::Instance()->GetComponent<Rigidbody2D>(player).drag.x = 0.25f;

		for (u64 i = 0; i < 4; ++i)
		{
			std::ostringstream oss;
			oss << "../ALEngine/Resources/Objects Files/wall" << i << ".json";
			Serializer::ObjectJson obj{ oss.str() };
			Transform trans{ { obj.GetPosX()  , obj.GetPosY(),  0.0f },
							 { obj.GetScaleX(), obj.GetScaleY()	 	 },
							   obj.GetRotX()						 };

			*(walls + i) = CreateSprite(trans);
			CreatePhysics2D(*(walls + i));
			Collider2D& col = Coordinator::Instance()->GetComponent<Collider2D>(*(walls + i));
			Rigidbody2D& rb = Coordinator::Instance()->GetComponent<Rigidbody2D>(*(walls + i));
			col.scale[0] = obj.GetScaleOffsetX(), col.scale[1] = obj.GetScaleOffsetY();
			col.m_localPosition.x = obj.GetPositionOffsetX(), col.m_localPosition.y = obj.GetPositionOffsetY();
			rb.isEnabled = obj.GetRigidBodyEnabled();
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

			// Begin new ImGui frame
			ALEditor::Instance()->Begin();

			// Normal Update
			Engine::Update();
			// Fixed Update (Physics)
			accumulator += Time::m_DeltaTime;
			while (accumulator >= Time::m_FixedDeltaTime)
			{
				Engine::FixedUpdate();
				accumulator -= Time::m_FixedDeltaTime;
				// AL_CORE_DEBUG(Time::m_FPS);
			}

			// Render
			Render();

			std::ostringstream oss;
			oss << OpenGLWindow::title << " | FPS: " << Time::m_FPS;
			glfwSetWindowTitle(OpenGLWindow::Window(), oss.str().c_str());

			// Wait for next frame
			Time::WaitUntil();
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
		UpdateCharacterControllerSystem();
	}

	void Engine::FixedUpdate(void)
	{
		// Raycast2DCollision({ -25, 25 }, { 25, 25 });
		UpdateRigidbodySystem();
		UpdateColliderSystem();
	}
}