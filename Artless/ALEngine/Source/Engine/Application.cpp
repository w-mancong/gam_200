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
	Entity Wall_Bottom, Wall_Left, Wall_Right, Wall_Up;
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
		ALEditor::Instance()->SetDockingEnabled(false);

		Engine::AssetManager::Instance()->Init();		

		AL_CORE_TRACE("THIS IS A TRACE MESSAGE");
		AL_CORE_DEBUG("THIS IS A DEBUG MESSAGE");
		AL_CORE_INFO("THIS IS A INFO MESSAGE");
		AL_CORE_WARN("THIS IS A WARNING MESSAGE");
		AL_CORE_ERROR("THIS IS AN ERROR MESSAGE");
		AL_CORE_CRITICAL("THIS IS A CRITICAL MESSAGE");

		// Showcasing of de-serialisation
		Serializer::objectJson oj{ "../ALEngine/Resources/Objects Files/Player1.json" };
		Transform t{ { static_cast<f32>(oj.getPosX()), static_cast<f32>(oj.getPosY()), 0.0f }, 
					 { static_cast<f32>(oj.getScaleX()), static_cast<f32>(oj.getScaleY()) }, 
					   oj.getRotX() };
		player = CreateSprite(t, oj.getSprite().c_str());
		Sprite& s = Coordinator::Instance()->GetComponent<Sprite>(player);
		s.color.r = static_cast<f32>(oj.getRed())	/ 255.0f;
		s.color.g = static_cast<f32>(oj.getGreen()) / 255.0f;
		s.color.b = static_cast<f32>(oj.getBlue())	/ 255.0f;
		s.color.a = static_cast<f32>(oj.getAlpha()) / 255.0f;
		s.layer = RenderLayer::UI;

		CreatePhysics2D(player);
		CreateCharacterController(player);
		Collider2D& playerCollider = Coordinator::Instance()->GetComponent<Collider2D>(player);
		playerCollider.scale[0] = 50.f;
		playerCollider.scale[1] = 50.f; 
		Coordinator::Instance()->GetComponent<Rigidbody2D>(player).drag.x = 0.25f;

		Wall_Bottom = CreateSprite(t);
		Wall_Up = CreateSprite(t);
		Wall_Left = CreateSprite(t);
		Wall_Right = CreateSprite(t);

		CreatePhysics2D(Wall_Bottom);
		CreatePhysics2D(Wall_Up);
		CreatePhysics2D(Wall_Left);
		CreatePhysics2D(Wall_Right);

		Coordinator::Instance()->GetComponent<Transform>(Wall_Bottom).position = Vector3(0.f, -250.f, 0.f);
		Coordinator::Instance()->GetComponent<Transform>(Wall_Up).position = Vector3(0.f, 250.f, 0.f);
		Coordinator::Instance()->GetComponent<Transform>(Wall_Left).position = Vector3(-450.f, 0.f, 0.f);
		Coordinator::Instance()->GetComponent<Transform>(Wall_Right).position = Vector3(450.f, 0.f, 0.f);

		Collider2D& wallBottomCollider = Coordinator::Instance()->GetComponent<Collider2D>(Wall_Bottom);
		Collider2D& wallUpCollider = Coordinator::Instance()->GetComponent<Collider2D>(Wall_Up);
		Collider2D& wallLeftCollider = Coordinator::Instance()->GetComponent<Collider2D>(Wall_Left);
		Collider2D& wallRightCollider = Coordinator::Instance()->GetComponent<Collider2D>(Wall_Right);
		wallBottomCollider.scale[0] = 1000.f, wallBottomCollider.scale[1] = 50.f;
		wallUpCollider.scale[0] = 1000.f, wallUpCollider.scale[1] = 50.f;
		wallLeftCollider.scale[0] = 50.f, wallLeftCollider.scale[1] = 430.f;
		wallRightCollider.scale[0] = 50.f, wallRightCollider.scale[1] = 430.f;

		wallLeftCollider.m_localPosition.x -= 20.f;

		Rigidbody2D& wallBottomRigidbody = Coordinator::Instance()->GetComponent<Rigidbody2D>(Wall_Bottom);
		Rigidbody2D& wallUpRigidbody = Coordinator::Instance()->GetComponent<Rigidbody2D>(Wall_Up);
		Rigidbody2D& wallLeftRigidbody = Coordinator::Instance()->GetComponent<Rigidbody2D>(Wall_Left);
		Rigidbody2D& wallRightRigidbody = Coordinator::Instance()->GetComponent<Rigidbody2D>(Wall_Right);
		wallBottomRigidbody.isEnabled = false;
		wallUpRigidbody.isEnabled = false;
		wallLeftRigidbody.isEnabled = false;
		wallRightRigidbody.isEnabled = false;
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