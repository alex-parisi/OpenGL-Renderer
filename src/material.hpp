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
        void SetTexture(Texture &newTexture);
        void SetShader(Shader &newShader);
        Texture* GetTexture();
        Shader* GetShader();
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
        Texture* texture;
        Shader* shader;
        glm::vec3 ambient;
        glm::vec3 diffuse;
        glm::vec3 specular;
        float shininess;


};