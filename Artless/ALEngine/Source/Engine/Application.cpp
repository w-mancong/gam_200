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

	namespace
	{
		Entity player;
		Entity walls[4]; // btm, left, right, up
		// add function pointers here
		u64 constexpr MAX_BATCH{ 3'000 };
		f32 constexpr ROT_SPEED{ 50.0f };
		//Entity batchShowCase[MAX_BATCH];

		void PhysicShowcase(void)
		{
			UpdateCharacterControllerSystem();
		}

		void BatchShowcase(void)
		{
			//for (u64 i{}; i < MAX_BATCH; ++i)
			//{
			//	if (!Coordinator::Instance()->HasComponent<EntityData>(*(batchShowCase + i)))
			//		continue;
			//	Transform& t = Coordinator::Instance()->GetComponent<Transform>(*(batchShowCase + i));
			//	t.rotation += Time::m_DeltaTime * ROT_SPEED;
			//}
		}

		std::function<void(void)> showcase[2]{ PhysicShowcase, BatchShowcase };
		u64 showcaseIndex{ 0 };
	}

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
		{
			Serializer::ObjectJson oj{ "../ALEngine/Resources/Objects Files/Player1.json" };
			Transform t{ { oj.GetPosX()  , oj.GetPosY(),  0.0f	},
						 { oj.GetScaleX(), oj.GetScaleY()		},
						   static_cast<f32>(oj.GetRotX()) };
			player = CreateSprite(t, oj.GetSprite().c_str(), "Player");
			Sprite& s = Coordinator::Instance()->GetComponent<Sprite>(player);
			s.color.r = static_cast<f32>(oj.GetRed()) / 255.0f;
			s.color.g = static_cast<f32>(oj.GetGreen()) / 255.0f;
			s.color.b = static_cast<f32>(oj.GetBlue()) / 255.0f;
			s.color.a = static_cast<f32>(oj.GetAlpha()) / 255.0f;
			s.layer = RenderLayer::UI;

			//CreatePhysics2D(player);
			//CreateCharacterController(player);
			//Collider2D& col = Coordinator::Instance()->GetComponent<Collider2D>(player);
			//col.scale[0] = oj.GetScaleOffsetX(), col.scale[1] = oj.GetScaleOffsetY();
			//Coordinator::Instance()->GetComponent<Rigidbody2D>(player).isEnabled = false;
		}

		//for (u64 i = 0; i < 4; ++i)
		//{
		//	std::ostringstream oss;
		//	oss << "../ALEngine/Resources/Objects Files/wall" << i << ".json";
		//	Serializer::ObjectJson oj{ oss.str() };
		//	Transform t{ { oj.GetPosX()  , oj.GetPosY(),  0.0f },
		//					 { oj.GetScaleX(), oj.GetScaleY()	 	 },
		//					   static_cast<f32>(oj.GetRotX())		 };

		//	Coordinator::Instance()->GetComponent<EntityData>(*(walls + i)).tag = "wall" + i;

		//	*(walls + i) = CreateSprite(t);
		//	CreatePhysics2D(*(walls + i));
		//	Collider2D& col = Coordinator::Instance()->GetComponent<Collider2D>(*(walls + i));
		//	Rigidbody2D& rb = Coordinator::Instance()->GetComponent<Rigidbody2D>(*(walls + i));
		//	col.scale[0] = oj.GetScaleOffsetX(), col.scale[1] = oj.GetScaleOffsetY();
		//	col.m_localPosition.x = oj.GetPositionOffsetX(), col.m_localPosition.y = oj.GetPositionOffsetY();
		//	rb.isEnabled = oj.GetRigidBodyEnabled();

		//	Sprite& sprite = Coordinator::Instance()->GetComponent<Sprite>(*(walls + i));
		//	sprite.layer = RenderLayer::UI;
		//}

		//f32 const HALF_WIDTH{ static_cast<f32>(OpenGLWindow::width >> 1) * 0.85f }, HALF_HEIGHT{ static_cast<f32>(OpenGLWindow::height >> 1) * 0.85f };
		//for (u64 i = 0; i < MAX_BATCH; ++i)
		//{
		//	Transform trans{ { Random::Range(-HALF_WIDTH, HALF_WIDTH), Random::Range(-HALF_HEIGHT, HALF_HEIGHT), 0.0f },
		//					 { Random::Range(30.0f, 60.0f), Random::Range(30.0f, 60.0f)},
		//					   Random::Range(0.0f, 360.0f) };
		//	Entity& en = *(batchShowCase + i);
		//	if (!(i % 2))
		//		en = CreateSprite(trans, "Assets/Images/awesomeface.png");
		//	else
		//		en = CreateSprite(trans, "Assets/Images/container.jpg");
		//	Coordinator::Instance()->GetComponent<EntityData>(en).active = false;
		//	Sprite& sprite = Coordinator::Instance()->GetComponent<Sprite>(en);
		//	sprite.color.r = Random::Range(0.0f, 1.0f);
		//	sprite.color.g = Random::Range(0.0f, 1.0f);
		//	sprite.color.b = Random::Range(0.0f, 1.0f);
		//	sprite.color.a = Random::Range(0.0f, 1.0f);
		//	sprite.layer = RenderLayer::UI;
		//}

		//{
		//	Serializer::ObjectJson oj{ "../ALEngine/Resources/Objects Files/background.json" };
		//	Transform t{ { oj.GetPosX()  , oj.GetPosY(),  0.0f	},
		//				 { oj.GetScaleX(), oj.GetScaleY()		},
		//				   static_cast<f32>(oj.GetRotX()) };
		//	Entity en = CreateSprite(t, oj.GetSprite().c_str(), "background");
		//	Sprite& s = Coordinator::Instance()->GetComponent<Sprite>(en);
		//	s.color.r = static_cast<f32>(oj.GetRed()) / 255.0f;
		//	s.color.g = static_cast<f32>(oj.GetGreen()) / 255.0f;
		//	s.color.b = static_cast<f32>(oj.GetBlue()) / 255.0f;
		//	s.color.a = static_cast<f32>(oj.GetAlpha()) / 255.0f;
		//	s.layer = RenderLayer::Background;
		//}
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

	void processInput(GLFWwindow* window);
	void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);

	// settings
	const unsigned int SCR_WIDTH = 800;
	const unsigned int SCR_HEIGHT = 600;
	//
	namespace
	{
		struct Vertex2D
		{
			float x, y;  //Position
			float u, v;  //Uv
		};

		struct DrawElementsCommand
		{
			GLuint vertexCount;
			GLuint instanceCount;
			GLuint firstIndex;
			GLuint baseVertex;
			GLuint baseInstance;
		};

		struct Matrix
		{
			float a0, a1, a2, a3;
			float b0, b1, b2, b3;
			float c0, c1, c2, c3;
			float d0, d1, d2, d3;
		};

		void setMatrix(Matrix* matrix, const float x, const float y)
		{
			/*
			1 0 0 0
			0 1 0 0
			0 0 1 0
			x y 0 1
			*/
			matrix->a0 = 1;
			matrix->a1 = matrix->a2 = matrix->a3 = 0;

			matrix->b1 = 1;
			matrix->b0 = matrix->b2 = matrix->b3 = 0;

			matrix->c2 = 1;
			matrix->c0 = matrix->c1 = matrix->c3 = 0;

			matrix->d0 = x;
			matrix->d1 = y;
			matrix->d2 = 0;
			matrix->d3 = 1;
		}

		const GLchar* gVertexShaderSource[] =
		{
			"#version 430 core\n"

			"layout (location = 0 ) in vec2 position;\n"
			"layout (location = 1 ) in vec2 texCoord;\n"
			"layout (location = 2 ) in uint drawid;\n"
			"layout (location = 3 ) in mat4 instanceMatrix;\n"

			"layout (location = 0 ) out vec2 uv;\n"
			"layout (location = 1 ) flat out uint drawID;\n"

			"void main(void)\n"
			"{\n"
			"  uv = texCoord;\n"
			"  drawID = drawid;\n"
			"  gl_Position = instanceMatrix * vec4(position,0.0,1.0);\n"
			"}\n"
		};

		const GLchar* gFragmentShaderSource[] =
		{
			"#version 430 core\n"

			"layout (location = 0 ) in vec2 uv;\n"
			"layout (location = 1 ) flat in uint drawID;\n"

			"layout (location = 0) out vec4 color;\n"

			"layout (binding = 0) uniform sampler2DArray textureArray;\n"

			"void main(void)\n"
			"{\n"
			"  color = texture(textureArray, vec3(uv.x, uv.y, drawID) );\n"
			"}\n"
		};

		GLuint gVAO(0);
		GLuint gArrayTexture(0);
		GLuint gVertexBuffer(0);
		GLuint gElementBuffer(0);
		GLuint gIndirectBuffer(0);
		GLuint gMatrixBuffer(0);
		GLuint gProgram(0);

		float gMouseX(0);
		float gMouseY(0);

	}//Unnamed namespace

	void GenerateGeometry()
	{
		const std::vector<Vertex2D> gQuad = {
			//xy			//uv
			{ 0.0f,0.0f,	0.0f,0.0f },
			{ 0.1f,0.0f,	1.0f,0.0f },
			{ 0.05f, 0.05f, 0.5f, 0.5f},
			{ 0.0f,0.1f,	0.0f,1.0f },
			{ 0.1f,0.1f,	1.0f,1.0f }
		};

		const std::vector<Vertex2D> gTriangle =
		{
			{ 0.0f, 0.0f,	0.0f,0.0f},
			{ 0.05f, 0.1f,	0.5f,1.0f},
			{ 0.1f, 0.0f,	1.0f,0.0f}
		};

		const std::vector<unsigned int> gQuadIndex = {
			0,1,2,
			1,4,2,
			2,4,3,
			0,2,3
		};

		const std::vector<unsigned int> gTriangleIndex =
		{
			0,1,2
		};

		//Generate 50 quads, 50 triangles
		const unsigned num_vertices = gQuad.size() * 50 + gTriangle.size() * 50;
		std::vector<Vertex2D> vVertex(num_vertices);
		Matrix vMatrix[100];
		unsigned vertexIndex(0);
		unsigned matrixIndex(0);
		//Clipspace, lower left corner = (-1, -1)
		float xOffset(-0.95f);
		float yOffset(-0.95f);
		// populate geometry
		for (unsigned int i(0); i != 10; ++i)
		{
			for (unsigned int j(0); j != 10; ++j)
			{
				//quad
				if (j % 2 == 0)
				{
					for (unsigned int k(0); k != gQuad.size(); ++k)
					{
						vVertex[vertexIndex++] = gQuad[k];
					}
				}
				//triangle
				else
				{
					for (unsigned int k(0); k != gTriangle.size(); ++k)
					{
						vVertex[vertexIndex++] = gTriangle[k];
					}
				}
				//set position in model matrix
				setMatrix(&vMatrix[matrixIndex++], xOffset, yOffset);
				xOffset += 0.2f;
			}
			yOffset += 0.2f;
			xOffset = -0.95f;
		}

		glGenVertexArrays(1, &gVAO);
		glBindVertexArray(gVAO);
		//Create a vertex buffer object
		glGenBuffers(1, &gVertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, gVertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex2D) * vVertex.size(), vVertex.data(), GL_STATIC_DRAW);

		//Specify vertex attributes for the shader
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), (GLvoid*)(offsetof(Vertex2D, x)));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), (GLvoid*)(offsetof(Vertex2D, u)));

		//Create an element buffer and populate it
		int triangle_bytes = sizeof(unsigned int) * gTriangleIndex.size();
		int quad_bytes = sizeof(unsigned int) * gQuadIndex.size();
		glGenBuffers(1, &gElementBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gElementBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, triangle_bytes + quad_bytes, NULL, GL_STATIC_DRAW);

		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, quad_bytes, gQuadIndex.data());
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, quad_bytes, triangle_bytes, gTriangleIndex.data());

		//Setup per instance matrices
		//Method 1. Use Vertex attributes and the vertex attrib divisor
		glGenBuffers(1, &gMatrixBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, gMatrixBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vMatrix), vMatrix, GL_STATIC_DRAW);
		//A matrix is 4 vec4s
		glEnableVertexAttribArray(3 + 0);
		glEnableVertexAttribArray(3 + 1);
		glEnableVertexAttribArray(3 + 2);
		glEnableVertexAttribArray(3 + 3);

		glVertexAttribPointer(3 + 0, 4, GL_FLOAT, GL_FALSE, sizeof(Matrix), (GLvoid*)(offsetof(Matrix, a0)));
		glVertexAttribPointer(3 + 1, 4, GL_FLOAT, GL_FALSE, sizeof(Matrix), (GLvoid*)(offsetof(Matrix, b0)));
		glVertexAttribPointer(3 + 2, 4, GL_FLOAT, GL_FALSE, sizeof(Matrix), (GLvoid*)(offsetof(Matrix, c0)));
		glVertexAttribPointer(3 + 3, 4, GL_FLOAT, GL_FALSE, sizeof(Matrix), (GLvoid*)(offsetof(Matrix, d0)));
		//Only apply one per instance
		glVertexAttribDivisor(3 + 0, 1);
		glVertexAttribDivisor(3 + 1, 1);
		glVertexAttribDivisor(3 + 2, 1);
		glVertexAttribDivisor(3 + 3, 1);

		//Method 2. Use Uniform Buffers. Not shown here
	}

	void GenerateArrayTexture()
	{
		//Generate an array texture
		glGenTextures(1, &gArrayTexture);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D_ARRAY, gArrayTexture);

		//Create storage for the texture. (100 layers of 1x1 texels)
		glTexStorage3D(GL_TEXTURE_2D_ARRAY,
			1,                    //No mipmaps as textures are 1x1
			GL_RGB8,              //Internal format
			1, 1,                 //width,height
			100                   //Number of layers
		);

		for (unsigned int i(0); i != 100; ++i)
		{
			//Choose a random color for the i-essim image
			GLubyte color[3] = { GLubyte(rand() % 255),GLubyte(rand() % 255),GLubyte(rand() % 255) };

			//Specify i-essim image
			glTexSubImage3D(GL_TEXTURE_2D_ARRAY,
				0,                     //Mipmap number
				0, 0, i,               //xoffset, yoffset, zoffset
				1, 1, 1,               //width, height, depth
				GL_RGB,                //format
				GL_UNSIGNED_BYTE,      //type
				color);                //pointer to data
		}

		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}

	GLuint CompileShaders(const GLchar** vertexShaderSource, const GLchar** fragmentShaderSource)
	{
		//Compile vertex shader
		GLuint vertexShader(glCreateShader(GL_VERTEX_SHADER));
		glShaderSource(vertexShader, 1, vertexShaderSource, NULL);
		glCompileShader(vertexShader);

		int success;
		char infoLog[1024];
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(vertexShader, 1024, NULL, infoLog);
			std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}

		//Compile fragment shader
		GLuint fragmentShader(glCreateShader(GL_FRAGMENT_SHADER));
		glShaderSource(fragmentShader, 1, fragmentShaderSource, NULL);
		glCompileShader(fragmentShader);

		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(fragmentShader, 1024, NULL, infoLog);
			std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}

		//Link vertex and fragment shader together
		GLuint program(glCreateProgram());
		glAttachShader(program, vertexShader);
		glAttachShader(program, fragmentShader);
		glLinkProgram(program);

		//Delete shaders objects
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		return program;
	}

	void generateDrawCommands()
	{
		//Generate draw commands
		DrawElementsCommand vDrawCommand[100];
		GLuint baseVert = 0;
		for (unsigned int i(0); i < 100; ++i)
		{
			//quad
			if (i % 2 == 0)
			{
				vDrawCommand[i].vertexCount = 12;		//4 triangles = 12 vertices
				vDrawCommand[i].instanceCount = 1;		//Draw 1 instance
				vDrawCommand[i].firstIndex = 0;			//Draw from index 0 for this instance
				vDrawCommand[i].baseVertex = baseVert;	//Starting from baseVert
				vDrawCommand[i].baseInstance = i;		//gl_InstanceID
				baseVert += 5;
			}
			//triangle
			else
			{
				vDrawCommand[i].vertexCount = 3;		//1 triangle = 3 vertices
				vDrawCommand[i].instanceCount = 1;		//Draw 1 instance
				vDrawCommand[i].firstIndex = 0;			//Draw from index 0 for this instance
				vDrawCommand[i].baseVertex = baseVert;	//Starting from baseVert
				vDrawCommand[i].baseInstance = i;		//gl_InstanceID
				baseVert += 3;
			}
		}

		//feed the draw command data to the gpu
		glBindBuffer(GL_DRAW_INDIRECT_BUFFER, gIndirectBuffer);
		glBufferData(GL_DRAW_INDIRECT_BUFFER, sizeof(vDrawCommand), vDrawCommand, GL_DYNAMIC_DRAW);

		//feed the instance id to the shader.
		glBindBuffer(GL_ARRAY_BUFFER, gIndirectBuffer);
		glEnableVertexAttribArray(2);
		glVertexAttribIPointer(2, 1, GL_UNSIGNED_INT, sizeof(DrawElementsCommand), (void*)(offsetof(DrawElementsCommand, baseInstance)));
		glVertexAttribDivisor(2, 1); //only once per instance
	}

	void Run(void)
	{
		Application app;
		app.Init();

		//// glfw: initialize and configure
		//// ------------------------------
		//glfwInit();
		//glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		//glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
		//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		//// glfw window creation
		//// --------------------
		//GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
		//if (window == NULL)
		//{
		//	std::cout << "Failed to create GLFW window" << std::endl;
		//	glfwTerminate();
		//	return;
		//}
		//glfwMakeContextCurrent(window);
		//glfwSetCursorPosCallback(window, cursor_position_callback);

		////glewInit();

		////Set clear color
		//glClearColor(1.0, 1.0, 1.0, 0.0);

		////Create and bind the shader program
		//gProgram = CompileShaders(gVertexShaderSource, gFragmentShaderSource);
		//glUseProgram(gProgram);

		//GenerateGeometry();
		//GenerateArrayTexture();

		////Set the sampler for the texture.
		////Hacky but we know that the arraysampler is at bindingpoint 0.
		//glUniform1i(0, 0);

		////Generate one indirect draw buffer
		//glGenBuffers(1, &gIndirectBuffer);
		//// render loop
		//int i = 0;
		//while (!glfwWindowShouldClose(window))
		//{
		//	// input
		//	processInput(window);

		//	glClear(GL_COLOR_BUFFER_BIT);

		//	// Use program. Not needed in this example since we only have one that
		//	// we already use
		//	//glUseProgram(gProgram);

		//	// Bind the vertex array we want to draw from. Not needed in this example
		//	// since we only have one that is already bounded
		//	//glBindVertexArray(gVAO);

		//	generateDrawCommands();

		//	//populate light uniform
		//	glUniform2f(glGetUniformLocation(gProgram, "light_pos"), gMouseX, gMouseY);

		//	//draw
		//	glMultiDrawElementsIndirect(GL_TRIANGLES, //type
		//		GL_UNSIGNED_INT, //indices represented as unsigned ints
		//		(GLvoid*)0, //start with the first draw command
		//		100, //draw 100 objects
		//		0); //no stride, the draw commands are tightly packed

		//	// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		//	glfwSwapBuffers(window);
		//	glfwPollEvents();
		//}

		//// glfw: terminate, clearing all previously allocated GLFW resources.
		//glfwTerminate();
		////Clean-up
		//glDeleteProgram(gProgram);
		//glDeleteVertexArrays(1, &gVAO);
		//glDeleteBuffers(1, &gVertexBuffer);
		//glDeleteBuffers(1, &gElementBuffer);
		//glDeleteBuffers(1, &gMatrixBuffer);
		//glDeleteBuffers(1, &gIndirectBuffer);
		//glDeleteTextures(1, &gArrayTexture);

		app.Update();
		app.Exit();
	}

	// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
	// ---------------------------------------------------------------------------------------------------------
	void processInput(GLFWwindow* window)
	{
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);
	}

	void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
	{
		gMouseX = -0.5f + float(xpos) / float(SCR_WIDTH);
		gMouseY = 0.5f - float(ypos) / float(SCR_HEIGHT);
	}

	void Engine::Update(void)
	{
		if (Input::KeyTriggered(KeyCode::LeftControl))
		{
			(++showcaseIndex) %= 2;

			auto batch_status = [](b8 active)
			{
				//for (u64 i = 0; i < MAX_BATCH; ++i)
				//{
				//	if (!Coordinator::Instance()->HasComponent<EntityData>(*(batchShowCase + i)))
				//		continue;
				//	Coordinator::Instance()->GetComponent<EntityData>(*(batchShowCase + i)).active = active;
				//}
			};

			auto physic_status = [](b8 active)
			{
				for (u64 i{}; i < 4; ++i)
				{
					if ( !Coordinator::Instance()->HasComponent<EntityData>(*(walls + i)) )
						continue;
					Coordinator::Instance()->GetComponent<EntityData>(*(walls + i)).active = active;
				}
				if ( !Coordinator::Instance()->HasComponent<EntityData>(player) )
					return;
				Coordinator::Instance()->GetComponent<EntityData>(player).active = active;
			};

			switch (showcaseIndex)
			{
				case 0: // physics showcase
				{
					physic_status(true);
					batch_status(false);
					break;
				}
				case 1: // batch showcase
				{
					physic_status(false);
					batch_status(true);
					break;
				}
			}
		}
		showcase[showcaseIndex]();

		if (Input::KeyTriggered(KeyCode::Key_8))
		{
			Transform xform{ Math::Vector2(Random::Range(-300.0f, 300.f), Random::Range(-300.0f, 300.f)),
							Math::Vector2(50.f, 50.f), 0.f };
			ECS::Entity GO = Coordinator::Instance()->CreateEntity();
			ECS::CreateSprite(GO, xform);

			Sprite& sprite2 = Coordinator::Instance()->GetComponent<Sprite>(GO);
			sprite2.color = Color{ Random::Range(0.0f, 1.0f), Random::Range(0.0f, 1.0f), Random::Range(0.0f, 1.0f), 1.0f };
		}
	}

	void Engine::FixedUpdate(void)
	{
		// Raycast2DCollision({ -25, 25 }, { 25, 25 });
		UpdateRigidbodySystem();
		UpdateColliderSystem();
	}
}