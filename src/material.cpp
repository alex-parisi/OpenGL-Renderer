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

void Material::SetAmbient(glm::vec3 newAmbient)
{
    ambient = newAmbient;
}

void Material::SetDiffuse(glm::vec3 newDiffuse)
{
    diffuse = newDiffuse;
}

void Material::SetSpecular(glm::vec3 newSpecular)
{
    specular = newSpecular;
}

void Material::SetShininess(float newShininess)
{
    shininess = newShininess;
}

glm::vec3 Material::GetAmbient()
{
    return ambient;
}

glm::vec3 Material::GetDiffuse()
{
    return diffuse;
}

glm::vec3 Material::GetSpecular()
{
    return specular;
}

float Material::GetShininess()
{
    return shininess;
}