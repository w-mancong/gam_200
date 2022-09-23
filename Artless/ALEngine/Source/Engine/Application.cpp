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

		//Player
		Entity player;

		//Boundaries
		Entity BottomWall,LeftWall, RightWall, TopWall;

		void Application::Init(void)
		{
			OpenGLWindow::InitGLFWWindow();
			ECS::InitSystem();

			//Player
			Transform transform{ Vector2(0, 150.0f), Vector2(50.f, 50.f), 0.0f };
			player = CreateSprite(transform, Shape::Rectangle, RenderLayer::Background);

			Sprite& sprite2 = Coordinator::Instance()->GetComponent<Sprite>(player);
			sprite2.mode = RenderMode::Line;
			sprite2.color = Color{ 1.0f, 0.0f, 0.0f, 1.0f };

			Transform& trans_noah = Coordinator::Instance()->GetComponent<Transform>(player);
			CreateCharacterController(player);

			//Character controller needs these
			CreateCollider(player, ColliderType::Rectangle2D_AABB);
			CreateRigidbody(player);
			Coordinator::Instance()->GetComponent<Rigidbody2D>(player).isEnabled = true;
			Coordinator::Instance()->GetComponent<Rigidbody2D>(player).drag.x = 0.25f;
			Coordinator::Instance()->GetComponent<Rigidbody2D>(player).mass = 1.f;
			Collider2D& collider_Noah = Coordinator::Instance()->GetComponent<Collider2D>(player);
			collider_Noah.scale[0] = 50.f, collider_Noah.scale[1] = 50.f;
			collider_Noah.rotation = 0.f;
			trans_noah.rotation = 0.f;
			collider_Noah.isDebug = true;

			Sprite& sprite = Coordinator::Instance()->GetComponent<Sprite>(player);
			sprite.mode = RenderMode::Line;
			sprite.color = Color{ 1.0f, 1.0f, 1.0f, 1.0f };

			//Bottom Wall
			transform = { Vector2(0.0f, -250.0f), Vector2(5000.0f, 50.0f), 0.0f };
			BottomWall = CreateSprite(transform, Shape::Rectangle, RenderLayer::Background);
			Sprite& sprite3 = Coordinator::Instance()->GetComponent<Sprite>(BottomWall);
			sprite3.mode = RenderMode::Line;
			sprite3.color = Color{ 1.0f, 1.0f, 1.0f, 1.0f };
			CreatePhysics2D(BottomWall, ColliderType::Rectangle2D_AABB);
			Collider2D& collider_bottom = Coordinator::Instance()->GetComponent<Collider2D>(BottomWall);
			collider_bottom.scale[0] = 5000.f, collider_bottom.scale[1] = 50.f;

			//Top Wall
			transform = { Vector2(0.0f, 250.0f), Vector2(5000.0f, 50.0f), 0.0f };
			TopWall = CreateSprite(transform, Shape::Rectangle, RenderLayer::Background);
			Sprite& sprite6 = Coordinator::Instance()->GetComponent<Sprite>(TopWall);
			sprite6.mode = RenderMode::Line;
			sprite6.color = Color{ 1.0f, 1.0f, 1.0f, 1.0f };
			CreatePhysics2D(TopWall, ColliderType::Rectangle2D_AABB);
			Collider2D& collider_top = Coordinator::Instance()->GetComponent<Collider2D>(TopWall);
			collider_top.scale[0] = 5000.0f, collider_top.scale[1] = 50.0f;


			//Left Wall
			transform = { Vector2(-500.0f, 0.0f), Vector2(50.0f, 500.0f), 0.0f };
			LeftWall = CreateSprite(transform, Shape::Rectangle, RenderLayer::Background);
			Sprite& sprite4 = Coordinator::Instance()->GetComponent<Sprite>(LeftWall);
			sprite4.mode = RenderMode::Line;
			sprite4.color = Color{ 1.0f, 1.0f, 1.0f, 1.0f };
			CreatePhysics2D(LeftWall, ColliderType::Rectangle2D_AABB);
			Collider2D& collider_left = Coordinator::Instance()->GetComponent<Collider2D>(LeftWall);
			collider_left.scale[0] = 50.f, collider_left.scale[1] = 500.f;

			//Right
			transform = { Vector2(500.0f, 0.0f), Vector2(200.0f, 425.0f), 0.0f };
			RightWall = CreateSprite(transform, Shape::Rectangle, RenderLayer::Background);
			Sprite& sprite5 = Coordinator::Instance()->GetComponent<Sprite>(RightWall);
			sprite5.mode = RenderMode::Line;
			sprite5.color = Color{ 1.0f, 1.0f, 1.0f, 1.0f };
			CreatePhysics2D(RightWall, ColliderType::Rectangle2D_AABB);
			Collider2D& collider_right = Coordinator::Instance()->GetComponent<Collider2D>(RightWall);
			collider_right.scale[0] = 200.f, collider_right.scale[1] = 425.0f;

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

					std::stringstream buffer;
					buffer << Time::m_FPS;
					glfwSetWindowTitle(OpenGLWindow::Window(), buffer.str().c_str());
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