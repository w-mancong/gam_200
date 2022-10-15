#include "pch.h"

void processInput(GLFWwindow* window);
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

using namespace ALEngine;
namespace
{
    struct Vertex2D
    {
        float x, y, z;  //Position
        float u, v;     //Uv
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
        //xy			            //uv
        { -0.5f, -0.5f,   0.0f,	    0.0f, 0.0f },   // btm left
        {  0.5f, -0.5f,   0.0f,	    1.0f, 0.0f },   // btm right
        { -0.5f,  0.5f,   0.0f,	    0.0f, 1.0f },   // top left
        {  0.5f,  0.5f,   0.0f,	    1.0f, 1.0f }    // top right
    };

    const std::vector<Vertex2D> gTriangle =
    {
        { 0.0f, 0.0f,	0.0f,0.0f},
        { 0.05f, 0.1f,	0.5f,1.0f},
        { 0.1f, 0.0f,	1.0f,0.0f}
    };

    const std::vector<unsigned int> gQuadIndex = {
        3, 2, 0,
        0, 1, 3
    };

    const std::vector<unsigned int> gTriangleIndex =
    {
        0,1,2
    };

    const GLchar* gVertexShaderSource[] =
    {
        "#version 430 core\n"

        "layout (location = 0 ) in vec3 position;\n"
        "layout (location = 1 ) in vec2 texCoord;\n"
        "layout (location = 2 ) in uint drawid;\n"
        "layout (location = 3 ) in mat4 instanceMatrix;\n"

        "layout (location = 0 ) out vec2 uv;\n"
        "layout (location = 1 ) flat out uint drawID;\n"

        "uniform mat4 view, proj;\n"

        "void main(void)\n"
        "{\n"
        "  uv = texCoord;\n"
        "  drawID = drawid;\n"
        "  gl_Position = proj * view * instanceMatrix * vec4(position, 1.0);\n"
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
    GLuint gColorBuffer(0);
    GLuint gTextureIndexBuffer(0);
    GLuint gProgram(0);

    float gMouseX(0);
    float gMouseY(0);

    u64 constexpr TEST_SIZE{ 500 };

    Math::mat4 vMatrix[TEST_SIZE];
    Math::vec4 vColor[TEST_SIZE];
    u64 texIndex[TEST_SIZE];

}//Unnamed namespace

void GenerateGeometry()
{
    //Generate 50 quads, 50 triangles
    const unsigned num_vertices = gQuad.size() * TEST_SIZE;
    std::vector<Vertex2D> vVertex(num_vertices);
    unsigned vertexIndex(0);
    // populate geometry
    for (size_t i{}; i < TEST_SIZE; ++i)
    {
        for (size_t j{}; j != gQuad.size(); ++j)
        {
            vVertex[vertexIndex++] = gQuad[j];
        }
    }

    for (size_t i = 0; i < TEST_SIZE; ++i)
    {
        *(vMatrix + i) = Math::mat4::ModelT({ Random::Range(-400.0f, 400.0f), Random::Range(-300.0f, 300.0f), 0.0f },
            { Random::Range(20.0f, 50.0f), Random::Range(20.0f, 50.0f), 1.0f }, 0.0f);
        *(vColor + i) = Math::vec4{ Random::Range(0.0f, 1.0f), Random::Range(0.0f, 1.0f), Random::Range(0.0f, 1.0f), Random::Range(0.0f, 1.0f) };
    }

    glGenVertexArrays(1, &gVAO);
    glBindVertexArray(gVAO);
    //Create a vertex buffer object
    glGenBuffers(1, &gVertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, gVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex2D) * vVertex.size(), vVertex.data(), GL_STATIC_DRAW);

