#include "directional_light.hpp"

DirectionalLight::DirectionalLight()
{
	direction = glm::vec3(0.0f);
	ambient = glm::vec3(0.0f);
	diffuse = glm::vec3(0.0f);
	specular = glm::vec3(0.0f);
}

DirectionalLight::~DirectionalLight()
{

}

glm::vec3 DirectionalLight::GetDirection()
{
	return direction;
}

glm::vec3 DirectionalLight::GetAmbient()
{
	return ambient;
}

glm::vec3 DirectionalLight::GetDiffuse()
{
	return diffuse;
}

glm::vec3 DirectionalLight::GetSpecular()
{
	return specular;
}

void DirectionalLight::SetDirection(glm::vec3 newDirection)
{
	direction = newDirection;
}

void DirectionalLight::SetAmbient(glm::vec3 newAmbient)
{
	ambient = newAmbient;
}

void DirectionalLight::SetDiffuse(glm::vec3 newDiffuse)
{
	diffuse = newDiffuse;
}

void DirectionalLight::SetSpecular(glm::vec3 newSpecular)
{
	specular = newSpecular;
}