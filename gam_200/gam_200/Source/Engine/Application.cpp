#include "pch.h"

namespace ManCong
{
	namespace Engine
	{
		using namespace Math; using namespace Graphics; using namespace ECS;
		class Application
		{
		public:
			void Init(void);
			void Update(void);
			void Exit(void);
		};

		typedef ManCong::Input::Input Input;
		typedef ManCong::Input::KeyCode KeyCode;

		Entity Entity_Chika, Entity_Michan;

		void Application::Init(void)
		{
			OpenGLWindow::InitGLFWWindow();
			ECS::InitSystem();

			//Chika
			{
				Transform transform{ Vector2(0, 0), Vector2(30, 30), 0.0f };

				Entity_Chika = CreateSprite(transform);
				auto& sprite = Coordinator::Instance()->GetComponent<Sprite>(Entity_Chika);
				sprite.color.r = 1.0f, sprite.color.g = 1.0f; sprite.color.b = 1.0f; sprite.color.a = 1.0f;
				sprite.layer = RenderLayer::Player; sprite.mode = RenderMode::Fill;

				CreateBoxCollider2D(Entity_Chika);
			}

			//Michan
			{
				Transform transform{ Vector2(100, 100), Vector2(30, 30), 0.0f };

				Entity_Michan = CreateSprite(transform);
				auto& sprite = Coordinator::Instance()->GetComponent<Sprite>(Entity_Michan);
				sprite.color.r = 0.0f, sprite.color.g = 1.0f; sprite.color.b = 0.0f; sprite.color.a = 1.0f;
				sprite.layer = RenderLayer::Player; sprite.mode = RenderMode::Fill;

				CreateBoxCollider2D(Entity_Michan);
			}

			//Create Circle
			//CreateSprite(Coordinator::Instance()->CreateEntity(), transform, Shape::Circle, RenderLayer::Background);
		}

		void Application::Update(void)
		{
			// should do the game loop here
			while (!glfwWindowShouldClose(OpenGLWindow::Window()))
			{
				Render();
				CheckCollision();

				auto& entity_transform = Coordinator::Instance()->GetComponent<Transform>(Entity_Chika);

				//x -= ManCong::Utility::Time::dt;

				if (Input::KeyDown(KeyCode::A)) {
					entity_transform.position.x--;
				}
				if (Input::KeyDown(KeyCode::D)) {
					entity_transform.position.x++;
				}
				if (Input::KeyDown(KeyCode::S)) {
					entity_transform.position.y--;
				}
				if (Input::KeyDown(KeyCode::W)) {
					entity_transform.position.y++;
				}

				//std::cout << ManCong::Utility::Time::dt << "\n";
				//std::cout << entity_transform.position.x << " : " << entity_transform.position.y << "\n";

				//std::cout << Coordinator::Instance()->GetComponent<Collider>(Entity_Chika).isActive << "\n";
			}
		}

		void Application::Exit(void)
		{
			glfwTerminate();	// clean/delete all GLFW resources
		}

		void Run(void)
		{
			Application app;
			app.Init();
			app.Update();
			app.Exit();
		}
	}
}