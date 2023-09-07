///////////////
// light.cpp //
///////////////

#include "light.hpp"

DirectionalLight::DirectionalLight()
{
	// Default directional light:
	m_position = glm::vec3(0.0f, 4.0f, 1.0f);
	m_direction = glm::normalize(-m_position);
	m_ambient = glm::vec3(0.05f);
	m_diffuse = glm::vec3(0.5f);
	m_specular = glm::vec3(0.5f);
	m_on = false;
}

DirectionalLight::~DirectionalLight()
{

}

void DirectionalLight::SetUniforms(Shader& shader)
{
	shader.SetVec3("dirLight.position", m_position);
	shader.SetVec3("dirLight.direction", m_direction);
	if (m_on)
	{
		shader.SetVec3("dirLight.ambient", m_ambient);
		shader.SetVec3("dirLight.diffuse", m_diffuse);
		shader.SetVec3("dirLight.specular", m_specular);
	}
	else
	{
		shader.SetVec3("dirLight.ambient", glm::vec3(0.0f));
		shader.SetVec3("dirLight.diffuse", glm::vec3(0.0f));
		shader.SetVec3("dirLight.specular", glm::vec3(0.0f));
	}
}

glm::vec3 DirectionalLight::GetPosition()
{
	return m_position;
}

glm::vec3 DirectionalLight::GetDirection()
{
	return m_direction;
}

glm::vec3 DirectionalLight::GetAmbient()
{
	return m_ambient;
}

glm::vec3 DirectionalLight::GetDiffuse()
{
	return m_diffuse;
}

glm::vec3 DirectionalLight::GetSpecular()
{
	return m_specular;
}

void DirectionalLight::SetPosition(glm::vec3 position)
{
	m_position = position;
}

void DirectionalLight::SetDirection(glm::vec3 direction)
{
	m_direction = direction;
}

void DirectionalLight::SetAmbient(glm::vec3 ambient)
{
	m_ambient = ambient;
}

void DirectionalLight::SetDiffuse(glm::vec3 diffuse)
{
	m_diffuse = diffuse;
}

void DirectionalLight::SetSpecular(glm::vec3 specular)
{
	m_specular = specular;
}

void DirectionalLight::TurnOn()
{
	m_on = true;
}

void DirectionalLight::TurnOff()
{
	m_on = false;
}