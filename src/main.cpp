#include <iostream>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <shader.hpp>
#include <render_buffer.hpp>
#include <textures.hpp>
#include <camera_manager.hpp>
#include <mesh.hpp>
#include <model.hpp>

const char *TITLE = "Learn OpenGL";
float WIDTH = 800.0f;
float HEIGHT = 600.0f;

void framebufferSizeCallback(GLFWwindow *, int, int);

void processMouseMovement(GLFWwindow *, double, double);
void processScroll(GLFWwindow *, double, double);

CameraManager camera{WIDTH / 2.0f, HEIGHT / 2.0f, glm::vec3(0.0f, 0.0f, 1.0f)};

float lastFrame = 0.0f;
float dt = 0.0f;

int main() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize glfw\n";
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);

    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, TITLE, NULL, NULL);
    if (!window) {
        std::cerr << "Failed to created a window\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    glfwSetCursorPosCallback(window, processMouseMovement);
    glfwSetScrollCallback(window, processScroll);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize glad\n";
        glfwTerminate();
        return -1;
    }
    glViewport(0, 0, WIDTH, HEIGHT);
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    Shader shader("./assets/shaders/shader.vert", "./assets/shaders/shader.frag");

    SkinnedModel vampire{"./assets/models/vampire/dancing_vampire.dae"};

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::scale(model, glm::vec3(1.0f / 50.0f));
    // model = glm::rotate(model, glm::radians(-90.0f), glm::normalize(glm::vec3(1.0f, 0.0f, 0.0f)));

    glm::mat4 bones[200];
    for (unsigned int i = 0; i < 200; i++)
        bones[i] = glm::mat4(1.0f);

    shader.setMat4Array("bones", bones, 200);

    while (!glfwWindowShouldClose(window)) {
        float currentFrame = glfwGetTime();
        dt = currentFrame - lastFrame;
        lastFrame = currentFrame;

        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, GL_TRUE);
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            camera.processCameraMovement(CameraMovement::WORLD_FORWARD, dt);
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            camera.processCameraMovement(CameraMovement::WORLD_BACKWARD, dt);
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            camera.processCameraMovement(CameraMovement::RIGHT, dt);
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            camera.processCameraMovement(CameraMovement::LEFT, dt);
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
            camera.processCameraMovement(CameraMovement::WORLD_DOWN, dt);
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
            camera.processCameraMovement(CameraMovement::WORLD_UP, dt);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.use();
        shader.set("view", camera.getViewMatrix());
        shader.set("proj", camera.getProjectionMatrix(WIDTH / HEIGHT));
        shader.set("model", model);
        vampire.render(shader);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void framebufferSizeCallback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
    WIDTH = width;
    HEIGHT = height;
}

void processMouseMovement(GLFWwindow *, double xPosIn, double yPosIn) {
    camera.processMouseMovement(xPosIn, yPosIn, dt);
}

void processScroll(GLFWwindow *, double, double yOffset) {
    camera.processMouseScroll((float)yOffset, dt);
}
