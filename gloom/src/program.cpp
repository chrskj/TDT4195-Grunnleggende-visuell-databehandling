#include <vector>
// Local headers
#include "program.hpp"
#include "gloom/gloom.hpp"
#include "gloom/shader.hpp"

const float DEG2RAD = 3.14159/180;
 
void setup_vao(float *ver_ptr, uint ver_count, uint *ind_ptr, uint ind_count)
{
    // Vertex array object
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Vertex buffer object
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, ver_count, ver_ptr, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    // index buffer object
    GLuint ibo;
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, ind_count, ind_ptr, GL_STATIC_DRAW);
}

void runProgram(GLFWwindow* window)
{
    // Enable depth (Z) buffer (accept "closest" fragment)
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // Configure miscellaneous OpenGL settings
    glEnable(GL_CULL_FACE);

    // Set default colour after clearing the colour buffer
    glClearColor(0.3f, 0.5f, 0.8f, 1.0f);

    // Data
    /*
    float vertices[] = {
        0.6f, -0.8f, -1.2f,
        0.0f, 0.4f, 0.0f,
        -0.8f, -0.2f, 1.2f,
    };
    
    uint indices[] = {
        0, 1, 2, 
    };
    */

    float vertices[] = {
        1.0f, 0.8f, 0.0f, 0.9f, 1.0f, 0.0f, 0.8f, 0.8f, 0.0f,
        1.0f, -1.0f, 0.0f, 0.9f, -0.8f, 0.0f, 0.8f, -1.0f, 0.0f,
        0.1f, -0.1f, 0.0f, 0.0f, 0.1f, 0.0f, -0.1f, -0.1f, 0.0f,
    };

    uint indices[] = {
        0, 1, 2, 
        3, 4, 5, 
        6, 7, 8, 
    };

    setup_vao(vertices, sizeof(vertices), indices, sizeof(indices));

    // shader
    Gloom::Shader shader;
    shader.makeBasicShader("../gloom/shaders/simple.vert", "../gloom/shaders/simple.frag");
    shader.activate();

    // Rendering Loop
    while (!glfwWindowShouldClose(window))
    {
        // Clear colour and depth buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Draw your scene here
        glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(uint), GL_UNSIGNED_INT, 0);

        // Handle other events
        glfwPollEvents();
        handleKeyboardInput(window);

        // Flip buffers
        glfwSwapBuffers(window);
    }
    shader.deactivate();
    shader.destroy();
}


void handleKeyboardInput(GLFWwindow* window)
{
    // Use escape key for terminating the GLFW window
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}
