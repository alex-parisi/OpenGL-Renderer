#pragma once
#include <vector>
#include "material.hpp"
#include "camera.hpp"
#include "point_light.hpp"
#include "directional_light.hpp"

#define MAX_BONE_INFLUENCE 4

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
        void Render(float deltaTime, Camera* camera, Shader& shader, DirectionalLight* directionalLight, std::vector<PointLight*> pointLights);
        // Get/Set
        glm::vec3 GetPosition();
        void SetWireframeMode(bool newWireframeMode);
        bool GetWireframeMode();
        void SetModel(glm::mat4 newModel);
        glm::mat4 GetModel();
        void SetTexScaling(float newTexScaling);

    private:
        // Private Attributes
        Material* m_material;
        glm::mat4 model;
        bool wireframeMode;
        glm::vec3 position;
        unsigned int VAO, VBO;
        float texScaling;
};