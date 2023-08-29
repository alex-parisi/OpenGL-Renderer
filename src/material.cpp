#include "material.hpp"

// Default Constructor:
Material::Material()
{
    texture = NULL;
    shader = NULL;
}

// Default Destructor:
Material::~Material()
{

}

void Material::SetTexture(Texture &newTexture)
{
    texture = &newTexture;
}

void Material::SetShader(Shader &newShader)
{
    shader = &newShader;
}

Texture* Material::GetTexture()
{
    return texture;
}

Shader* Material::GetShader()
{
    return shader;
}