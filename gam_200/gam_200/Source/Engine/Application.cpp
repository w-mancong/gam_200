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

		Entity Wall_Up, Wall_Down, Wall_Left, Wall_Right;
		Entity inner_Wall_Up, inner_Wall_Left;

		bool isDebug = false;

		float speed = 1;

		void Application::Init(void)
		{
			OpenGLWindow::InitGLFWWindow();
			ECS::InitSystem();

			Transform transform{ Vector2(0, 0), Vector2(30, 30), 0.0f };

			//Chika
			{
				transform = { Vector2(0, 0), Vector2(30, 30), 0.0f };

				Entity_Chika = CreateSprite(transform);
				auto& sprite = Coordinator::Instance()->GetComponent<Sprite>(Entity_Chika);
				sprite.color.r = 1.0f, sprite.color.g = 1.0f; sprite.color.b = 1.0f; sprite.color.a = 1.0f;
				sprite.layer = RenderLayer::Player; sprite.mode = RenderMode::Fill;

				CreateBoxCollider2D(Entity_Chika);
				auto& collider = Coordinator::Instance()->GetComponent<Collider>(Entity_Chika);
			}

			//Michan
			{
				transform = { Vector2(120, 150), Vector2(15, 30), 0.0f };

				Entity_Michan = CreateSprite(transform);
				auto& sprite = Coordinator::Instance()->GetComponent<Sprite>(Entity_Michan);
				sprite.color.r = 0.0f, sprite.color.g = 1.0f; sprite.color.b = 0.0f; sprite.color.a = 1.0f;
				sprite.layer = RenderLayer::Player; sprite.mode = RenderMode::Fill;

				CreateBoxCollider2D(Entity_Michan);
			}

			//Wall
			{
				transform = { Vector2(-350, 0), Vector2(30, 550), 0.0f };
				Wall_Left = CreateSprite(transform);
				auto& sprite = Coordinator::Instance()->GetComponent<Sprite>(Wall_Left);
				CreateBoxCollider2D(Wall_Left);

				transform = { Vector2(350, 0), Vector2(30, 550), 0.0f };
				Wall_Right = CreateSprite(transform);
				sprite = Coordinator::Instance()->GetComponent<Sprite>(Wall_Right);
				CreateBoxCollider2D(Wall_Right);

				transform = { Vector2(0, 250), Vector2(650, 30), 0.0f };
				Wall_Up = CreateSprite(transform);
				sprite = Coordinator::Instance()->GetComponent<Sprite>(Wall_Up);
				CreateBoxCollider2D(Wall_Up);

				transform = { Vector2(0, -250), Vector2(650, 30), 0.0f };
				Wall_Down = CreateSprite(transform);
				sprite = Coordinator::Instance()->GetComponent<Sprite>(Wall_Down);
				CreateBoxCollider2D(Wall_Down);

				transform = { Vector2(0, 150), Vector2(200, 2), 0.0f };
				inner_Wall_Up = CreateSprite(transform);
				sprite = Coordinator::Instance()->GetComponent<Sprite>(inner_Wall_Up);
				CreateBoxCollider2D(inner_Wall_Up);

				transform = { Vector2(-250, 0), Vector2(5, 200), 0.0f };
				inner_Wall_Left = CreateSprite(transform);
				sprite = Coordinator::Instance()->GetComponent<Sprite>(inner_Wall_Left);
				CreateBoxCollider2D(inner_Wall_Left);
			}

			//Create Circle
			//CreateSprite(Coordinator::Instance()->CreateEntity(), transform, Shape::Circle, RenderLayer::Background);
		}

		void Application::Update(void)
		{
			// should do the game loop here
			while (!glfwWindowShouldClose(OpenGLWindow::Window()))
			{
				CollisionSystemUpdate();

				//input
				auto& entity_transform = Coordinator::Instance()->GetComponent<Transform>(Entity_Chika);

				//x -= ManCong::Utility::Time::dt;

				if (Input::KeyDown(KeyCode::A)) {
					entity_transform.position.x -= speed;
				}
				if (Input::KeyDown(KeyCode::D)) {
					entity_transform.position.x += speed;
				}
				if (Input::KeyDown(KeyCode::S)) {
					entity_transform.position.y -= speed;
				}
				if (Input::KeyDown(KeyCode::W)) {
					entity_transform.position.y += speed;
				}


				auto& chikaCollider = Coordinator::Instance()->GetComponent<Collider>(Entity_Chika);
				auto& michanCollider = Coordinator::Instance()->GetComponent<Collider>(Entity_Michan);
				auto& upCollider = Coordinator::Instance()->GetComponent<Collider>(Wall_Up);
				auto& downCollider = Coordinator::Instance()->GetComponent<Collider>(Wall_Down);
				auto& leftCollider = Coordinator::Instance()->GetComponent<Collider>(Wall_Left);
				auto& rightCollider = Coordinator::Instance()->GetComponent<Collider>(Wall_Right);
				auto& innerleftCollider = Coordinator::Instance()->GetComponent<Collider>(inner_Wall_Left);
				auto& innerupCollider = Coordinator::Instance()->GetComponent<Collider>(inner_Wall_Up);

				//Change position of collider size
				if (Input::KeyDown(KeyCode::Left)) {
					chikaCollider.m_data.localPosition.x--;
				}
				if (Input::KeyDown(KeyCode::Right)) {
					chikaCollider.m_data.localPosition.x++;
				}
				if (Input::KeyDown(KeyCode::Down)) {
					chikaCollider.m_data.localPosition.y--;
				}
				if (Input::KeyDown(KeyCode::Up)) {
					chikaCollider.m_data.localPosition.y++;
				}

				if (Input::KeyDown(KeyCode::Pad_Add)) {
					chikaCollider.m_data.m_size_data[0]++;
					chikaCollider.m_data.m_size_data[1]++;
					//michanCollider.m_data.m_size_data[0]++;
					//michanCollider.m_data.m_size_data[1]++;
				}
				if (Input::KeyDown(KeyCode::Pad_Subtract)) {
					chikaCollider.m_data.m_size_data[0]--;
					chikaCollider.m_data.m_size_data[1]--;
					//michanCollider.m_data.m_size_data[0]--;
					//michanCollider.m_data.m_size_data[1]--;
				}

				if (Input::KeyDown(KeyCode::P)) {
					speed++;
				}
				if (Input::KeyDown(KeyCode::O)) {
					speed--;
				}

				if (Input::KeyTriggered(KeyCode::Enter)) {
					isDebug = !isDebug;
					michanCollider.m_data.isShowCollider = isDebug;
					chikaCollider.m_data.isShowCollider = isDebug;

					upCollider.m_data.isShowCollider = isDebug;
					downCollider.m_data.isShowCollider = isDebug;
					leftCollider.m_data.isShowCollider = isDebug;
					rightCollider.m_data.isShowCollider = isDebug;
					innerleftCollider.m_data.isShowCollider = isDebug;
					innerupCollider.m_data.isShowCollider = isDebug;
				}

				Render();


				//std::cout << ManCong::Utility::Time::dt << "\n";
				//std::cout << entity_transform.position.x << " : " << entity_transform.position.y << "\n";

				//std::cout << Coordinator::Instance()->GetComponent<Collider>(Entity_Chika).isActive << "\n";

				CollisionSystemLateUpdate();
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