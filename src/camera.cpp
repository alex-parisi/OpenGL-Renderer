////////////////
// camera.cpp //
////////////////

#include "camera.hpp"

Camera::Camera()
{
    m_locked = true;
    m_cameraPos = glm::vec3(12.713f, 11.7414f, 8.60537f);
    m_cameraFront = glm::vec3(-0.741935f, -0.452435, -0.494808f);
    m_cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
    m_firstMouse = true;
    m_yaw = 213.7f;
    m_pitch = -26.9f;
    m_lastX = 1195;
    m_lastY = 0;
    m_fov = 45.0f;
    m_windowHeight = SCREEN_HEIGHT;
    m_windowWidth = SCREEN_WIDTH;
}

Camera::~Camera()
{

}

void Camera::SetCameraPos(glm::vec3 cameraPos)
{
    m_cameraPos = cameraPos;
}

void Camera::SetCameraFront(glm::vec3 cameraFront)
{
    m_cameraFront = cameraFront;
}

void Camera::SetCameraUp(glm::vec3 cameraUp)
{
    m_cameraUp = cameraUp;
}

glm::vec3 Camera::GetCameraPos()
{
    return m_cameraPos;
}

glm::vec3 Camera::GetCameraFront()
{
    return m_cameraFront;
}

glm::vec3 Camera::GetCameraUp()
{
    return m_cameraUp;
}

void Camera::SetFirstMouse(bool firstMouse)
{
    m_firstMouse = firstMouse;
}

void Camera::SetYaw(float yaw)
{
    m_yaw = yaw;
}

void Camera::SetPitch(float pitch)
{
    m_pitch = pitch;
}

void Camera::SetLastX(float lastX)
{
    m_lastX = lastX;
}

void Camera::SetLastY(float lastY)
{
    m_lastY = lastY;
}

void Camera::SetFov(float fov)
{
    m_fov = fov;
}

bool Camera::GetFirstMouse()
{
    return m_firstMouse;
}

float Camera::GetYaw()
{
    return m_yaw;
}

float Camera::GetPitch()
{
    return m_pitch;
}

float Camera::GetLastX()
{
    return m_lastX;
}

float Camera::GetLastY()
{
    return m_lastY;
}

float Camera::GetFov()
{
    return m_fov;
}

glm::mat4 Camera::GetProjectionMatrix()
{
    return glm::perspective(glm::radians(m_fov), 800.0f / 600.0f, 0.1f, 100.0f);
}

glm::mat4 Camera::GetViewMatrix()
{
    return glm::lookAt(m_cameraPos, m_cameraPos + m_cameraFront, m_cameraUp);
}

bool Camera::GetLock()
{
    return m_locked;
}

void Camera::SetLock(bool locked)
{
    m_locked = locked;
}

void Camera::FlipLock()
{
    m_locked = !m_locked;
}

void Camera::SetWindowHeight(int windowHeight)
{
    m_windowHeight = windowHeight;
}

int Camera::GetWindowHeight()
{
    return m_windowHeight;
}

void Camera::SetWindowWidth(int windowWidth)
{
    m_windowWidth = windowWidth;
}

int Camera::GetWindowWidth()
{
    return m_windowWidth;
}