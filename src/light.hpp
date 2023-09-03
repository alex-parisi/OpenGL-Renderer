#pragma once
#include "camera.hpp"
#include "shader.hpp"
#include <unordered_map>

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
        void SetPreset(int preset);

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
        std::unordered_map<int, glm::vec3> presets = {
            {7,     glm::vec3(1.0,    0.7,        1.8)},
            {13,    glm::vec3(1.0,    0.35,       0.44)},
            {20,    glm::vec3(1.0,    0.22,       0.20)},
            {32,    glm::vec3(1.0,    0.14,       0.07)},
            {50,    glm::vec3(1.0,    0.09,       0.032)},
            {65,    glm::vec3(1.0,    0.07,       0.017)},
            {100,   glm::vec3(1.0,    0.045,      0.0075)},
            {160,   glm::vec3(1.0,    0.027,      0.0028)},
            {200,   glm::vec3(1.0,    0.022,      0.0019)},
            {325,   glm::vec3(1.0,    0.014,      0.0007)},
            {600,   glm::vec3(1.0,    0.007,      0.0002)},
            {3250,  glm::vec3(1.0,    0.0014,     0.000007)}
        };
};