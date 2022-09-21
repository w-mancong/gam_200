#include "pch.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

using namespace ALEngine;

Engine::Camera mycam(Math::vec3(0.0f, 0.0f, 155.0f));

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

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
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile shaders
    // -------------------------
    Graphics::Shader shader("10.1.instancing.vs", "10.1.instancing.fs");
    //Shader shader("10.1.instancing.vs", "10.1.instancing.fs");

    unsigned int instanceVBO;
    glGenBuffers(1, &instanceVBO);
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Math::mat4) * 100 + sizeof(Math::vec4) * 100, nullptr, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    //unsigned int colorVBO;
    //glGenBuffers(1, &colorVBO);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(Math::vec4) * 100, nullptr, GL_DYNAMIC_DRAW);
    //glBindBuffer(GL_ARRAY_BUFFER, 0);

    float position[] =
    {
        -0.5f,  0.5f,   // top left
        -0.5f, -0.5f,   // btm left
         0.5f,  0.5f,   // top right
         0.5f, -0.5f,   // btm right
    };

    //float color[] =
    //{
    //    1.0f, 0.0f, 0.0f,
    //    0.0f, 1.0f, 0.0f,
    //    0.0f, 0.0f, 1.0f,
    //    0.0f, 1.0f, 1.0f
    //};

    unsigned int indices[] =
    {
        0, 1, 2, 3
    };

    unsigned int quadVAO, quadVBO, ebo;
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO); glGenBuffers(1, &ebo);
    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(position), nullptr, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(position), position);
    //glBufferSubData(GL_ARRAY_BUFFER, sizeof(position), sizeof(color), color);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    // -------------------------- Element Buffer Object ---------------------------------
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    //glBindBuffer(GL_ARRAY_BUFFER, colorVBO);

    //glBindBuffer(GL_ARRAY_BUFFER, 0);

    // also set instance data
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO); // this attribute comes from a different vertex buffer

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Math::vec4), (void*)(sizeof(Math::mat4) * 100));
    glVertexAttribDivisor(1, 1);

    u64 const index{ 2 }, max{ index + 4 };
    for (u64 i = index; i < max; ++i)
    {
        glEnableVertexAttribArray(i);
        glVertexAttribPointer(i, 4, GL_FLOAT, GL_FALSE, sizeof(Math::mat4), (void*)(sizeof(Math::vec4) * (i - index)));
        glVertexAttribDivisor(i, 1);
    }
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    Math::mat4 mat[100]{};
    for (size_t i = 0; i < 100; ++i)
    {
        *(mat + i) = Math::mat4::Model({ Random::Range(-75.0f, 75.0f), Random::Range(-75.0f, 75.0f), 0.0f },
            { Random::Range(10.0f, 50.0f), Random::Range(10.0f, 50.0f), 1.0f },
            Random::Range(0.0f, 360.0f));
    }

    Math::vec4 color[2] = {
            {   1.0f, 0.0f, 0.0f, 1.0f  },
            {   0.0f, 1.0f, 0.0f, 1.0f  }
    };
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Math::mat4) * 100, mat);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(Math::mat4) * 100, sizeof(Math::vec4) * 100, color);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    //glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
    //glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Math::vec4) * 100, color);
    //glBindBuffer(GL_ARRAY_BUFFER, 0);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // render
        // ------
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        Math::mat4 projection = Math::mat4::Perspective(45.0f, (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f);
        Math::mat4 view = mycam.ViewMatrix();

        // draw 100 instanced quads
        shader.use();
        shader.Set("projection", projection);
        shader.Set("view", view);
        glBindVertexArray(quadVAO);
        glDrawElementsInstanced(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, nullptr, 2);
        glBindVertexArray(0);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &quadVAO);
    glDeleteBuffers(1, &quadVBO);

    glfwTerminate();
    return 0;
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}