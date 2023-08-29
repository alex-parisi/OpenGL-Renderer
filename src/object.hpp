#pragma once
#include "material.hpp"
#include "camera.hpp"

class Object
{
    public:
        // Constructor & Destructor:
        Object(Material& material, float* vertices, int N, bool wireframeOn);
        ~Object();
        // Transforms
        void Translate(glm::vec3 newPos);
        void Rotate(float angle, glm::vec3 axis);
        void Scale(glm::vec3 scale);
        // Inherited Functions:
        void Render(float deltaTime, Camera& camera);
        // Get/Set
        glm::vec3 GetPosition();
        void SetWireframeMode(bool newWireframeMode);
        bool GetWireframeMode();

    private:
        // Private Attributes
        Material m_material;
        glm::mat4 model;
        bool wireframeMode;
        glm::vec3 position;
        unsigned int VAO, VBO;
};