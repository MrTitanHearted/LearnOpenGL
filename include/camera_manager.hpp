#pragma once

#include <glm/glm.hpp>

#include <camera.hpp>

class CameraManager {
   public:
    CameraManager(float lastX,
                  float lastY,
                  glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
                  glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f))
        : m_FirstMouse(true),
          m_LastXPosition(lastX),
          m_LastYPosition(lastY),
          m_Far(1000.0f),
          m_Near(0.0001f),
          m_Camera(Camera(position, front)) {}

    void processCameraMovement(CameraMovement direction, float dt);
    void processMouseMovement(float xPos, float yPos, bool constrain = true, float constrainValue = 89.9f);
    void processMouseScroll(float yOffset, float zoomMax = 45.0f, float zoomMin = 1.0f);

    void setWorldUp(glm::vec3 worldUp);
    void setMovementSpeed(float speed);
    void setMouseSensitivity(float sensitivity);
    void setFarNear(float far, float near);
    void setFar(float far);
    void setNear(float near);
    void setFirstMouse(bool firstMouse = true);

    const Camera &getCamera() const;
    float getFar() const;
    float getNear() const;

    glm::mat4 getProjectionMatrix(float aspect) const;
    glm::mat4 getViewMatrix() const;

   private:
    Camera m_Camera;

    bool m_FirstMouse;

    float m_LastXPosition;
    float m_LastYPosition;
    float m_Far;
    float m_Near;
};