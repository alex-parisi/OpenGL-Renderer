#include "directional_light.hpp"

DirectionalLight::DirectionalLight()
{
	position = glm::vec3(0.0f);
	direction = glm::vec3(0.0f);
	ambient = glm::vec3(0.0f);
	diffuse = glm::vec3(0.0f);
	specular = glm::vec3(0.0f);
	depthMap = NULL;
	depthMapFBO = NULL;
	ConfigureDepthMap();
}

DirectionalLight::~DirectionalLight()
{

}

glm::vec3 DirectionalLight::GetPosition()
{
	return position;
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

void DirectionalLight::SetPosition(glm::vec3 newPosition)
{
	position = newPosition;
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

void DirectionalLight::ConfigureDepthMap()
{
	glGenFramebuffers(1, &depthMapFBO);
	// create depth texture
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	// attach depth texture as FBO's depth buffer
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}