#pragma once
#include "shader.hpp"
#include "texture.hpp"

class Material
{
    public:
        // Constructor & Destructor:
        Material();
        ~Material();
        // Public Functions:
        void SetDiffuseTexture(Texture &newDiffuseTexture);
        void SetSpecularTexture(Texture &newSpecularTexture);
        Texture* GetDiffuseTexture();
        Texture* GetSpecularTexture();
        void SetAmbient(glm::vec3 newAmbient);
        void SetDiffuse(glm::vec3 newDiffuse);
        void SetSpecular(glm::vec3 newSpecular);
        void SetShininess(float newShininess);
        glm::vec3 GetAmbient();
        glm::vec3 GetDiffuse();
        glm::vec3 GetSpecular();
        float GetShininess();

    private:
        // Private Attributes:
        Texture* diffuseTexture;
        Texture* specularTexture;
        glm::vec3 ambient;
        glm::vec3 diffuse;
        glm::vec3 specular;
        float shininess;


};