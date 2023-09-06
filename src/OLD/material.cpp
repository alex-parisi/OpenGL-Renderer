#include "material.hpp"

// Default Constructor:
Material::Material()
{
    diffuseTexture = NULL;
    specularTexture = NULL;
}

// Default Destructor:
Material::~Material()
{

}

void Material::SetDiffuseTexture(Texture &newDiffuseTexture)
{
    diffuseTexture = &newDiffuseTexture;
}

void Material::SetSpecularTexture(Texture &newSpecularTexture)
{
    specularTexture = &newSpecularTexture;
}

Texture* Material::GetDiffuseTexture()
{
    return diffuseTexture;
}

Texture* Material::GetSpecularTexture()
{
    return specularTexture;
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