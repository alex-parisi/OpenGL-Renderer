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

    private:
        // Private Attributes:
        Texture* texture;
        Shader* shader;

};