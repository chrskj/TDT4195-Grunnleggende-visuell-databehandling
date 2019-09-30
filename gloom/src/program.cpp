// Local headers
#include "program.hpp"
#include "gloom/gloom.hpp"
#include "gloom/shader.hpp"
#include "glm/ext.hpp"

const float DEG2RAD = 3.14159/180;

float x_strafe = 0;
float y_strafe = 0;
float z_strafe = 3;
glm::vec3 current_translation = glm::vec3(x_strafe, y_strafe, z_strafe);

float x_rot = 0.0f;
float y_rot = 0.0f;
float z_rot = 0.0f;
glm::vec3 current_rotation = glm::vec3(x_rot, y_rot, z_rot);
 
void setup_vao(float *ver_ptr, unsigned int ver_count, unsigned int *ind_ptr, unsigned int ind_count,
        float *clr_ver_ptr, unsigned int clr_count)
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

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);

    // Color buffer object
    GLuint cbo;
    glGenBuffers(1, &cbo);
    glBindBuffer(GL_ARRAY_BUFFER, cbo);
    glBufferData(GL_ARRAY_BUFFER, clr_count, clr_ver_ptr,
            GL_STATIC_DRAW);

    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(1);


    // index buffer object
    GLuint ibo;
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, ind_count, ind_ptr, GL_STATIC_DRAW);
}

std::ostream &operator<< (std::ostream &out, const glm::mat4x4 &m) {
	out << "{" << "\n"
		<< m[0][0] << "\t" << m[0][1] << "\t" << m[0][2] << "\t" << m[0][3] << "\n"
		<< m[1][0] << "\t" << m[1][1] << "\t" << m[1][2] << "\t" << m[1][3] << "\n"
		<< m[2][0] << "\t" << m[2][1] << "\t" << m[2][2] << "\t" << m[2][3] << "\n"
		<< m[3][0] << "\t" << m[3][1] << "\t" << m[3][2] << "\t" << m[3][3] << "\n"
		<< "}";

	return out;
}

