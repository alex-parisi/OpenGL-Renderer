////////////////
// camera.hpp //
////////////////

#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "settings.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

class Camera
{
    public:
        // Constructor & Destructor:
        Camera();
        ~Camera();
        // Functions to get/set camera attributes:
        void SetCameraPos(glm::vec3 cameraPos);
        void SetCameraFront(glm::vec3 cameraFront);
        void SetCameraUp(glm::vec3 cameraUp);
        glm::vec3 GetCameraPos();
        glm::vec3 GetCameraFront();
        glm::vec3 GetCameraUp();
        // Functions to get/set the look attributes:
        void SetFirstMouse(bool firstMouse);
        void SetYaw(float yaw);
        void SetPitch(float pitch);
        void SetLastX(float lastX);
        void SetLastY(float lastY);
        void SetFov(float fov);
        bool GetFirstMouse();
        float GetYaw();
        float GetPitch();
        float GetLastX();
        float GetLastY();
        float GetFov();
        // Get the view and projections matrices
        glm::mat4 GetProjectionMatrix();
        glm::mat4 GetViewMatrix();
        // Lock
        bool GetLock();
        void SetLock(bool locked);
        void FlipLock();
        // Window width and height:
        void SetWindowHeight(int windowHeight);
        int GetWindowHeight();
        void SetWindowWidth(int windowWidth);
        int GetWindowWidth();

    private:
        // Camera attributes:
        glm::vec3 m_cameraPos;
        glm::vec3 m_cameraFront;
        glm::vec3 m_cameraUp;
        // Look attributes:
        bool m_firstMouse;
        float m_yaw;
        float m_pitch;
        float m_lastX;
        float m_lastY;
        float m_fov;
        // Lock
        bool m_locked;
        // Window attributes
        int m_windowHeight, m_windowWidth;
};