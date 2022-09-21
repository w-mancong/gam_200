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

		void Application::Init(void)
		{
			OpenGLWindow::InitGLFWWindow();
			ECS::InitSystem();

			//Obj 1
			Transform transform{ Vector2(0, 150.0f), Vector2(50.f, 50.f), 0.0f };
			Noah = CreateSprite(transform, Shape::Circle, RenderLayer::Background);

			Sprite& sprite2 = Coordinator::Instance()->GetComponent<Sprite>(Noah);
			sprite2.mode = RenderMode::Line;
			sprite2.color = Color{ 1.0f, 0.0f, 0.0f, 1.0f };

			Transform& trans_noah = Coordinator::Instance()->GetComponent<Transform>(Noah);
			CreatePhysics2D(Noah, ColliderType::Circle2D);
			Coordinator::Instance()->GetComponent<Rigidbody2D>(Noah).isEnabled = true;
			Collider2D& collider_Noah = Coordinator::Instance()->GetComponent<Collider2D>(Noah);
			collider_Noah.scale[0] = 50.f, collider_Noah.scale[1] = 25.f;
			collider_Noah.rotation = 0.f;
			trans_noah.rotation = 0.f;

			//Obj 2
			transform = { Vector2(0.0f, 0.0f), Vector2(50.0f, 50.0f), 0.0f };
			Hinata = CreateSprite(transform, Shape::Circle, RenderLayer::Background);

			Sprite& sprite3 = Coordinator::Instance()->GetComponent<Sprite>(Hinata);
			sprite3.mode = RenderMode::Line;
			sprite3.color = Color{ 1.0f, 0.0f, 0.0f, 1.0f };
			Transform& trans_hinata = Coordinator::Instance()->GetComponent<Transform>(Hinata);
			CreatePhysics2D(Hinata, ColliderType::Circle2D);
			Collider2D &collider_hinata = Coordinator::Instance()->GetComponent<Collider2D>(Hinata);
			collider_hinata.scale[0] = 50.f, collider_hinata.scale[1] = 25.f;
			collider_hinata.rotation = 0.0f;

			// Initialize Time (Framerate Controller)
			Time::Init();

			// Init Logger
			ALEngine::Exceptions::Logger::Init();

			// Init ImGui
			ALEditor::Instance()->SetImGuiEnabled(true);
			ALEditor::Instance()->SetDockingEnabled(false);
			ALEditor::Instance()->SetSelectedEntityTransform(&trans_noah);

			AL_CORE_TRACE("THIS IS A TRACE MESSAGE");
			AL_CORE_DEBUG("THIS IS A DEBUG MESSAGE");
			AL_CORE_INFO("THIS IS A INFO MESSAGE");
			AL_CORE_WARN("THIS IS A WARNING MESSAGE");
			AL_CORE_ERROR("THIS IS AN ERROR MESSAGE");
			AL_CORE_CRITICAL("THIS IS A CRITICAL MESSAGE");
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
				//Engine::Update();
				// Fixed Update (Physics)
				accumulator += Time::m_DeltaTime;
				while (accumulator >= Time::m_FixedDeltaTime)
				{
					Engine::FixedUpdate();
					accumulator -= Time::m_FixedDeltaTime;
					//AL_CORE_DEBUG(Time::m_FPS);
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
			Transform& trans = Coordinator::Instance()->GetComponent<Transform>(Noah);
			Rigidbody2D& rigid = Coordinator::Instance()->GetComponent<Rigidbody2D>(Noah);
			f32 constexpr speed = 150.f;
			f32 constexpr rot = 1.0f;

			/*
			if (Input::Input::KeyTriggered(KeyCode::Space)) {
				rigid.velocity.y = 250;
			}*/

			//Use Velocity
			rigid.velocity.x = 0, rigid.velocity.y = 0;
			if (Input::Input::KeyDown(KeyCode::Down))
			{
				rigid.velocity.y = -speed;
			}
			if (Input::Input::KeyDown(KeyCode::Up))
			{
				rigid.velocity.y = speed;
			}
			if (Input::Input::KeyDown(KeyCode::Left))
			{
				rigid.velocity.x = -speed;
			}
			if (Input::Input::KeyDown(KeyCode::Right))
			{
				rigid.velocity.x = speed;
			}

			//Manual Position
			//if (Input::Input::KeyTriggered(KeyCode::W))
			//{
			//	trans.position.y += speed;
			//}
			//if (Input::Input::KeyTriggered(KeyCode::S))
			//{
			//	trans.position.y -= speed * Time::m_DeltaTime;
			//}
			//if (Input::Input::KeyTriggered(KeyCode::D))
			//{
			//	trans.position.x += speed;
			//}
			//if (Input::Input::KeyTriggered(KeyCode::A))
			//{
			//	trans.position.x -= speed;
			//}

			//if(Input::Input::KeyDown(KeyCode::W))
			//{
			//	trans.position.y += speed * Time::m_DeltaTime;
			//}
			//if (Input::Input::KeyDown(KeyCode::S))
			//{
			//	trans.position.y -= speed * Time::m_DeltaTime;
			//}
			//if (Input::Input::KeyDown(KeyCode::D))
			//{
			//	trans.position.x += speed * Time::m_DeltaTime;
			//}
			//if (Input::Input::KeyDown(KeyCode::A))
			//{
			//	trans.position.x -= speed * Time::m_DeltaTime;
			//}
			//if (Input::Input::KeyDown(KeyCode::Q))
			//{
			//	trans.rotation += rot;
			//}
			//if (Input::Input::KeyDown(KeyCode::E))
			//{
			//	trans.rotation -= rot;
			//}		
		}
		
		void Engine::FixedUpdate(void)
		{
			//Raycast2DCollision({ -25, 25 }, { 25, 25 });
			UpdateRigidbodySystem();
			UpdateColliderSystem();
		}
	}
}