#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <sstream>

#include <buffers.hpp>
#include <shaders.hpp>
#include <textures.hpp>
#include <camera.hpp>
#include <model.hpp>

void framebufferSizeCallback(GLFWwindow *window, int width, int height);
void mouseCallback(GLFWwindow *window, double x, double y);
void scrollCallback(GLFWwindow *window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 600;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = (float)WIDTH / 2.0;
float lastY = (float)HEIGHT / 2.0;
bool firstMouse = true;

float currentFrame = 0.0;
float deltaTime = 0.0;

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetScrollCallback(window, scrollCallback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glEnable(GL_DEPTH_TEST);

    BaseModel vampire("./assets/models/vampire/dancing_vampire.dae");
    Shader vampireShader("./shaders/model.vs", "./shaders/model.fs");

    camera.SetSpeed(1.2);
    float fps = 0.0;
    while (!glfwWindowShouldClose(window))
    {
        deltaTime = (float)glfwGetTime() - currentFrame;
        currentFrame = (float)glfwGetTime();

        if (currentFrame - fps >= 1.0)
        {
            fps = currentFrame;
            std::stringstream title;
            title << "LearnOpenGL             FPS: " << (int)(1.0 / deltaTime) << ", delta_time: " << deltaTime << " seconds";
            glfwSetWindowTitle(window, title.str().c_str());
        }

        int width;
        int height;
        glfwGetFramebufferSize(window, &width, &height);
        glm::mat4x4 projection = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.001f, 1000.0f);
        
        processInput(window);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        vampireShader.setMat4x4("projection", projection);
        vampireShader.setMat4x4("view", camera.GetViewMatrix());
        glm::mat4x4 model(1.0);
        model = glm::scale(model, glm::vec3(0.05));
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0, 1.0, 0.0));
        vampireShader.setMat4x4("model", model);

        vampire.Render(vampireShader);
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS )
        camera.ProcessCameraMovement(CameraDirection::WORLD_FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) 
        camera.ProcessCameraMovement(CameraDirection::WORLD_BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        camera.ProcessCameraMovement(CameraDirection::LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        camera.ProcessCameraMovement(CameraDirection::RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera.ProcessCameraMovement(CameraDirection::WORLD_UP, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        camera.ProcessCameraMovement(CameraDirection::WORLD_DOWN, deltaTime);

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void framebufferSizeCallback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void mouseCallback(GLFWwindow *window, double x, double y)
{
    float xpos = (float)x;
    float ypos = (float)y;

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void scrollCallback(GLFWwindow *window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll((float)yoffset);
}