    //Specify vertex attributes for the shader
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), (GLvoid*)(offsetof(Vertex2D, x)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), (GLvoid*)(offsetof(Vertex2D, u)));

    //Create an element buffer and populate it
    int quad_bytes = sizeof(unsigned int) * gQuadIndex.size();
    glGenBuffers(1, &gElementBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gElementBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, quad_bytes, NULL, GL_STATIC_DRAW);

    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, quad_bytes, gQuadIndex.data());

    //Setup per instance matrices
    //Method 1. Use Vertex attributes and the vertex attrib divisor
    glGenBuffers(1, &gMatrixBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, gMatrixBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vMatrix), vMatrix, GL_DYNAMIC_DRAW);

    u32 constexpr colorLayout{ 3 }, matrixLayout{ 4 };

    //A matrix is 4 vec4s
    glEnableVertexAttribArray(matrixLayout + 0);
    glEnableVertexAttribArray(matrixLayout + 1);
    glEnableVertexAttribArray(matrixLayout + 2);
    glEnableVertexAttribArray(matrixLayout + 3);

    glVertexAttribPointer(matrixLayout + 0, 4, GL_FLOAT, GL_FALSE, sizeof(Math::mat4), (GLvoid*)(offsetof(Math::mat4, mat[0])));
    glVertexAttribPointer(matrixLayout + 1, 4, GL_FLOAT, GL_FALSE, sizeof(Math::mat4), (GLvoid*)(offsetof(Math::mat4, mat[1])));
    glVertexAttribPointer(matrixLayout + 2, 4, GL_FLOAT, GL_FALSE, sizeof(Math::mat4), (GLvoid*)(offsetof(Math::mat4, mat[2])));
    glVertexAttribPointer(matrixLayout + 3, 4, GL_FLOAT, GL_FALSE, sizeof(Math::mat4), (GLvoid*)(offsetof(Math::mat4, mat[3])));
    //Only apply one per instance
    glVertexAttribDivisor(matrixLayout + 0, 1);
    glVertexAttribDivisor(matrixLayout + 1, 1);
    glVertexAttribDivisor(matrixLayout + 2, 1);
    glVertexAttribDivisor(matrixLayout + 3, 1);

    glGenBuffers(1, &gColorBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, gColorBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vColor), vColor, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(colorLayout);
    glVertexAttribPointer(colorLayout, 4, GL_FLOAT, GL_FALSE, sizeof(Math::vec4), (GLvoid*)(offsetof(Math::vec4, x)));
    glVertexAttribDivisor(colorLayout, 1);

    glBindVertexArray(0);

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
    DrawElementsCommand vDrawCommand[TEST_SIZE];
    GLuint baseVert = 0;
    for (unsigned int i(0); i < TEST_SIZE; ++i)
    {
        vDrawCommand[i].vertexCount = 6;		//4 triangles = 12 vertices
        vDrawCommand[i].instanceCount = 1;		//Draw 1 instance
        vDrawCommand[i].firstIndex = 0;			//Draw from index 0 for this instance
        vDrawCommand[i].baseVertex = baseVert;	//Starting from baseVert
        vDrawCommand[i].baseInstance = i;		//gl_InstanceID
        baseVert += gQuad.size();
    }

    //for (size_t i = 0; i < 100; ++i)
    //    (vDrawCommand + i)->color = { Random::Range(0.0f, 1.0f), Random::Range(0.0f, 1.0f), Random::Range(0.0f, 1.0f) };

    //feed the draw command data to the gpu
    glBindBuffer(GL_DRAW_INDIRECT_BUFFER, gIndirectBuffer);
    glBufferData(GL_DRAW_INDIRECT_BUFFER, sizeof(vDrawCommand), vDrawCommand, GL_DYNAMIC_DRAW);

    //for (size_t i = 0; i < TEST_SIZE; ++i)
    //{
    //    *(vMatrix + i) = Math::mat4::ModelT({ Random::Range(-400.0f, 400.0f), Random::Range(-300.0f, 300.0f), 0.0f },
    //        { Random::Range(20.0f, 50.0f), Random::Range(20.0f, 50.0f), 1.0f }, 0.0f);
    //    *(vColor + i) = Math::vec4{ Random::Range(0.0f, 1.0f), Random::Range(0.0f, 1.0f), Random::Range(0.0f, 1.0f), Random::Range(0.0f, 1.0f) };
    //}

    glBindBuffer(GL_ARRAY_BUFFER, gMatrixBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vMatrix), vMatrix, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, gColorBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vColor), vColor, GL_DYNAMIC_DRAW);

    //feed the instance id to the shader.
    glBindBuffer(GL_ARRAY_BUFFER, gIndirectBuffer);
    glEnableVertexAttribArray(2);
    glVertexAttribIPointer(2, 1, GL_UNSIGNED_INT, sizeof(DrawElementsCommand), (void*)(offsetof(DrawElementsCommand, baseInstance)));
    glVertexAttribDivisor(2, 1); //only once per instance
}

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        std::exit(EXIT_FAILURE);
    }

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //Set clear color
    glClearColor(0.25f, 0.34f, 0.51f, 0.0);

    //Create and bind the shader program
    //gProgram = CompileShaders(gVertexShaderSource, gFragmentShaderSource);
    //glUseProgram(gProgram);

    Graphics::Shader shader{ "Assets/Shaders/indirect.vert", "Assets/Shaders/indirect.frag" };

    GenerateGeometry();
    GenerateArrayTexture();

    //Set the sampler for the texture.
    //Hacky but we know that the arraysampler is at bindingpoint 0.
    glUniform1i(0, 0);

    Engine::Camera camera{ Math::Vector3(0.0f, 0.0f, 725.0f) };

    //Generate one indirect draw buffer
    glGenBuffers(1, &gIndirectBuffer);
    // render loop
    int i = 0;
    while (!glfwWindowShouldClose(window))
    {
        // input
        processInput(window);

        glClear(GL_COLOR_BUFFER_BIT);

        // Use program. Not needed in this example since we only have one that
        // we already use
        //glUseProgram(gProgram);
        //glUniformMatrix4fv(glGetUniformLocation(gProgram, "proj"), 1, GL_FALSE, camera.ProjectionMatrix().value_ptr());
        //glUniformMatrix4fv(glGetUniformLocation(gProgram, "view"), 1, GL_FALSE, camera.ViewMatrix().value_ptr());

        shader.use();
        shader.Set("proj", camera.ProjectionMatrix());
        shader.Set("view", camera.ViewMatrix());

        // Bind the vertex array we want to draw from. Not needed in this example
        // since we only have one that is already bounded
        glBindVertexArray(gVAO);

        generateDrawCommands();

        //populate light uniform
        glUniform2f(glGetUniformLocation(gProgram, "light_pos"), gMouseX, gMouseY);

        //draw
        glMultiDrawElementsIndirect(GL_TRIANGLES,   //type
            GL_UNSIGNED_INT,                        //indices represented as unsigned ints
            (GLvoid*)0,                             //start with the first draw command
            TEST_SIZE,                                    //draw 100 objects
            0);                                     //no stride, the draw commands are tightly packed

        glBindVertexArray(0);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    glfwTerminate();
    //Clean-up
    glDeleteProgram(gProgram);
    glDeleteVertexArrays(1, &gVAO);
    glDeleteBuffers(1, &gVertexBuffer);
    glDeleteBuffers(1, &gElementBuffer);
    glDeleteBuffers(1, &gMatrixBuffer);
    glDeleteBuffers(1, &gIndirectBuffer);
    glDeleteTextures(1, &gArrayTexture);
    return 0;
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

//int main(void)
//{
//	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
//
//	ALEngine::Engine::Run();
//	ALEngine::Memory::FreeAll();
//}