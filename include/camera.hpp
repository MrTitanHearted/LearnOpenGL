#pragma once

#include <glm/glm.hpp>

enum class CameraDirection
{
    FORWARD = 0,
    BACKWARD = 1,
    RIGHT = 2,
    LEFT = 3,
    UP = 4,
    DOWN = 5,
    WORLD_FORWARD = 6,
    WORLD_BACKWARD = 7,
    WORLD_UP = 8,
    WORLD_DOWN = 9,
};

class Camera
{
private:
    static constexpr float YAW = -90.0;
    static constexpr float PITCH = 0.0;
    static constexpr float SPEED = 0.50;
    static constexpr float SENSITIVITY = 0.1;
    static constexpr float ZOOM = 45.0;

    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;

    float Yaw;
    float Pitch;

    float Speed;
    float Sensitivity;
    float Zoom;

    void updateCameraVectors();
public:
    Camera(glm::vec3 position = glm::vec3(0.0, 0.0, 0.0), glm::vec3 front = glm::vec3(0.0, 0.0, -1.0), glm::vec3 worldUp = glm::vec3(0.0, 1.0, 0.0), float yaw = YAW, float pitch = PITCH);

    glm::mat4x4 GetViewMatrix();

    void ProcessCameraMovement(CameraDirection direction, float deltaTime);
    void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);
    void ProcessMouseScroll(float yoffset, bool constrainZoom = true);

    void SetSpeed(float speed);
    void SetMouseSensitivity(float sensitivity);
    void SetZoom(float zoom);

    void SetWorldUp(glm::vec3 worldUp);
};