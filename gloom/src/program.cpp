// Local headers
#include "program.hpp"
#include "gloom/gloom.hpp"
#include "gloom/shader.hpp"
#include "glm/ext.hpp"
#include "mesh.hpp"
#include "OBJLoader.hpp"
#include "sceneGraph.hpp"
#include "toolbox.hpp"

// Global constants
const float DEG2RAD = 3.14159/180;

// Global variables
Mesh surface(loadTerrainMesh("../gloom/resources/lunarsurface.obj"));
Helicopter helicopter(loadHelicopterModel("../gloom/resources/helicopter.obj"));
Heading course;
float door_position = 0;

SceneNode* surface_node = createSceneNode();
SceneNode* h_body_node = createSceneNode();
SceneNode* h_mainRotor_node = createSceneNode();
SceneNode* h_tailRotor_node = createSceneNode();
SceneNode* h_door_node = createSceneNode();

glm::mat4 id_mat = glm::mat4(1.f);
glm::mat4 corr_mat = glm::mat4(
        -1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, -1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
        );

glm::vec3 rotation = glm::vec3(0, 0, 0);
glm::mat4x4 rotation_x = glm::rotate(id_mat, rotation.x, glm::vec3(1,0,0));
glm::mat4x4 rotation_y = glm::rotate(id_mat, rotation.y, glm::vec3(0,1,0));
glm::mat4x4 rotation_z = glm::rotate(id_mat, rotation.z, glm::vec3(0,0,1));
glm::vec3 position = glm::vec3(0, 0, 0);
glm::mat4x4 translation = glm::translate(id_mat, position);

glm::mat4x4 model = id_mat;
glm::mat4x4 view = id_mat;
glm::mat4x4 projection = glm::perspective(30.0f, 1.0f, 1.0f, 1000.0f);
 
std::ostream &operator<< (std::ostream &out, const glm::mat4x4 &m)
{
	out << "{" << "\n"
		<< m[0][0] << "\t" << m[0][1] << "\t" << m[0][2] << "\t" << m[0][3] << "\n"
		<< m[1][0] << "\t" << m[1][1] << "\t" << m[1][2] << "\t" << m[1][3] << "\n"
		<< m[2][0] << "\t" << m[2][1] << "\t" << m[2][2] << "\t" << m[2][3] << "\n"
		<< m[3][0] << "\t" << m[3][1] << "\t" << m[3][2] << "\t" << m[3][3] << "\n"
		<< "}";
	return out;
}

std::ostream &operator<< (std::ostream &out, const std::vector<float> &v)
{
    for (std::vector<float>::size_type i = 0; i < v.size(); i++) {
        out << v.at(i) << ", ";
    }
	return out;
}

std::ostream &operator<< (std::ostream &out, const std::vector<unsigned int> &v)
{
    for (std::vector<unsigned int>::size_type i = 0; i < v.size(); i++) {
        out << v.at(i) << ", ";
    }
	return out;
}

GLuint setup_vao(Mesh *m)
{
    // Vertex array object
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    
    // index buffer object
    GLuint ibo;
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m->indices.size()* sizeof(unsigned int), &(m->indices[0]), GL_STATIC_DRAW);

    // Vertex buffer object
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, m->vertices.size()*sizeof(float), &(m->vertices[0]), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);

    // Color buffer object
    GLuint cbo;
    glGenBuffers(1, &cbo);
    glBindBuffer(GL_ARRAY_BUFFER, cbo);
    glBufferData(GL_ARRAY_BUFFER, m->colours.size()* sizeof(float), &(m->colours[0]), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(1);
 
    // Normals buffer object
    GLuint nbo;
    glGenBuffers(1, &nbo);
    glBindBuffer(GL_ARRAY_BUFFER, nbo);
    glBufferData(GL_ARRAY_BUFFER, m->normals.size()* sizeof(float), &(m->normals[0]), GL_STATIC_DRAW);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
    return vao;
}

SceneNode* setup_scene_graph()
{
    SceneNode* root = createSceneNode();
    addChild(root, surface_node);
    addChild(surface_node, h_body_node);
    addChild(h_body_node, h_mainRotor_node);
    addChild(h_body_node, h_tailRotor_node);
    h_tailRotor_node->referencePoint = glm::vec3(0.35f, 2.3f, 10.4f);
    addChild(h_body_node, h_door_node);
    return root;
}

void drawSceneNode(SceneNode* root)
{
    // Draw here
    translation = glm::translate(id_mat, position);
    rotation_x = glm::rotate(id_mat, rotation.x, glm::vec3(1,0,0));
    rotation_y = glm::rotate(id_mat, rotation.y, glm::vec3(0,1,0));
    rotation_z = glm::rotate(id_mat, rotation.z, glm::vec3(0,0,1));
    view = corr_mat*rotation_x*rotation_y*rotation_z*translation;

	glUniformMatrix4fv(3, 1, GL_FALSE, &root->currentTransformationMatrix[0][0]);
	glUniformMatrix4fv(4, 1, GL_FALSE, &view[0][0]);
	glUniformMatrix4fv(5, 1, GL_FALSE, &projection[0][0]);
	glUniform3f(6, position.x, position.y, position.z);
    
    glBindVertexArray(root->vertexArrayObjectID);
    glDrawElements(GL_TRIANGLES, root->VAOIndexCount*sizeof(unsigned int), GL_UNSIGNED_INT, 0);

    for (SceneNode* child : root->children) {
        drawSceneNode(child);
    }
}