void runProgram(GLFWwindow* window)
{
    // Enable transparancy
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Enable depth (Z) buffer (accept "closest" fragment)
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // Configure miscellaneous OpenGL settings
    glEnable(GL_CULL_FACE);

    // Set default colour after clearing the colour buffer
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // Data
	float vertices[] = {
        -1.0f,-1.0f,-1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f,-1.0f, 1.0f,
        
		1.0f, 1.0f,-1.0f,
        -1.0f, 1.0f,-1.0f,
        -1.0f,-1.0f,-1.0f,
        
		1.0f,-1.0f, 1.0f,
        1.0f,-1.0f,-1.0f,
        -1.0f,-1.0f,-1.0f,
        
		1.0f, 1.0f,-1.0f,
        -1.0f,-1.0f,-1.0f,
        1.0f,-1.0f,-1.0f,
        
		-1.0f,-1.0f,-1.0f,
        -1.0f, 1.0f,-1.0f,
        -1.0f, 1.0f, 1.0f,
        
		1.0f,-1.0f, 1.0f,
        -1.0f,-1.0f,-1.0f,
        -1.0f,-1.0f, 1.0f,
        
		-1.0f, 1.0f, 1.0f,
        1.0f,-1.0f, 1.0f,
        -1.0f,-1.0f, 1.0f,
        
		1.0f, 1.0f, 1.0f,
        1.0f, 1.0f,-1.0f,
        1.0f,-1.0f,-1.0f,
        
		1.0f,-1.0f,-1.0f,
        1.0f,-1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        
		1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f,-1.0f,
        1.0f, 1.0f,-1.0f,
        
		1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f,-1.0f,

        1.0f, 1.0f, 1.0f,
        1.0f,-1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,
    };

    float vertix_colors[] = {
        1.583f,  0.771f,  0.014f, 1.0f,
        0.609f,  0.115f,  0.436f, 1.0f,
        0.327f,  0.483f,  0.844f, 1.0f,
        0.822f,  0.569f,  0.201f, 1.0f,
        0.435f,  0.602f,  0.223f, 1.0f,
        0.310f,  0.747f,  0.185f, 1.0f,
        0.597f,  0.770f,  0.761f, 1.0f,
        0.559f,  0.436f,  0.730f, 1.0f,
        0.359f,  0.583f,  0.152f, 1.0f,
        0.483f,  0.596f,  0.789f, 1.0f,
        0.559f,  0.861f,  0.639f, 1.0f,
        0.195f,  0.548f,  0.859f, 1.0f,
        0.014f,  0.184f,  0.576f, 1.0f,
        0.771f,  0.328f,  0.970f, 1.0f,
        0.406f,  0.615f,  0.116f, 1.0f,
        0.676f,  0.977f,  0.133f, 1.0f,
        0.971f,  0.572f,  0.833f, 1.0f,
        0.140f,  0.616f,  0.489f, 1.0f,
        0.997f,  0.513f,  0.064f, 1.0f,
        0.945f,  0.719f,  0.592f, 1.0f,
        0.543f,  0.021f,  0.978f, 1.0f,
        0.279f,  0.317f,  0.505f, 1.0f,
        0.167f,  0.620f,  0.077f, 1.0f,
        0.347f,  0.857f,  0.137f, 1.0f,
        0.055f,  0.953f,  0.042f, 1.0f,
        0.714f,  0.505f,  0.345f, 1.0f,
        0.783f,  0.290f,  0.734f, 1.0f,
        0.722f,  0.645f,  0.174f, 1.0f,
        0.302f,  0.455f,  0.848f, 1.0f,
        0.225f,  0.587f,  0.040f, 1.0f,
        0.517f,  0.713f,  0.338f, 1.0f,
        0.053f,  0.959f,  0.120f, 1.0f,
        0.393f,  0.621f,  0.362f, 1.0f,
        0.673f,  0.211f,  0.457f, 1.0f,
        0.820f,  0.883f,  0.371f, 1.0f,
        0.982f,  0.099f,  0.879f, 1.0f,
    };

    unsigned int indices[] = {
        0, 1, 2,
        3, 4, 5, 
        6, 7, 8,
        9, 10, 11, 
        12, 13, 14,
        15, 16, 17, 
        18, 19, 20,
        21, 22, 23, 
        24, 25, 26,
        27, 28, 29, 
        30, 31, 32,
        33, 34, 35, 
    };

    /* 8 trekanter
  float vertices[] = {
        1.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f, 0.0f, 1.0f,
        -1.0f, -1.0f, 1.0f,
        0.0f, -1.0f, 1.0f,
        1.0f, -1.0f, 1.0f,
        1.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 0.0f,
    };

    float vertix_colors[] = {
        1.583f,  0.771f,  0.014f, 1.0f,
        0.609f,  0.115f,  0.436f, 1.0f,
        0.327f,  0.483f,  0.844f, 1.0f,
        0.822f,  0.569f,  0.201f, 1.0f,
        0.435f,  0.602f,  0.223f, 1.0f,
        0.310f,  0.747f,  0.185f, 1.0f,
        0.597f,  0.770f,  0.761f, 1.0f,
        0.559f,  0.436f,  0.730f, 1.0f,
        0.359f,  0.583f,  0.152f, 1.0f,
    };

    unsigned int indices[] = {
        0, 1, 8,
        1, 2, 8, 
        2, 3, 8,
        3, 4, 8, 
        4, 5, 8,
        5, 6, 8, 
        6, 7, 8,
        7, 0, 8, 
    };
    */
        
    /* overlappende trekanter
    float vertices[] = {
        0.6f, 1.0f, 0.75f,
        -0.6f, 1.0f, 0.75f,
        0.0f, -0.6f, 0.75f,
        -1.0f, 0.6f, 0.5f,
        -1.0f, -0.6f, 0.5f,
        0.6f, 0.0f, 0.5f,
        1.0f, -0.6f, 0.25f,
        1.0f, 0.6f, 0.25f,
        -0.6f, 0.0f, 0.25f,
    };

    float vertix_colors[] = {
        1.0f, 0.0f, 0.0f, 0.5f,
        1.0f, 0.0f, 0.0f, 0.5f,
        1.0f, 0.0f, 0.0f, 0.5f,
        0.0f, 1.0f, 0.0f, 0.5f,
        0.0f, 1.0f, 0.0f, 0.5f,
        0.0f, 1.0f, 0.0f, 0.5f,
        0.0f, 0.0f, 1.0f, 0.5f,
        0.0f, 0.0f, 1.0f, 0.5f,
        0.0f, 0.0f, 1.0f, 0.5f,
    };

    unsigned int indices[] = {
        0,1,2,
        3,4,5,
        6,7,8,
        9,10,11
    };
    */

    setup_vao(vertices, sizeof(vertices), indices, sizeof(indices),
            vertix_colors, sizeof(vertix_colors));

    // shader
    Gloom::Shader shader;
    shader.makeBasicShader("../../../gloom/shaders/simple.vert", "../../../gloom/shaders/simple.frag");
    shader.activate();

	// Matrix transformation
	glm::mat4 id_mat = glm::mat4(1.f);
	glm::mat4 correction_mat = glm::mat4x4(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, -1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);
	glm::mat4x4 update_translation = glm::translate(id_mat, current_translation);
	glm::mat4x4 update_rotation_x = glm::rotate(id_mat, x_rot, current_rotation);
	glm::mat4x4 update_rotation_y = glm::rotate(id_mat, y_rot, current_rotation);
	glm::mat4x4 update_rotation_z = glm::rotate(id_mat, z_rot, current_rotation);
	glm::mat4x4 model_mat = id_mat * update_translation;
	glm::mat4x4 projection_mat = glm::perspective(30.0f, 1.0f, 1.0f, 100.0f);

    // Rendering Loop
    while (!glfwWindowShouldClose(window))
    {
        // Clear colour and depth buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Matrix transformation
		current_translation = glm::vec3(x_strafe, y_strafe, z_strafe);
		update_translation = glm::translate(id_mat, current_translation);

		current_rotation = glm::vec3(x_rot, y_rot, z_rot);
		update_rotation_x = glm::rotate(id_mat, x_rot, glm::vec3(1,0,0));
		update_rotation_y = glm::rotate(id_mat, y_rot, glm::vec3(0,1,0));
		update_rotation_z = glm::rotate(id_mat, z_rot, glm::vec3(0,0,1));

		model_mat = correction_mat*update_translation*update_rotation_x*update_rotation_y*update_rotation_z;
		//std::cout << update_rotation << std::endl;

		// Send array of 4x4-matrix values to shader
		glUniformMatrix4fv(2, 1, GL_FALSE, &model_mat[0][0]);
		glUniformMatrix4fv(3, 1, GL_FALSE, &projection_mat[0][0]);

        // Draw your scene here
        glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(unsigned int),
                GL_UNSIGNED_INT, 0);

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
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
		z_strafe = z_strafe - 0.01f;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
		z_strafe = z_strafe + 0.01f;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
		x_strafe = x_strafe - 0.01f;
    }
   if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
		x_strafe = x_strafe + 0.01f;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
    {
		y_strafe = y_strafe - 0.01f;
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
		y_strafe = y_strafe + 0.01f;
    }
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
		x_rot = x_rot - 0.01f;
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
		x_rot = x_rot + 0.01f;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
    {
		y_rot = y_rot - 0.01f;
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    {
		y_rot = y_rot + 0.01f;
    }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    {
		z_rot = z_rot - 0.01f;
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
    {
		z_rot = z_rot + 0.01f;
    }
}
