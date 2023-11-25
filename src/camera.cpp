#include <camera.hpp>
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(glm::vec3 position, glm::vec3 front) {
    m_Position = position;
    m_WorldUp = WORLD_UP;
    m_Yaw = YAW;
    m_Pitch = PITCH;
    m_Front = front;
    m_MovementSpeed = SPEED;
    m_MouseSensitivity = SENSITIVITY;
    m_Zoom = ZOOM;

    updateCameraVectors();
}

void Camera::processCameraMovement(CameraMovement direction, float dt) {
    float speed = m_MovementSpeed * dt;
    if (direction == CameraMovement::FORWARD)
        m_Position += m_Front * speed;
    if (direction == CameraMovement::BACKWARD)
        m_Position -= m_Front * speed;
    if (direction == CameraMovement::UP)
        m_Position += m_Up * speed;
    if (direction == CameraMovement::DOWN)
        m_Position -= m_Up * speed;
    if (direction == CameraMovement::WORLD_FORWARD)
        m_Position += glm::normalize(glm::vec3(m_Front.x, 0.0f, m_Front.z)) * speed;
    if (direction == CameraMovement::WORLD_BACKWARD)
        m_Position -= glm::normalize(glm::vec3(m_Front.x, 0.0f, m_Front.z)) * speed;
    if (direction == CameraMovement::WORLD_UP)
        m_Position += m_WorldUp * speed;
    if (direction == CameraMovement::WORLD_DOWN)
        m_Position -= m_WorldUp * speed;
    if (direction == CameraMovement::RIGHT)
        m_Position += m_Right * speed;
    if (direction == CameraMovement::LEFT)
        m_Position -= m_Right * speed;
}

void Camera::processMouseMovement(float xOffset, float yOffset, bool constrain, float constrainValue) {
    xOffset *= m_MouseSensitivity;
    yOffset *= m_MouseSensitivity;

    m_Yaw += xOffset;
    m_Pitch += yOffset;

    if (constrain)
        if (m_Pitch > constrainValue)
            m_Pitch = constrainValue;
        else if (m_Pitch < -constrainValue)
            m_Pitch = -constrainValue;

    updateCameraVectors();
}

void Camera::processMouseScroll(float yOffset, float zoomMax, float zoomMin) {
    m_Zoom -= yOffset;
    if (m_Zoom < zoomMin)
        m_Zoom = zoomMin;
    if (m_Zoom > zoomMax)
        m_Zoom = zoomMax;
}

void Camera::setWorldUp(glm::vec3 worldUp) {
    m_WorldUp = glm::normalize(worldUp);
    updateCameraVectors();
}

void Camera::setMovementSpeed(float speed) {
    m_MovementSpeed = speed;
}

void Camera::setMouseSensitivity(float sensitivity) {
    m_MouseSensitivity = sensitivity;
}

float Camera::getZoom() const {
    return m_Zoom;
}

glm::vec3 Camera::getPosition() const {
    return m_Position;
}

glm::vec3 Camera::getFront() const {
    return m_Front;
}

glm::vec3 Camera::getUp() const {
    return m_Up;
}

glm::vec3 Camera::getWorldUp() const {
    return m_WorldUp;
}

glm::mat4 Camera::getViewMatrix() const {
    return glm::lookAt(m_Position, m_Position + m_Front, m_Up);
}

glm::mat4 Camera::getProjectionMatrix(float aspect, float near, float far) const {
    return glm::perspective(glm::radians(m_Zoom), aspect, near, far);
}

void Camera::updateCameraVectors() {
    glm::vec3 front;
    front.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
    front.y = sin(glm::radians(m_Pitch));
    front.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));

    m_Front = glm::normalize(front);
    m_Right = glm::normalize(glm::cross(m_Front, m_WorldUp));
    m_Up = glm::normalize(glm::cross(m_Right, m_Front));
}
