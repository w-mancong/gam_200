#include "pch.h"
#include "Time.h"

#include <iostream>

namespace ALEngine::Engine
{
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
            float x, y;         // Position
            float u, v;         // Uv
            float r, g, b, a;   // Color
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

        const GLchar* gVertexShaderSource[] =
        {
            "#version 430 core\n"

            "layout (location = 0 ) in vec2 position;\n"
            "layout (location = 1 ) in vec2 texCoord;\n"
            "layout (location = 2 ) in uint drawid;\n"
            "layout (location = 3 ) in vec4 color;\n" 
            "layout (location = 4 ) in mat4 instanceMatrix;\n"

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
        glEnableVertexAttribArray(4 + 0);
        glEnableVertexAttribArray(4 + 1);
        glEnableVertexAttribArray(4 + 2);
        glEnableVertexAttribArray(4 + 3);

        glVertexAttribPointer(4 + 0, 4, GL_FLOAT, GL_FALSE, sizeof(Matrix), (GLvoid*)(offsetof(Matrix, a0)));
        glVertexAttribPointer(4 + 1, 4, GL_FLOAT, GL_FALSE, sizeof(Matrix), (GLvoid*)(offsetof(Matrix, b0)));
        glVertexAttribPointer(4 + 2, 4, GL_FLOAT, GL_FALSE, sizeof(Matrix), (GLvoid*)(offsetof(Matrix, c0)));
        glVertexAttribPointer(4 + 3, 4, GL_FLOAT, GL_FALSE, sizeof(Matrix), (GLvoid*)(offsetof(Matrix, d0)));
        //Only apply one per instance
        glVertexAttribDivisor(4 + 0, 1);
        glVertexAttribDivisor(4 + 1, 1);
        glVertexAttribDivisor(4 + 2, 1);
        glVertexAttribDivisor(4 + 3, 1);

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
            GLubyte color[3] = { Random::Range(0, 255), Random::Range(0, 255), Random::Range(0, 255) };

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
                baseVert += gQuad.size();
            }
            //triangle
            else
            {
                vDrawCommand[i].vertexCount = 3;		//1 triangle = 3 vertices
                vDrawCommand[i].instanceCount = 1;		//Draw 1 instance
                vDrawCommand[i].firstIndex = 0;			//Draw from index 0 for this instance
                vDrawCommand[i].baseVertex = baseVert;	//Starting from baseVert
                vDrawCommand[i].baseInstance = i;		//gl_InstanceID
                baseVert += gTriangle.size();
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

	using namespace Math; using namespace Graphics; using namespace ECS;
	class Application
	{
	public:
		void Init(void);
		void Update(void);
		void Exit(void);
	};

	Entity en[2500];

	void Application::Init(void)
	{
		OpenGLWindow::InitGLFWWindow();
		ECS::InitSystem();

		for (u64 i = 0; i < 2500; ++i)
		{
			Transform t{ { Random::Range(-600.0f, 600.0f), Random::Range(-300.0f, 300.0f), 0.0f },
				{ Random::Range(20.0f, 75.0f), Random::Range(20.0f, 75.0f)},
				{ Random::Range(0.0f, 360.0f) } };

			*(en + i) = CreateSprite(t, "Assets/Images/awesomeface.png");
			Sprite& sprite = Coordinator::Instance()->GetComponent<Sprite>(*(en + i));
			sprite.color = Color{ Random::Range(0.0f, 1.0f), Random::Range(0.0f, 1.0f), Random::Range(0.0f, 1.0f), Random::Range(0.0f, 1.0f) };
		}

		//Transform t1{ { 0.0f, 150.0f, -200.0f }, { 50.0f, 50.0f }, 0.0f };
		//Transform t2{ { 0.0f, 0.0f, 0.0f }, { 50.0f, 50.0f }, 0.0f };
		//Transform t3{ { -150.0f, 0.0f, 0.0f }, { 50.0f, 50.0f }, 0.0f };

		//test = CreateSprite(t2, "Assets/Images/awesomeface.png");
		//CreateSprite(t1, "Assets/Images/awesomeface.png");
		//CreateSprite(t3, "Assets/Images/awesomeface.png");

		//Sprite& sprite = Coordinator::Instance()->GetComponent<Sprite>(test);
		//sprite.color = Color{ 0.4f, 0.5f, 0.25f, 0.85f };
		//sprite.layer = RenderLayer::UI;

		// Initialize Time (Framerate Controller)
		Time::Init();

		//// Init ImGui
		//ALEditor::Instance()->Init();
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

			//// Begin new ImGui frame
			//ALEditor::Instance()->Begin();

			// Normal Update
			Engine::Update();
			// Fixed Update (Physics)
			accumulator += Time::m_DeltaTime;
			while (accumulator >= Time::m_FixedDeltaTime)
			{
				Engine::FixedUpdate();
				accumulator -= Time::m_FixedDeltaTime;
			}

			// Render
			Render();

			//std::cout << Time::m_FPS << std::endl;

			// Wait for next frame
			Time::WaitUntil();
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
        //while (!glfwWindowShouldClose(OpenGLWindow::Window()))
        //{
        //    // input
        //    processInput(OpenGLWindow::Window());

        //    glClear(GL_COLOR_BUFFER_BIT);

        //    // Use program. Not needed in this example since we only have one that
        //    // we already use
        //    glUseProgram(gProgram);

        //    // Bind the vertex array we want to draw from. Not needed in this example
        //    // since we only have one that is already bounded
        //    glBindVertexArray(gVAO);

        //    generateDrawCommands();

        //    //populate light uniform
        //    glUniform2f(glGetUniformLocation(gProgram, "light_pos"), gMouseX, gMouseY);

        //    //draw
        //    glMultiDrawElementsIndirect(GL_TRIANGLES,   //type
        //        GL_UNSIGNED_INT,                        //indices represented as unsigned ints
        //        (GLvoid*)0,                             //start with the first draw command
        //        100,                                    //draw 100 objects
        //        0);                                     //no stride, the draw commands are tightly packed

        //    // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        //    glfwSwapBuffers(OpenGLWindow::Window());
        //    glfwPollEvents();
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
	}
		
	void Engine::Update(void)
	{
		f32 constexpr rot_spd{ 50.0f };
		for (u64 i = 0; i < ARRAY_SIZE(en); ++i)
		{
			Transform& trans = Coordinator::Instance()->GetComponent<Transform>(*(en + i));
			trans.rotation += rot_spd * Time::m_DeltaTime;
		}

		//Transform& trans = Coordinator::Instance()->GetComponent<Transform>(test);

		//f32 constexpr spd{ 150.0f };

		//if (Input::KeyDown(KeyCode::W))
		//{
		//	trans.position.y += spd * Time::m_DeltaTime;
		//}
		//if (Input::KeyDown(KeyCode::S))
		//{
		//	trans.position.y -= spd * Time::m_DeltaTime;
		//}
		//if (Input::KeyDown(KeyCode::D))
		//{
		//	trans.position.x += spd * Time::m_DeltaTime;
		//}
		//if (Input::KeyDown(KeyCode::A))
		//{
		//	trans.position.x -= spd * Time::m_DeltaTime;
		//}
	}
		
	void Engine::FixedUpdate(void)
	{
		//Raycast2DCollision({ -25, 25 }, { 25, 25 });
		UpdateRigidbodySystem();
		UpdateColliderSystem();
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
}