#include "camera.hpp"

Camera::Camera()
{
    locked = true;
    cameraPos = glm::vec3(0.0f, 10.0f, 3.0f);
    cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
    firstMouse = true;
    yaw = -90.0f;
    pitch = 0.0f;
    lastX = 800.0f / 2.0;
    lastY = 600.0f / 2.0;
    fov = 45.0f;
}

Camera::~Camera()
{

}

void Camera::SetCameraPos(glm::vec3 newCameraPos)
{
    cameraPos = newCameraPos;
}

void Camera::SetCameraFront(glm::vec3 newCameraFront)
{
    cameraFront = newCameraFront;
}

void Camera::SetCameraUp(glm::vec3 newCameraUp)
{
    cameraUp = newCameraUp;
}

glm::vec3 Camera::GetCameraPos()
{
    return cameraPos;
}

glm::vec3 Camera::GetCameraFront()
{
    return cameraFront;
}

glm::vec3 Camera::GetCameraUp()
{
    return cameraUp;
}

void Camera::SetFirstMouse(bool newFirstMouse)
{
    firstMouse = newFirstMouse;
}

void Camera::SetYaw(float newYaw)
{
    yaw = newYaw;
}

void Camera::SetPitch(float newPitch)
{
    pitch = newPitch;
}

void Camera::SetLastX(float newLastX)
{
    lastX = newLastX;
}

void Camera::SetLastY(float newLastY)
{
    lastY = newLastY;
}

void Camera::SetFov(float newFov)
{
    fov = newFov;
}

bool Camera::GetFirstMouse()
{
    return firstMouse;
}

float Camera::GetYaw()
{
    return yaw;
}

float Camera::GetPitch()
{
    return pitch;
}

float Camera::GetLastX()
{
    return lastX;
}

float Camera::GetLastY()
{
    return lastY;
}

float Camera::GetFov()
{
    return fov;
}

glm::mat4 Camera::GetProjectionMatrix()
{
    return glm::perspective(glm::radians(fov), 800.0f / 600.0f, 0.1f, 100.0f);
}

glm::mat4 Camera::GetViewMatrix()
{
    return glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
}