#pragma once
#include "camera.hpp"
#include "shader.hpp"
#include "object.hpp"

class Light
{
    public:
        // Constructor & Destructor:
        Light(Shader& shader, float* vertices, int N);
        ~Light();
        // Inherited Functions:
        void Render(float deltaTime, Camera& camera);
        // Get/Set
        glm::vec3 GetPosition();
        void SetPosition(glm::vec3 newPosition);
        void SetDraw(bool newDraw);

    private:
        // Private Attributes
        glm::vec3 position;
        unsigned int m_VAO, m_VBO;
        Shader* m_shader;
        glm::mat4 model;
        bool draw;
};