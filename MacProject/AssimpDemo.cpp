//
//  AssimpDemo.cpp
//  MacProject
//
//  Created by santian_mac on 2021/7/4.
//

#include <stdio.h>
#include "AssimpDemo.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader_s.h"
#include "Camera.h"
#include "stb_image.h"
#include "model.h"

#include <iostream>

void framebuffer_size_callback3(GLFWwindow* window, int width, int height);
void mouse_callback3(GLFWwindow* window, double xpos, double ypos);
void scroll_callback3(GLFWwindow* window, double xoffset, double yoffset);
void processInput3(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;

// camera
//Camera camera2(glm::vec3(0.0f, 10.0f, 20.0f));
Camera camera2(glm::vec3(0.0f, 0.0f, 5.0f));
float lastX2 = SCR_WIDTH / 2.0f;
float lastY2 = SCR_HEIGHT / 2.0f;
bool firstMouse2 = true;

// timing
float deltaTime2 = 0.0f;
float lastFrame2 = 0.0f;

void AssimpDemo::test()
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
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "AssimpDemo", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback3);
    glfwSetCursorPosCallback(window, mouse_callback3);
    glfwSetScrollCallback(window, scroll_callback3);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
    }

    // tell stb_image.h to flip loaded texture's on the y-axis (before loading model).
    stbi_set_flip_vertically_on_load(true);

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile shaders
    // -------------------------
    Shader ourShader("1.model_loading.vs", "1.model_loading.fs");

    // load models
    // -----------
//    Model ourModel("Handgun.obj");
    Model ourModel("nanosuit.obj");

    
    // draw in wireframe
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = glfwGetTime();
        deltaTime2 = currentFrame - lastFrame2;
        lastFrame2 = currentFrame;

        // input
        // -----
        processInput3(window);

        // render
        // ------
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // don't forget to enable shader before setting uniforms
        ourShader.use();

        // view/projection transformations
        glm::mat4 projection = glm::perspective(glm::radians(camera2.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera2.GetViewMatrix();
        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);

        // render the loaded model
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));    // it's a bit too big for our scene, so scale it down
        ourShader.setMat4("model", model);
        ourModel.Draw(ourShader);


        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput3(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera2.ProcessKeyboard(FORWARD, deltaTime2);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera2.ProcessKeyboard(BACKWARD, deltaTime2);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera2.ProcessKeyboard(LEFT, deltaTime2);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera2.ProcessKeyboard(RIGHT, deltaTime2);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback3(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback3(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse2)
    {
        lastX2 = xpos;
        lastY2 = ypos;
        firstMouse2 = false;
    }

    float xoffset = xpos - lastX2;
    float yoffset = lastY2 - ypos; // reversed since y-coordinates go from bottom to top

    lastX2 = xpos;
    lastY2 = ypos;

    camera2.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback3(GLFWwindow* window, double xoffset, double yoffset)
{
    camera2.ProcessMouseScroll(yoffset);
}