void updateSceneNode(SceneNode* node, glm::mat4 transformationThusFar, double time_step)
{
    // Do transformation computations here
    float rotor_speed = 2.0f*time_step;
    switch (node->vertexArrayObjectID)
    {
        case 1: // surface
            break;
        case 2: // body
            node->position.x = course.x;
            node->position.y = 20;
            node->position.z = course.z;
            node->rotation.z = course.roll;
            node->rotation.x = course.pitch;
            node->rotation.y = course.yaw;
            break;
        case 3: // main rotor
            node->rotation.y += rotor_speed;
            break;
        case 4: // tail rotor
            node->rotation.x += rotor_speed;
            break;
        case 5: // door
            node->position.z = door_position;
            break;
        default:
            break;
    }

    // Store matrix in the node's currentTransformationMatrix here
    glm::mat4 model_translate = glm::translate(id_mat, node->position);
    glm::mat4 model_reference = glm::translate(id_mat, node->referencePoint+node->position);
    glm::mat4 model_inv_reference = glm::translate(id_mat, -node->referencePoint-node->position);
    glm::mat4 model_rotate_x = glm::rotate(id_mat, node->rotation.x, glm::vec3(1,0,0));
    glm::mat4 model_rotate_y = glm::rotate(id_mat, node->rotation.y, glm::vec3(0,1,0));
    glm::mat4 model_rotate_z = glm::rotate(id_mat, node->rotation.z, glm::vec3(0,0,1));

    node->currentTransformationMatrix = transformationThusFar*model_reference
            *model_rotate_x*model_rotate_y*model_rotate_z*model_translate*model_inv_reference;
    glm::mat4 combinedTransformation = node->currentTransformationMatrix;

    for(SceneNode* child : node->children) {
        updateSceneNode(child, combinedTransformation, time_step);
    }
}

void runProgram(GLFWwindow* window)
{
    double game_time = getTimeDeltaSeconds();
    double total_time = game_time;

    // Enable transparancy
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Enable depth (Z) buffer (accept "closest" fragment)
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // Configure miscellaneous OpenGL settings
    glEnable(GL_CULL_FACE);

    // Set default colour after clearing the colour buffer
    glClearColor(0.3f, 0.5f, 0.8f, 1.0f);
   
    // Set up VAO for mesh objects
    GLuint surface_vao = setup_vao(&surface);
    surface_node->vertexArrayObjectID = surface_vao;
    surface_node->VAOIndexCount = surface.indices.size();

    GLuint h_body_vao = setup_vao(&helicopter.body);
    h_body_node->vertexArrayObjectID = h_body_vao;
    h_body_node->VAOIndexCount = helicopter.body.indices.size();
    
    GLuint h_mainRotor_vao = setup_vao(&helicopter.mainRotor);
    h_mainRotor_node->vertexArrayObjectID = h_mainRotor_vao;
    h_mainRotor_node->VAOIndexCount = helicopter.mainRotor.indices.size();
    
    GLuint h_tailRotor_vao = setup_vao(&helicopter.tailRotor);
    h_tailRotor_node->vertexArrayObjectID = h_tailRotor_vao;
    h_tailRotor_node->VAOIndexCount = helicopter.tailRotor.indices.size();

    GLuint h_door_vao = setup_vao(&helicopter.door);
    h_door_node->vertexArrayObjectID = h_door_vao;
    h_door_node->VAOIndexCount = helicopter.door.indices.size();
    
    // Set up scene graph
    SceneNode* root = setup_scene_graph();

    // shader
    Gloom::Shader shader;
    shader.makeBasicShader("../gloom/shaders/simple.vert", "../gloom/shaders/simple.frag");
    shader.activate();

    // Rendering Loop
    while (!glfwWindowShouldClose(window))
    {
        game_time = getTimeDeltaSeconds();
        total_time += game_time;

        // Clear colour and depth buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Draw your scene here
        for (int i = 0; i<5; i++) {
            course = simpleHeadingAnimation(total_time+i*3.5);
            updateSceneNode(root, model, game_time);
            drawSceneNode(root);
        }

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
    float trans_speed = 0.5f;
    float rotation_speed = 0.03f;

    // Use escape key for terminating the GLFW window
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
		position.z += trans_speed;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
		position.z -= trans_speed;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
		position.x += trans_speed;
    }
   if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
		position.x -= trans_speed;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
    {
		position.y += trans_speed;
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
		position.y -= trans_speed;
    }
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
		rotation.x -= rotation_speed;
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
		rotation.x += rotation_speed;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
    {
		rotation.y -= rotation_speed;
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    {
		rotation.y += rotation_speed;
    }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    {
		rotation.z -= rotation_speed;
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
    {
		rotation.z += rotation_speed;
    }
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
    {
        if (door_position < 2.0f)
		    door_position += 0.1f;
    }
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
    {
        if (door_position > 0.0f)
            door_position -= 0.1f;
    }
 }
