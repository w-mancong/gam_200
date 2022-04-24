#include "pch.h"

//using namespace ManCong::Math; using namespace ManCong::Engine; using namespace ManCong::Graphics;
namespace
{
	//Vector3 pos{ 0.0f, 0.0f, 3.0f }, front{ 0.0f, 0.0f, -1.0f }, up{ 0.0f, 1.0f, 0.0f };
	//f32 constexpr speed = 2.5f;
	//Camera cam{ pos, up };
	//f32 const HALF_WIDTH = static_cast<f32>(*OpenGLWindow::WindowWidth >> 1), HALF_HEIGHT = static_cast<f32>(*OpenGLWindow::WindowHeight >> 1);
	//f32 prevX = HALF_WIDTH, prevY = HALF_HEIGHT;
	//f32 constexpr sensitivity = 0.1f;

	//void ProcessInput()
	//{
	//	GLFWwindow* window = ManCong::Graphics::OpenGLWindow::GetWindow();
	//	if (Input::KeyTriggered(KeyCode::Escape))
	//		glfwSetWindowShouldClose(window, true);

	//	if (Input::KeyDown(KeyCode::W))
	//		pos += speed * cam.Front() * Time::dt;
	//	if (Input::KeyDown(KeyCode::S))
	//		pos -= speed * cam.Front() * Time::dt;
	//	if (Input::KeyDown(KeyCode::D))
	//		pos += cam.Right() * speed * Time::dt;
	//	if (Input::KeyDown(KeyCode::A))
	//		pos -= cam.Right() * speed * Time::dt;
	//	pos.y = 0.0f;
	//}

	//void mouse_callback(GLFWwindow* window, f64 x_posIn, f64 y_posIn)
	//{
	//	static bool firstMouse = true;
	//	f32 x_pos = static_cast<f32>(x_posIn), y_pos = static_cast<f32>(y_posIn);
	//	if (firstMouse)
	//	{
	//		prevX = x_pos, prevY = y_pos;
	//		firstMouse = false;
	//	}
	//	f32 xOffset = (x_pos - prevX) * sensitivity, yOffset = (y_pos - prevY) * sensitivity;
	//	prevX = x_pos, prevY = y_pos;

	//	// processing mouse_input
	//	cam.Rotation(cam.Yaw() + xOffset, cam.Pitch() - yOffset);
	//}
}

namespace ManCong
{
	namespace Engine
	{
		void Application::Init(void)
		{
			using namespace ECS;
			Graphics::OpenGLWindow::InitGLFWWindow();
			ECS::InitSystem();

			//m1 = MeshBuilder::Instance()->MakeRectangle();
			//m1.shader->use(); 
			//m1.shader->Set("proj", cam.ProjectionMatrix());
			//m1.shader->Set("view", cam.ViewMatrix());
		}

		void Application::Update(void)
		{
			s32 const mode[] = { GL_FILL, GL_LINE }; u64 index{ 0 };
			Matrix4x4 model;
			// should do the game loop here
			//while (!glfwWindowShouldClose(OpenGLWindow::GetWindow()))
			//{
			//	// Input
			//	Time::Update();
			//	ProcessInput();
			//	cam.Position(pos);

			//	if (Input::Input::KeyTriggered(KeyCode::Key_1))
			//		(++index) %= ARRAY_SIZE(mode);

			//	glPolygonMode(GL_FRONT_AND_BACK, mode[index]);

			//	// rendering commands here
			//	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			//	glClear(GL_COLOR_BUFFER_BIT);

				//glActiveTexture(GL_TEXTURE0);
				//glBindTexture(GL_TEXTURE_2D, 0);
				//model = Matrix4x4::Scale(0.5f, 0.5f, 0.5f) * Matrix4x4::Rotation(0.0f, Vector3(0.0f, 0.0f, 1.0f)) * Matrix4x4::Translate(1.0f, 0.0f, 0.0f);
				//m1.shader->use();
				//m1.shader->Set("model", model);
				//m1.shader->Set("color", color.r, 0.0f, color.b, 0.5f);
				//glBindVertexArray(m1.vao);
				//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m1.ebo);
				//glDrawElements(GL_TRIANGLES, m1.indicesSize, GL_UNSIGNED_INT, 0);
				//glBindVertexArray(0);

				//glActiveTexture(GL_TEXTURE0);
				//glBindTexture(GL_TEXTURE_2D, 0);
				//model = Matrix4x4::Scale(0.25f, 0.25f, 0.25f) * Matrix4x4::Rotation(0.0f, Vector3(0.0f, 0.0f, 1.0f)) * Matrix4x4::Translate(1.0f, -1.0f, 0.0f);
				//m2.shader->use();
				//m2.shader->Set("model", model);
				//m2.shader->Set("color", color.r, 0.0f, 0.0f, color.a);
				//glBindVertexArray(m2.vao);
				//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m2.ebo);
				//glDrawElements(GL_TRIANGLES, m2.indicesSize, GL_UNSIGNED_INT, 0);
				//glBindVertexArray(0);

				//model = Matrix4x4::Scale(1.0f, 1.0f, 1.0f) * Matrix4x4::Translate(-1.0f, 0.0f, 0.0f);
				//img1.shader->use();
				//img1.shader->Set("model", model);
				//img1.shader->Set("color", color.r, color.g, color.b, color.a);
				//
				//glActiveTexture(GL_TEXTURE0);
				//glBindTexture(GL_TEXTURE_2D, img1.texture);
				//glBindVertexArray(img1.vao);
				//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, img1.ebo);
				//glDrawElements(GL_TRIANGLES, img1.indicesSize, GL_UNSIGNED_INT, 0);

				//model = Matrix4x4::Scale(0.5f, 0.5f, 0.5f) * Matrix4x4::Rotation(90.0f, Vector3(0.0f, 0.0f, 1.0f)) * Matrix4x4::Translate(-1.0f, 0.0f, 0.0f);
				//img2.shader->use();
				//img2.shader->Set("model", model);
				//img2.shader->Set("color", color.r, 0.0f, color.b, 0.5f);

				//glActiveTexture(GL_TEXTURE0);
				//glBindTexture(GL_TEXTURE_2D, img2.texture);
				//glBindVertexArray(img2.vao);
				//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, img2.ebo);
				//glDrawElements(GL_TRIANGLES, img2.indicesSize, GL_UNSIGNED_INT, 0);

				//img1.shader->use();
				//img1.shader->Set("view", cam.ViewMatrix());

				//m1.shader->use();
				//m1.shader->Set("view", cam.ViewMatrix());

				// check and call events and swap the buffers
				//glfwPollEvents();
				//glfwSwapBuffers(OpenGLWindow::GetWindow());
			//}
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