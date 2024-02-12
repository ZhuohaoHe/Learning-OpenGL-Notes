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

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <vector>

void processInput(GLFWwindow *window); 
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// set up triangle vertices data
// OpenGL only processes 3D normalized device coordinates, 
// which means they are in between -1.0 and 1.0 in all 3 axes

float vertices[] = {
        // 0-2: posistion,  3-4: texture, 5-7: normal vector
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f,  0.0f, -1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f,  0.0f, -1.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  0.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f,  0.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f,  0.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f,  0.0f, 1.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, -1.0f,  0.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f, -1.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f, -1.0f,  0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  1.0f,  0.0f
    };

unsigned int indices[] = {
    // Front face
    0, 1, 2, 3, 4, 5,
    // Right face
    6, 7, 8, 9, 10, 11,
    // Back face
    12, 13, 14, 15, 16, 17,
    // Left face
    18, 19, 20, 21, 22, 23,
    // Bottom face
    24, 25, 26, 27, 28, 29,
    // Top face
    30, 31, 32, 33, 34, 35
};

glm::vec3 cubePositions[] = {
    glm::vec3( 0.0f,  0.0f,  0.0f), 
    glm::vec3( 2.0f,  5.0f, -15.0f), 
    glm::vec3(-1.5f, -2.2f, -2.5f),  
    glm::vec3(-3.8f, -2.0f, -12.3f),  
    glm::vec3( 2.4f, -0.4f, -3.5f),  
    glm::vec3(-1.7f,  3.0f, -7.5f),  
    glm::vec3( 1.3f, -2.0f, -2.5f),  
    glm::vec3( 1.5f,  2.0f, -2.5f), 
    glm::vec3( 1.5f,  0.2f, -1.5f), 
    glm::vec3(-1.3f,  1.0f, -1.5f)  
};

glm::vec3 pointLightPositions[] = {
    glm::vec3( 0.7f,  0.2f,  2.0f),
    glm::vec3( 2.3f, -3.3f, -4.0f),
    glm::vec3(-4.0f,  2.0f, -12.0f),
    glm::vec3( 0.0f,  0.0f, -3.0f)
};

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
    Shader lightShader("res/shaders/LightShader.shader");
/*  -----   -----  -----   */

/*  -----   Blending  -----   */
    GLCall(glEnable(GL_BLEND));
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
/*  -----   -----  -----   */


/*          ****    ****    ****        */
/*  -----   data & buffer & array ----- */
/*          ****    ****    ****        */

    VertexArray cubeVa;

    VertexBuffer cubeVb(vertices, sizeof(vertices));

    VertexBufferLayout cubeLayout;

    cubeLayout.Push<float>(3);
    cubeLayout.Push<float>(2);
    cubeLayout.Push<float>(3);

    cubeVa.AddBuffer(cubeVb, cubeLayout);

    IndexBuffer cubeIb(indices, sizeof(indices) / sizeof(unsigned int));

    // light
    VertexArray lightVa;

    VertexBufferLayout lightLayout;

    lightLayout.Push<float>(3);

    lightVa.AddBuffer(cubeVb, lightLayout); // share same cubeVb, because light is a cube too

    cubeIb.UnBind();

/*  -----    ------    -----    */


/*  -----   Texture  -----   */
    
    Texture texture1("res/textures/container2.png");
    Texture texture2("res/textures/container2_specular.png");
    Texture texture3("res/textures/matrix.jpg");

    // tell OpenGL for each sampler to which texture unit it belongs to (only has to be done once)
    texture1.Bind(0);
    texture2.Bind(1);
    texture3.Bind(2);
    basicShader.Use();
    basicShader.setInt("material.diffuse", 0);
    basicShader.setInt("material.specular", 1);
    basicShader.setInt("material.emission", 2);
    basicShader.UnUse(); 

/*  -----   -----   -----   -----   */

    float drop_speed = 0.01f;
    float bottom_y = -3.0f;
    float top_y = 3.0f;

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

