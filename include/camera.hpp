#pragma once

#include <glm/glm.hpp>

enum class CameraMovement {
    FORWARD,
    BACKWARD,
    UP,
    DOWN,
    WORLD_FORWARD,
    WORLD_BACKWARD,
    WORLD_UP,
    WORLD_DOWN,
    RIGHT,
    LEFT,
};

const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

const glm::vec3 WORLD_UP = glm::vec3(0.0f, 1.0f, 0.0f);

class Camera {
   public:
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f));

    void processCameraMovement(CameraMovement direction, float dt);
    void processMouseMovement(float xOffset, float yOffset, bool constrain = true, float constrainValue = 89.9f);
    void processMouseScroll(float yOffset, float zoomMax = 45.0f, float zoomMin = 1.0f);

    void setWorldUp(glm::vec3 worldUp);
    void setMovementSpeed(float speed);
    void setMouseSensitivity(float sensitivity);

    float getZoom() const;

    glm::vec3 getPosition() const;
    glm::vec3 getFront() const;
    glm::vec3 getUp() const;
    glm::vec3 getWorldUp() const;

    glm::mat4 getViewMatrix() const;
    glm::mat4 getProjectionMatrix(float aspect, float near = 0.0001f, float far = 1000.0f) const;

   private:
    glm::vec3 m_Position;
    glm::vec3 m_Front;
    glm::vec3 m_Right;
    glm::vec3 m_Up;
    glm::vec3 m_WorldUp;

    float m_Yaw;
    float m_Pitch;

    float m_MovementSpeed;
    float m_MouseSensitivity;
    float m_Zoom;

    void updateCameraVectors();
};