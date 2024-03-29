#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "stb_image/stb_image.h"

#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"
#include "VertexArray.hpp"
#include "Shader.hpp"
#include "Camera.hpp"
#include "GLDebug.hpp"
#include "Renderer.hpp"
#include "Texture.hpp"
#include "Mesh.hpp"
#include "Model.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <vector>
#include <memory>

void processInput(GLFWwindow *window); 
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
std::vector<Vertex> set_date();

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;


// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

bool firstMouse = true;

float lastX = 400.0f, lastY = 300.0f;

// camera
Camera camera;

int main(){
    glfwInit();

    // set OpenGL version to 4.1
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    
    // set OpenGL profile to core-profile
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // set OpenGL forward compatibility to true, 
    // which means application uses only the modern features of OpenGL
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // for macOS
    
    // creating window
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL){
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    // window -> context
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    
    // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }    
    glEnable(GL_DEPTH_TEST);

/*  -----   Shader  -----   */
    Shader basicShader("res/shaders/BasicShader.shader");
/*  -----   -----  -----   */

/*  -----   Blending  -----   */
    GLCall(glEnable(GL_BLEND));
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
/*  -----   -----  -----   */


/*  -----   -----   -----   -----   */

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui_ImplOpenGL3_Init("#version 410");

    // Setup Style
    ImGui::StyleColorsDark();
    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);

    bool show_demo_window = false;


/*          ****    ****    ****        */
/*  ----- vertices indices textures --- */
/*          ****    ****    ****        */

    // model
    Model backpackModel("res/models/glasscube/Grass_Block.obj");

/*  -----   define light uniform   -----   */
    glm::vec3 direct_light_color = glm::vec3(1.0f, 1.0f, 1.0f);

    glm::vec3 direct_light_direction = glm::vec3(-0.2f, -1.0f, -0.3f);
    glm::vec3 direct_light_ambient = direct_light_color * 1.0f;
    glm::vec3 direct_light_diffuse = direct_light_color * 1.0f;
    glm::vec3 direct_light_specular = direct_light_color * 1.0f;


/*  -----   -------   -----   */


/*          ****    ****    ****        */
/*       -----   Render Loop  -----     */
    Renderer renderer;
    while(!glfwWindowShouldClose(window)){
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        processInput(window);

        // rendering
        renderer.Clear();

        // --- 2D ---
        glm::mat4 trans = glm::mat4(1.0f);

        // --- 3D ---
        glm::mat4 projection = glm::perspective(glm::radians(FOV), (float)SCR_WIDTH / (float)SCR_HEIGHT, 1.0f, 100.f);
        glm::mat4 view = camera.getViewMatrix();
        glm::mat4 model = glm::mat4(1.0f);

        // light paarmeters
        // directional light
        direct_light_direction = glm::vec3(-0.2f, -1.0f, -0.3f);
        direct_light_ambient = direct_light_color * 0.05f;
        direct_light_diffuse = direct_light_color * 0.4f;
        direct_light_specular = direct_light_color * 0.5f;


        /*  -----   draw cubes -----   */
        basicShader.Use();
        
        basicShader.setMat4("projection", projection);
        basicShader.setMat4("view", view);
        basicShader.setMat4("model", model);
        basicShader.setMat4("trans", trans); 
        basicShader.setVec3("viewPos", camera.Position);

        // light properties
        // direct
        basicShader.setVec3("directLight.direction", direct_light_direction);
        basicShader.setVec3("directLight.ambient", direct_light_ambient);
        basicShader.setVec3("directLight.diffuse", direct_light_diffuse);
        basicShader.setVec3("directLight.specular", direct_light_specular);
        
        // material properties
        basicShader.setFloat("material.shininess", 32.0f);

        float angle = 20.0f; 
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, -5.0f));
        model = glm::rotate(model, glm::radians(angle) + (float)glfwGetTime(), glm::vec3(1.0f, 0.3f, 0.5f));
        basicShader.setMat4("model", model);
        basicShader.UnUse();

        // cubeMesh.Render(&basicShader);
        backpackModel.Render(&basicShader);
        
        // poll IO events
        glfwPollEvents();

        // Start the ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        if (show_demo_window) {
            ImGui::ShowDemoWindow(&show_demo_window);
        }

        {
            static float f = 0.0f;
            static int counter = 0;

            ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

            ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
        
            ImGui::ColorEdit3("direct light color", (float*)&direct_light_color); 
        
            ImGui::SameLine();
            ImGui::Text("counter = %d", counter);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
            ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
            
            ImGui::End();
        }

        ImGui::EndFrame();
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // swap buffers ( from back to front screen)
        glfwSwapBuffers(window);

    }
/*  -----   -----   -----   -----   */
    
    ImGui_ImplGlfw_Shutdown();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    return 0;

}

// handling user input for a given window 
// query GLFW whether relevant keys are pressed/released this frame and react accordingly
void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera.processKeyboard(FORWARD, deltaTime);
    }
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.processKeyboard(BACKWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera.processKeyboard(RIGHT, deltaTime);
    }
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.processKeyboard(LEFT, deltaTime);
    }
}

// this callback function executes whenever the window size changed 
//  adjust the OpenGL viewport to match the new window size
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    // make sure the viewport matches the new window dimensions
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = ypos - lastY;
    lastX = xpos;
    lastY = ypos;

    camera.processMouse(xoffset, yoffset);
}


