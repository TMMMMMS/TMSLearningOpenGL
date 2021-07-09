//
//  LightDemo.cpp
//  MacProject
//
//  Created by santian_mac on 2021/7/1.
//

#include <stdio.h>
#include "LightDemo.h"

void framebuffer_size_callback1(GLFWwindow* window, int width, int height);
void mouse_callback1(GLFWwindow* window, double xpos, double ypos);
void scroll_callback1(GLFWwindow* window, double xoffset, double yoffset);
void processInput1(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 10.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// lighting
glm::vec3 lightPos(0.6f, 0.0f, 5.0f);

void LightDemo::test() {
    
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
     GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LightDemo", NULL, NULL);
     if (window == NULL)
     {
         std::cout << "Failed to create GLFW window" << std::endl;
         glfwTerminate();
     }
     glfwMakeContextCurrent(window);
     glfwSetFramebufferSizeCallback(window, framebuffer_size_callback1);
     glfwSetCursorPosCallback(window, mouse_callback1);
     glfwSetScrollCallback(window, scroll_callback1);

     // tell GLFW to capture our mouse
     glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

     // glad: load all OpenGL function pointers
     // ---------------------------------------
     if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
     {
         std::cout << "Failed to initialize GLAD" << std::endl;
     }

     // configure global opengl state
     // -----------------------------
     glEnable(GL_DEPTH_TEST);

     // build and compile our shader zprogram
     // ------------------------------------
     Shader lightingShader("1.colors.vs", "1.colors.fs");
     Shader lightCubeShader("1.light_cube.vs", "1.light_cube.fs");

     // set up vertex data (and buffer(s)) and configure vertex attributes
     // ------------------------------------------------------------------
    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        
        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
        0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
        
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
        0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
        
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };
     // first, configure the cube's VAO (and VBO)
     unsigned int VBO, cubeVAO;
     glGenVertexArrays(1, &cubeVAO);
     glGenBuffers(1, &VBO);

     glBindBuffer(GL_ARRAY_BUFFER, VBO);
     glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

     glBindVertexArray(cubeVAO);

     // position attribute
     glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
     glEnableVertexAttribArray(0);
    // normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

     // second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
     unsigned int lightCubeVAO;
     glGenVertexArrays(1, &lightCubeVAO);
     glBindVertexArray(lightCubeVAO);

     glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // note that we update the lamp's position attribute's stride to reflect the updated buffer data
     glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
     glEnableVertexAttribArray(0);


     // render loop
     // -----------
     while (!glfwWindowShouldClose(window))
     {
         // per-frame time logic
         // --------------------
         float currentFrame = glfwGetTime();
         deltaTime = currentFrame - lastFrame;
         lastFrame = currentFrame;

         // input
         // -----
         processInput1(window);

         // render
         // ------
         glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
         glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
         
         // 让光源在场景中来回移动
         //         lightPos.x = 1.0f + sin(glfwGetTime()) * 2.0f;
         //         lightPos.y = sin(glfwGetTime() / 2.0f) * 1.0f;
         
         // be sure to activate shader when setting uniforms/drawing objects
         lightingShader.use();
         lightingShader.setVec3("light.position", lightPos);
         lightingShader.setVec3("viewPos", camera.Position);

         // light properties
         glm::vec3 lightColor;
         lightColor.x = sin(glfwGetTime() * 2.0f);
         lightColor.y = sin(glfwGetTime() * 0.7f);
         lightColor.z = sin(glfwGetTime() * 1.3f);
         glm::vec3 diffuseColor = lightColor   * glm::vec3(0.5f); // decrease the influence
         glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f); // low influence
         lightingShader.setVec3("light.ambient", ambientColor);
         lightingShader.setVec3("light.diffuse", diffuseColor);
         lightingShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);

         // material properties
         lightingShader.setVec3("material.ambient", 1.0f, 0.5f, 0.31f);
         lightingShader.setVec3("material.diffuse", 1.0f, 0.5f, 0.31f);
         lightingShader.setVec3("material.specular", 0.5f, 0.5f, 0.5f); // specular lighting doesn't have full effect on this object's material
         lightingShader.setFloat("material.shininess", 32.0f);

         // view/projection transformations
         glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
         glm::mat4 view = camera.GetViewMatrix();
         lightingShader.setMat4("projection", projection);
         lightingShader.setMat4("view", view);

         // world transformation
         glm::mat4 model = glm::mat4(1.0f);
//         model = glm::translate(model, glm::vec3(-1.f, .0f, .0f));
//         model = glm::scale(model, glm::vec3(0.7f));
//         model = glm::rotate(model, glm::radians(50.0f), glm::vec3(1.f, 1.f, .0f));
//         model = rotate(model, (float)glfwGetTime(), glm::vec3(1.0f, 1.0f, .0f));
         lightingShader.setMat4("model", model);
         

         // render the cube
         glBindVertexArray(cubeVAO);
         glDrawArrays(GL_TRIANGLES, 0, 36);


         // also draw the lamp object
         lightCubeShader.use();
         lightCubeShader.setMat4("projection", projection);
         lightCubeShader.setMat4("view", view);
         model = glm::mat4(1.0f);
         model = glm::translate(model, lightPos);
         model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
         lightCubeShader.setMat4("model", model);

         glBindVertexArray(lightCubeVAO);
         glDrawArrays(GL_TRIANGLES, 0, 36);


         // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
         // -------------------------------------------------------------------------------
         glfwSwapBuffers(window);
         glfwPollEvents();
     }

     // optional: de-allocate all resources once they've outlived their purpose:
     // ------------------------------------------------------------------------
     glDeleteVertexArrays(1, &cubeVAO);
     glDeleteVertexArrays(1, &lightCubeVAO);
     glDeleteBuffers(1, &VBO);

     // glfw: terminate, clearing all previously allocated GLFW resources.
     // ------------------------------------------------------------------
     glfwTerminate();
}

void processInput1(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback1(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}


// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback1(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback1(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}
