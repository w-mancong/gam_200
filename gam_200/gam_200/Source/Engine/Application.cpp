#include "pch.h"

namespace
{
	void ProcessInput()
	{
		GLFWwindow* window = ManCong::Graphics::OpenGLWindow::GetWindow();
		if (Input::KeyTriggered(KeyCode::Escape))
			glfwSetWindowShouldClose(window, true);
	}
}

namespace ManCong
{
	namespace Engine
	{
		float vertices[] =
		{
			-0.3f,  0.3f,  // top right
			-0.3f, -0.3f,  // bottom right
			-0.8f, -0.3f,  // bottom left
			-0.8f,  0.3f,  // top left 
		};
		u32 indices[] = {	// starting from 0
			0, 3, 1,   // first triangle
			1, 2, 3    // second triangle
		};
		
		Graphics::Mesh m1, m2;

		const char* vertexShaderSource = "#version 330 core\n"
			"layout (location = 0) in vec3 aPos;\n"
			"void main()\n"
			"{\n"
			"   gl_Position = vec4(aPos.x, aPos.y, 0.0f, 1.0f);\n"
			"}\0";
		const char* fragmentShaderSource = "#version 330 core\n"
			"out vec4 FragColor;\n"
			"void main()\n"
			"{\n"
			"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
			"}\0";

		const char* fragmentShaderSource2 = "#version 330 core\n"
			"out vec4 FragColor;\n"
			"void main()\n"
			"{\n"
			"   FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);\n"
			"}\0";

		u32 triIndex = 0;
		void ChangeTriangle()
		{
			if (Input::Input::KeyTriggered(KeyCode::Key_2))
			{
				(++triIndex) %= 2;
				switch (triIndex)
				{
					case 0:
					{
						f32 vert[] = {
							0.8f,  0.3f,  // top right
							0.8f, -0.3f,  // btm right
							0.3f,  0.3f,  // top left 
							0.3f, -0.3f,  // btm left
						};
						glBindBuffer(GL_ARRAY_BUFFER, m2.vbo);
						glBufferData(GL_ARRAY_BUFFER, sizeof(vert), vert, GL_STATIC_DRAW);
						break;
					}
					case 1:
					{
						f32 vert[] = {
							 0.2f,  0.2f,  // top right
							 0.2f, -0.2f,  // btm right
							-0.2f,  0.2f,  // top left 
							-0.2f, -0.2f,  // btm left
						};
						glBindBuffer(GL_ARRAY_BUFFER, m2.vbo);
						glBufferData(GL_ARRAY_BUFFER, sizeof(vert), vert, GL_STATIC_DRAW);
						break;
					}
				}
			}
		}

		using namespace Graphics;
		void Application::Init(void)
		{
			OpenGLWindow::InitGLFWWindow();

			u32 vertexShader = glCreateShader(GL_VERTEX_SHADER);
			glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
			glCompileShader(vertexShader);

			s32 success;
			char infoLog[512];
			glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(vertexShader, sizeof(infoLog), nullptr, infoLog);
				std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED" << std::endl;
				std::exit(EXIT_FAILURE); return;
			}

			u32 fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
			glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
			glCompileShader(fragmentShader);

			glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(fragmentShader, sizeof(infoLog), nullptr, infoLog);
				std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED" << std::endl;
				std::exit(EXIT_FAILURE); return;
			}

			u32 shaderProgram = glCreateProgram();
			glAttachShader(shaderProgram, vertexShader);
			glAttachShader(shaderProgram, fragmentShader);
			glLinkProgram(shaderProgram);

			glGetShaderiv(shaderProgram, GL_LINK_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(shaderProgram, sizeof(infoLog), nullptr, infoLog);
				std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED" << std::endl;
				std::exit(EXIT_FAILURE); return;
			}

			u32 fs = glCreateShader(GL_FRAGMENT_SHADER);
			glShaderSource(fs, 1, &fragmentShaderSource2, nullptr);
			glCompileShader(fs);
			glGetShaderiv(fs, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(fs, sizeof(infoLog), nullptr, infoLog);
				std::cerr << "ERROR::SHADER::FRAGMENT2::COMPILATION_FAILED" << std::endl;
				std::exit(EXIT_FAILURE); return;
			}
			
			u32 program = glCreateProgram();
			glAttachShader(program, fs);
			glAttachShader(program, vertexShader);
			glLinkProgram(program);

			glDeleteShader(vertexShader); glDeleteShader(fragmentShader); glDeleteShader(fs);

			glGenVertexArrays(1, &m1.vao); glBindVertexArray(m1.vao);
			glGenBuffers(1, &m1.vbo);
			glGenBuffers(1, &m1.ebo);
			// 0. Copy our vertices array in a buffer for OpenGL to use
			glBindBuffer(GL_ARRAY_BUFFER, m1.vbo);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m1.ebo);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
			// 1. Then set the vertex attributes pointer
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*)0);
			glEnableVertexAttribArray(0);

			m1.indicesSize = ARRAY_SIZE(indices);
			m1.shaderProgram = shaderProgram;

			f32 vert[] = {
				0.8f,  0.3f,  // top right
				0.8f, -0.3f,  // btm right
				0.3f,  0.3f,  // top left 
				0.3f, -0.3f,  // btm left
			};
			u32 in[] = {
				1, 0, 3,
				3, 2, 0,
			};

			glGenVertexArrays(1, &m2.vao); glBindVertexArray(m2.vao);
			glGenBuffers(1, &m2.vbo);
			glGenBuffers(1, &m2.ebo);
			// 0. Copy our vertices array in a buffer for OpenGL to use
			glBindBuffer(GL_ARRAY_BUFFER, m2.vbo);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vert), vert, GL_STATIC_DRAW);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m2.ebo);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(in), in, GL_STATIC_DRAW);
			// 1. Then set the vertex attributes pointer
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*)0);
			glEnableVertexAttribArray(0);

			m2.indicesSize = ARRAY_SIZE(in);
			m2.shaderProgram = program;
		}

		void Application::Update(void)
		{
			s32 const mode[] = { GL_LINE, GL_FILL }; u64 index{ 0 }; 
			bool fullScreen = false;
			// should do the game loop here
			while (!glfwWindowShouldClose(OpenGLWindow::GetWindow()))
			{
				// Input
				ProcessInput();
				ChangeTriangle();

				if (Input::Input::KeyTriggered(KeyCode::Key_3))
				{
					fullScreen = !fullScreen;
					OpenGLWindow::FullScreen(fullScreen);
				}

				if (Input::Input::KeyTriggered(KeyCode::Key_1))
					(++index) %= ARRAY_SIZE(mode);

				glPolygonMode(GL_FRONT_AND_BACK, mode[index]);

				// rendering commands here
				glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
				glClear(GL_COLOR_BUFFER_BIT);

				glUseProgram(m1.shaderProgram);
				glBindVertexArray(m1.vao);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m1.ebo);
				glDrawElements(GL_TRIANGLES, m1.indicesSize, GL_UNSIGNED_INT, 0);

				glUseProgram(m2.shaderProgram);
				glBindVertexArray(m2.vao);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m2.ebo);
				glDrawElements(GL_TRIANGLES, m2.indicesSize, GL_UNSIGNED_INT, 0);

				// check and call events and swap the buffers
				glfwPollEvents();
				glfwSwapBuffers(OpenGLWindow::GetWindow());
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