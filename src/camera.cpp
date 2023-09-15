#include <camera.hpp>
#include <glm/gtc/matrix_transform.hpp>

void Camera::updateCameraVectors()
{
    glm::vec3 front;
    front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    front.y = sin(glm::radians(Pitch));
    front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    Front = glm::normalize(front);
    Right = glm::normalize(glm::cross(Front, WorldUp));
    Up = glm::normalize(glm::cross(Right, Front));
}

Camera::Camera(glm::vec3 position, glm::vec3 front, glm::vec3 worldUp, float yaw, float pitch)
{
    Position = position;
    Front = front;
    WorldUp = worldUp;
    Yaw = yaw;
    Pitch = pitch;
    Speed = SPEED;
    Sensitivity = SENSITIVITY;
    Zoom = ZOOM;
    updateCameraVectors();
}

glm::mat4x4 Camera::GetViewMatrix()
{
    return glm::lookAt(Position, Position + Front, Up);
}

void Camera::ProcessCameraMovement(CameraDirection direction, float deltaTime)
{
    float velocity = Speed * deltaTime;
    switch (direction)
    {
    case CameraDirection::FORWARD:
        Position += Front * velocity;
        break;
    case CameraDirection::BACKWARD:
        Position -= Front * velocity;
        break;
    case CameraDirection::LEFT:
        Position -= Right * velocity;
        break;
    case CameraDirection::RIGHT:
        Position += Right * velocity;
        break;
    case CameraDirection::UP:
        Position += Up * velocity;
        break;
    case CameraDirection::DOWN:
        Position -= Up * velocity;
        break;
    case CameraDirection::WORLD_FORWARD:
        Position += glm::normalize(glm::vec3(Front.x, 0.0, Front.z)) * velocity;
        break;
    case CameraDirection::WORLD_BACKWARD:
        Position -= glm::normalize(glm::vec3(Front.x, 0.0, Front.z)) * velocity;
        break;
    case CameraDirection::WORLD_UP:
        Position += WorldUp * velocity;
        break;
    case CameraDirection::WORLD_DOWN:
        Position -= WorldUp * velocity;
        break;
    }
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch)
{
    xoffset *= Sensitivity;
    yoffset *= Sensitivity;

    Yaw += xoffset;
    Pitch += yoffset;

    if (constrainPitch)
    {
        if (Pitch > 89.9)
            Pitch = 89.9;
        else if (Pitch < -89.9)
            Pitch = -89.9;
    }

    updateCameraVectors();
}

void Camera::ProcessMouseScroll(float yoffset, bool constrainZoom)
{
    Zoom -= (float)yoffset;
    if (constrainZoom)
    {
        if (Zoom < 1.0)
            Zoom = 1.0;
        else if (Zoom > 45.0)
            Zoom = 45.0;
    }
}

void Camera::SetSpeed(float speed)
{
    Speed = speed;
}

void Camera::SetMouseSensitivity(float sensitivity)
{
    Sensitivity = sensitivity;
}

void Camera::SetZoom(float zoom)
{
    Zoom = zoom;
}

void Camera::SetWorldUp(glm::vec3 worldUp)
{
    WorldUp = worldUp;
    updateCameraVectors();
}
