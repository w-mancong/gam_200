#include "pch.h"
#include "Time.h"

#include <iostream>

namespace ALEngine
{
	namespace Engine
	{
		using namespace Math; using namespace Graphics; using namespace ECS; using namespace Editor;
		class Application
		{
		public:
			void Init(void);
			void Update(void);
			void Exit(void);
		};

		Entity Noah, Hinata;

		Entity LeftWall, RightWall;

		void Application::Init(void)
		{
			OpenGLWindow::InitGLFWWindow();
			ECS::InitSystem();

			//Obj 1
			Transform transform{ Vector2(0, 150.0f), Vector2(50.f, 50.f), 0.0f };
			Noah = CreateSprite(transform, Shape::Rectangle, RenderLayer::Background);

			Sprite& sprite2 = Coordinator::Instance()->GetComponent<Sprite>(Noah);
			sprite2.mode = RenderMode::Line;
			sprite2.color = Color{ 1.0f, 0.0f, 0.0f, 1.0f };

			Transform& trans_noah = Coordinator::Instance()->GetComponent<Transform>(Noah);
			CreateCharacterController(Noah);
			//Character controller needs these
			CreateCollider(Noah, ColliderType::Rectangle2D_AABB);
			CreateRigidbody(Noah);
			Coordinator::Instance()->GetComponent<Rigidbody2D>(Noah).isEnabled = true;
			Coordinator::Instance()->GetComponent<Rigidbody2D>(Noah).drag.x = 0.5f;
			Coordinator::Instance()->GetComponent<Rigidbody2D>(Noah).mass = 1.f;
			Collider2D& collider_Noah = Coordinator::Instance()->GetComponent<Collider2D>(Noah);
			collider_Noah.scale[0] = 50.f, collider_Noah.scale[1] = 50.f;
			collider_Noah.rotation = 0.f;
			trans_noah.rotation = 0.f;
			collider_Noah.isDebug = true;

			//Obj 2
			transform = { Vector2(0.0f, 0.0f), Vector2(5000.0f, 50.0f), 0.0f };
			Hinata = CreateSprite(transform, Shape::Rectangle, RenderLayer::Background);

			Sprite& sprite3 = Coordinator::Instance()->GetComponent<Sprite>(Hinata);
			sprite3.mode = RenderMode::Line;
			sprite3.color = Color{ 1.0f, 0.0f, 0.0f, 1.0f };
			Transform& trans_hinata = Coordinator::Instance()->GetComponent<Transform>(Hinata);
			CreatePhysics2D(Hinata, ColliderType::Rectangle2D_AABB);
			Collider2D& collider_hinata = Coordinator::Instance()->GetComponent<Collider2D>(Hinata);
			collider_hinata.scale[0] = 5000.f, collider_hinata.scale[1] = 50.f;

			transform = { Vector2(-200.0f, 0.0f), Vector2(50.0f, 500.0f), 0.0f };
			LeftWall = CreateSprite(transform, Shape::Rectangle, RenderLayer::Background);
			Sprite& sprite4 = Coordinator::Instance()->GetComponent<Sprite>(LeftWall);
			sprite4.mode = RenderMode::Line;
			sprite4.color = Color{ 1.0f, 0.0f, 0.0f, 1.0f };
			CreatePhysics2D(LeftWall, ColliderType::Rectangle2D_AABB);
			Collider2D& collider_left = Coordinator::Instance()->GetComponent<Collider2D>(LeftWall);
			collider_left.scale[0] = 50.f, collider_left.scale[1] = 500.f;

			transform = { Vector2(200, 0.0f), Vector2(200.0f, 300), 0.0f };
			RightWall = CreateSprite(transform, Shape::Rectangle, RenderLayer::Background);
			Sprite& sprite5 = Coordinator::Instance()->GetComponent<Sprite>(RightWall);
			sprite5.mode = RenderMode::Line;
			sprite5.color = Color{ 1.0f, 0.0f, 0.0f, 1.0f };
			CreatePhysics2D(RightWall, ColliderType::Rectangle2D_AABB);
			Collider2D& collider_right = Coordinator::Instance()->GetComponent<Collider2D>(RightWall);
			collider_right.scale[0] = 200.f, collider_right.scale[1] = 300.f;

			// Initialize Time (Framerate Controller)
			Time::Init();

			// Init ImGui
			ALEditor::Instance()->SetImGuiEnabled(false);
			ALEditor::Instance()->SetDockingEnabled(true);
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
					//std::cout << Time::m_FPS << std::endl;
				}

				// Render
				Render();

				// Wait for next frame
				Time::WaitUntil();
			}
		}

		void Application::Exit(void)
		{
			ALEditor::Instance()->Exit();	// Exit ImGui
			glfwTerminate();	// clean/delete all GLFW resources
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
			//Raycast2DCollision({ -25, 25 }, { 25, 25 });
			UpdateRigidbodySystem();
			UpdateColliderSystem();
		}
	}
}