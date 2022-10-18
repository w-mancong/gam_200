#include "pch.h"
#include "Graphics/stb_image.h"

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

		Transform trans{ {}, { 200.0f, 200.0f } };
		CreateSprite(trans, "Assets/Images/tileset.png");
	}

	void Application::Update(void)
	{
		// Accumulator for fixed delta time
		f32 accumulator{ 0.f };

		// should do the game loop here
		while (!glfwWindowShouldClose(OpenGLWindow::Window()) && !Input::KeyTriggered(KeyCode::Escape))
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

				while (accumulator >= Time::m_FixedDeltaTime)
				{
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
		OpenGLWindow::InitGLFWWindow();
		GLFWwindow* window = OpenGLWindow::Window();

		Shader shader{ "Assets/Shaders/test.vert", "Assets/Shaders/test.frag" };

		// set up vertex data (and buffer(s)) and configure vertex attributes
		// ------------------------------------------------------------------
		float vertices[] = {
			// positions        // texture coords
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f, // top right
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, // bottom right
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, // bottom left
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f  // top left 
		};
		unsigned int indices[] = {
			0, 1, 3, // first triangle
			1, 2, 3  // second triangle
		};
		unsigned int VBO, VAO, EBO;
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		// position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		// texture coord attribute
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		s32 width, height, nrChannels;
		// The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
		stbi_set_flip_vertically_on_load(true);
		u8* data = stbi_load("Assets/Images/tileset2.png", &width, &height, &nrChannels, STBI_rgb_alpha);

		u32 texture{};
		size_t constexpr tiles_x{ 20 }, tiles_y{ 5 };

		size_t p_dx = width / tiles_x; // pixels of each tile in x
		size_t p_dy = height / tiles_y; // pixels of each tile in y
		size_t tiles = tiles_x * tiles_y; // number of tiles total

		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D_ARRAY, texture);
		glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_RGBA8, p_dx, p_dy, tiles);
		
		u32 pbo{};
		glGenBuffers(1, &pbo);
		glBindBuffer(GL_PIXEL_UNPACK_BUFFER, pbo);
		glBufferData(GL_PIXEL_UNPACK_BUFFER, width * height * nrChannels, data, GL_STREAM_DRAW);
		glPixelStorei(GL_UNPACK_ROW_LENGTH, width); // width
		glPixelStorei(GL_UNPACK_IMAGE_HEIGHT, height); // height

		for (u64 i{}; i < tiles; ++i)
		{
			s32 row = std::floor(i / tiles_x) * p_dy;
			s32 col = (i % tiles_x) * p_dx;

			glPixelStorei(GL_UNPACK_SKIP_PIXELS, col);
			glPixelStorei(GL_UNPACK_SKIP_ROWS, row);

			glTexSubImage3D(GL_TEXTURE_2D_ARRAY,
				0,
				0, 0, i,
				p_dx, p_dy, 1,
				GL_RGBA, 
				GL_UNSIGNED_BYTE,
				0);
		}

		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		
		stbi_image_free(data);
		glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
		glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);

		while (!glfwWindowShouldClose(window) && !Input::KeyTriggered(KeyCode::Escape))
		{
			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D_ARRAY, texture);
			glBindVertexArray(VAO);
			shader.use();
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);

			glfwSwapBuffers(window);
			glfwPollEvents();
		}

		glfwTerminate();
	}

	//Application app;
	//app.Init();
	//app.Update();
	//app.Exit();

	void Engine::Update(void)
	{

	}

	void Engine::FixedUpdate(void)
	{
		// Raycast2DCollision({ -25, 25 }, { 25, 25 });
		UpdateRigidbodySystem();
		UpdateColliderSystem();
	}
}