/*  -----   define uniform   -----   */
    glm::vec3 direct_light_color = glm::vec3(1.0f, 1.0f, 1.0f);
    glm::vec3 point_light_color = glm::vec3(1.0f, 1.0f, 1.0f);
    glm::vec3 spot_light_color = glm::vec3(1.0f, 1.0f, 1.0f);

    float light_constant = 1.0f;
    float light_linear = 0.09f;
    float light_quadratic = 0.032f;

    glm::vec3 direct_light_direction = glm::vec3(-0.2f, -1.0f, -0.3f);
    glm::vec3 direct_light_ambient = direct_light_color * 0.05f;
    glm::vec3 direct_light_diffuse = direct_light_color * 0.4f;
    glm::vec3 direct_light_specular = direct_light_color * 0.5f;
    // point light
    glm::vec3 point_light_ambient = point_light_color * 0.05f;
    glm::vec3 point_light_diffuse = point_light_color * 0.6f;
    glm::vec3 point_light_specular = point_light_color * 1.0f;
    // spot light
    glm::vec3 spot_light_ambient = spot_light_color * 0.0f;
    glm::vec3 spot_light_diffuse = spot_light_color * 0.8f;
    glm::vec3 spot_light_specular = spot_light_color * 1.0f;

    float time = 0.0f;


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
        // point light
        point_light_ambient = point_light_color * 0.05f;
        point_light_diffuse = point_light_color * 0.6f;
        point_light_specular = point_light_color * 1.0f;
        // spot light
        spot_light_ambient = spot_light_color * 0.0f;
        spot_light_diffuse = spot_light_color * 0.8f;
        spot_light_specular = spot_light_color * 1.0f;


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
        // point
        for (int i = 0; i < 4; i ++ ) {
            std::string index = std::to_string(i);
            basicShader.setVec3("pointLights[" + index + "].position", pointLightPositions[i]);
            basicShader.setVec3("pointLights[" + index + "].ambient", point_light_ambient);
            basicShader.setVec3("pointLights[" + index + "].diffuse", point_light_diffuse);
            basicShader.setVec3("pointLights[" + index + "].specular", point_light_specular);
            basicShader.setFloat("pointLights[" + index + "].constant", light_constant);
            basicShader.setFloat("pointLights[" + index + "].linear", light_linear);
            basicShader.setFloat("pointLights[" + index + "].quadratic", light_quadratic);
        }
        // spot
        basicShader.setVec3("spotLight.position", camera.Position);
        basicShader.setVec3("spotLight.direction", camera.Front);
        basicShader.setVec3("spotLight.ambient", spot_light_ambient);
        basicShader.setVec3("spotLight.diffuse", spot_light_diffuse);
        basicShader.setVec3("spotLight.specular", spot_light_specular);
        basicShader.setFloat("spotLight.constant", light_constant);
        basicShader.setFloat("spotLight.linear", light_linear);
        basicShader.setFloat("spotLight.quadratic", light_quadratic);
        basicShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
        basicShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(17.5f)));


        // material properties
        basicShader.setFloat("material.shininess", 32.0f);

        // time
        time = (float)glfwGetTime();
        basicShader.setFloat("time", time);

        for(unsigned int i = 0; i < 10; i++){
            glm::mat4 model = glm::mat4(1.0f);
            // cubePositions[i].y -= drop_speed;
            // if (cubePositions[i].y < bottom_y) {
            //     cubePositions[i].y = top_y;
            // }
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i; 
            model = glm::rotate(model, glm::radians(angle) + (float)glfwGetTime(), glm::vec3(1.0f, 0.3f, 0.5f));
            basicShader.setMat4("model", model);

            renderer.Draw(cubeVa, cubeIb);
        }

        basicShader.UnUse();
        /*  -----   -----   -----   -----   */

        /*  -----   draw light cube -----   */
        lightShader.Use();
        lightShader.setMat4("projection", projection);
        lightShader.setMat4("view", view);
        for (int i = 0; i < 4; i ++ ) {
            model = glm::mat4(1.0f);
            model = glm::translate(model, pointLightPositions[i]);
            model = glm::scale(model, glm::vec3(0.2f));
            lightShader.setMat4("model", model);
            renderer.Draw(lightVa, cubeIb);
        }
        lightShader.UnUse();
        /*  -----   -----   -----   -----   */
        
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
        
            ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
            ImGui::ColorEdit3("direct light color", (float*)&direct_light_color); 
            ImGui::ColorEdit3("point light color", (float*)&point_light_color); 
            ImGui::ColorEdit3("spot light color", (float*)&spot_light_color); 


            if (ImGui::Button("Button")) {
                counter++;   // Buttons return true when clicked (most widgets return true when edited/activated)
            }                           
                
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
