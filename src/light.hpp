#pragma once
#include "camera.hpp"
#include "shader.hpp"

class Light
{
    public:
        // Constructor & Destructor:
        Light(float* vertices, int N);
        ~Light();
        // Inherited Functions:
        void Render(float deltaTime, Camera* camera, Shader& shader);
        // Get/Set
        glm::vec3 GetPosition();
        void SetPosition(glm::vec3 newPosition);
        void SetDraw(bool newDraw);
        glm::vec3 GetAmbient();
        glm::vec3 GetDiffuse();
        glm::vec3 GetSpecular();
        float GetConstant();
        float GetLinear();
        float GetQuadratic();
        void SetAmbient(glm::vec3 newAmbient);
        void SetDiffuse(glm::vec3 newDiffuse);
        void SetSpecular(glm::vec3 newSpecular);
        void SetConstant(float newConstant);
        void SetLinear(float newLinear);
        void SetQuadratic(float newQuadratic);

    private:
        // Private Attributes
        glm::vec3 position;
        unsigned int m_VAO, m_VBO;
        glm::mat4 model;
        bool draw;
        glm::vec3 ambient;
        glm::vec3 diffuse;
        glm::vec3 specular;
        float constant;
        float linear;
        float quadratic;
};