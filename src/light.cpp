///////////////
// light.cpp //
///////////////

#include "light.hpp"

///////////////////////
// Directional Light //
///////////////////////

DirectionalLight::DirectionalLight()
{
	// Default directional light:
	m_position = glm::vec3(0.0f, 4.0f, 1.0f);
	m_direction = glm::normalize(-m_position);
	m_ambient = glm::vec3(0.025f);
	m_diffuse = glm::vec3(0.25f);
	m_specular = glm::vec3(0.25f);
	m_on = true;
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

/////////////////
// Point Light //
/////////////////

PointLight::PointLight()
{
	m_position = glm::vec3(0.0f);
	m_ambient = glm::vec3(0.0f);
	m_diffuse = glm::vec3(0.0f);
	m_specular = glm::vec3(0.0f);
	m_constant = 0.0f;
	m_linear = 0.0f;
	m_quadratic = 0.0f;
	m_on = true;
	m_id = NULL;
}

PointLight::PointLight(unsigned int id)
{
	m_position = glm::vec3(0.0f);
	m_ambient = glm::vec3(0.0f);
	m_diffuse = glm::vec3(0.0f);
	m_specular = glm::vec3(0.0f);
	m_constant = 0.0f;
	m_linear = 0.0f;
	m_quadratic = 0.0f;
	m_on = true;
	m_id = id;
}

PointLight::~PointLight()
{

}

void PointLight::SetUniforms(Shader& shader, int i)
{
	char strBuf[32];
	sprintf_s(strBuf, "pointLights[%d]", i);
	std::string s(strBuf);
	shader.SetVec3(s + ".position", m_position);
	shader.SetVec3(s + ".ambient", m_ambient);
	shader.SetVec3(s + ".diffuse", m_diffuse);
	shader.SetVec3(s + ".specular", m_specular);
	shader.SetFloat(s + ".constant", m_constant);
	shader.SetFloat(s + ".linear", m_linear);
	shader.SetFloat(s + ".quadratic", m_quadratic);
}

glm::vec3 PointLight::GetPosition()
{
	return m_position;
}

glm::vec3 PointLight::GetAmbient()
{
	return m_ambient;
}

glm::vec3 PointLight::GetDiffuse()
{
	return m_diffuse;
}

glm::vec3 PointLight::GetSpecular()
{
	return m_specular;
}

float PointLight::GetConstant()
{
	return m_constant;
}

float PointLight::GetLinear()
{
	return m_linear;
}

float PointLight::GetQuadratic()
{
	return m_quadratic;
}

void PointLight::SetPosition(glm::vec3 position)
{
	m_position = position;
}

void PointLight::SetAmbient(glm::vec3 ambient)
{
	m_ambient = ambient;
}

void PointLight::SetDiffuse(glm::vec3 diffuse)
{
	m_diffuse = diffuse;
}

void PointLight::SetSpecular(glm::vec3 specular)
{
	m_specular = specular;
}

void PointLight::SetConstant(float constant)
{
	m_constant = constant;
}

void PointLight::SetLinear(float linear)
{
	m_linear = linear;
}

void PointLight::SetQuadratic(float quadratic)
{
	m_quadratic = quadratic;
}

void PointLight::TurnOn()
{
	m_on = true;
}

void PointLight::TurnOff()
{
	m_on = false;
}