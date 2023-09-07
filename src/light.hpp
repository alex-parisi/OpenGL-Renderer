///////////////
// light.hpp //
///////////////

#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.hpp"

#include <string>

class DirectionalLight
{
	public:
		// Constructor & Destructor
		DirectionalLight();
		~DirectionalLight();
		// External functions:
		void SetUniforms(Shader& shader);
		// Get/Set
		glm::vec3 GetPosition();
		glm::vec3 GetDirection();
		glm::vec3 GetAmbient();
		glm::vec3 GetDiffuse();
		glm::vec3 GetSpecular();
		void SetPosition(glm::vec3 position);
		void SetDirection(glm::vec3 direction);
		void SetAmbient(glm::vec3 ambient);
		void SetDiffuse(glm::vec3 diffuse);
		void SetSpecular(glm::vec3 specular);
		void TurnOn();
		void TurnOff();

	private:
		glm::vec3 m_position;
		glm::vec3 m_direction;
		glm::vec3 m_ambient;
		glm::vec3 m_diffuse;
		glm::vec3 m_specular;
		bool m_on;
};

class PointLight
{
	public:
		// Constructor & Destructor:
		PointLight();
		PointLight(unsigned int id);
		~PointLight();
		// External functions:
		void SetUniforms(Shader& shader, int i);
		// Get/Set
		glm::vec3 GetPosition();
		glm::vec3 GetAmbient();
		glm::vec3 GetDiffuse();
		glm::vec3 GetSpecular();
		float GetConstant();
		float GetLinear();
		float GetQuadratic();
		void SetPosition(glm::vec3 position);
		void SetAmbient(glm::vec3 ambient);
		void SetDiffuse(glm::vec3 diffuse);
		void SetSpecular(glm::vec3 specular);
		void SetConstant(float constant);
		void SetLinear(float linear);
		void SetQuadratic(float quadratic);
		void TurnOn();
		void TurnOff();
		// External attributes:
		unsigned int m_id;
	private:
		glm::vec3 m_position;
		glm::vec3 m_ambient;
		glm::vec3 m_diffuse;
		glm::vec3 m_specular;
		float m_constant;
		float m_linear;
		float m_quadratic;
		bool m_on;
};