#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "input.hpp"
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
        // Functions to get/set camera settings
        void SetCameraPos(glm::vec3 newCameraPos);
        void SetCameraFront(glm::vec3 newCameraFront);
        void SetCameraUp(glm::vec3 newCameraUp);
        glm::vec3 GetCameraPos();
        glm::vec3 GetCameraFront();
        glm::vec3 GetCameraUp();
        // Functions to get/set the look settings
        void SetFirstMouse(bool newFirstMouse);
        void SetYaw(float newYaw);
        void SetPitch(float newPitch);
        void SetLastX(float newLastX);
        void SetLastY(float newLastY);
        void SetFov(float newFov);
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
        bool locked;

    private:
        // Camera attributes:
        glm::vec3 cameraPos;
        glm::vec3 cameraFront;
        glm::vec3 cameraUp;
        // Look attributes:
        bool firstMouse;
        float yaw;
        float pitch;
        float lastX;
        float lastY;
        float fov;
};