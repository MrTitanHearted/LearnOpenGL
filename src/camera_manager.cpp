#include <camera_manager.hpp>

#include <glm/gtc/matrix_transform.hpp>

CameraManager::CameraManager(float lastX, float lastY, glm::vec3 position, glm::vec3 front) {
    m_FirstMouse = true;

    m_LastXPosition = lastX;
    m_LastYPosition = lastY;
    m_Far = 1000.0f;
    m_Near = 0.0001f;

    m_Camera = Camera(position, front);
}

void CameraManager::processCameraMovement(CameraMovement direction, float dt) {
    m_Camera.processCameraMovement(direction, dt);
}

void CameraManager::processMouseMovement(float xPos, float yPos, bool constrain, float constrainValue) {
    if (m_FirstMouse) {
        m_LastXPosition = xPos;
        m_LastYPosition = yPos;

        m_FirstMouse = false;
    }

    float xOffset = xPos - m_LastXPosition;
    float yOffset = m_LastYPosition - yPos;
    m_LastXPosition = xPos;
    m_LastYPosition = yPos;

    m_Camera.processMouseMovement(xOffset, yOffset, constrain, constrainValue);
}

void CameraManager::processMouseScroll(float yOffset, float zoomMax, float zoomMin) {
    m_Camera.processMouseScroll(yOffset, zoomMax, zoomMin);
}

void CameraManager::setWorldUp(glm::vec3 worldUp) {
    m_Camera.setWorldUp(worldUp);
}

void CameraManager::setMovementSpeed(float speed) {
    m_Camera.setMovementSpeed(speed);
}

void CameraManager::setMouseSensitivity(float sensitivity) {
    m_Camera.setMouseSensitivity(sensitivity);
}

void CameraManager::setFarNear(float far, float near) {
    m_Far = far;
    m_Near = near;
}

void CameraManager::setFar(float far) {
    m_Far = far;
}

void CameraManager::setNear(float near) {
    m_Near = near;
}

void CameraManager::setFirstMouse(bool firstMouse) {
    m_FirstMouse = firstMouse;
}

const Camera& CameraManager::getCamera() const {
    return m_Camera;
}

float CameraManager::getFar() const {
    return m_Far;
}

float CameraManager::getNear() const {
    return m_Near;
}

glm::mat4 CameraManager::getProjectionMatrix(float aspect) const {
    return m_Camera.getProjectionMatrix(aspect, m_Near, m_Far);
}

glm::mat4 CameraManager::getViewMatrix() const {
    return m_Camera.getViewMatrix();
}
