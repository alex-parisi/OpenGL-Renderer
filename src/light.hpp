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
#include "vertices.hpp"

#include <string>
#include <unordered_map>

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
		PointLight(unsigned int id, int preset);
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
		unsigned int GetVAO();
		unsigned int GetVBO();
		// External attributes:
		unsigned int m_id;
	private:
		void SetupBuffers();
		glm::vec3 m_position;
		glm::vec3 m_ambient;
		glm::vec3 m_diffuse;
		glm::vec3 m_specular;
		float m_constant;
		float m_linear;
		float m_quadratic;
		bool m_on;
		unsigned int m_VAO, m_VBO;
		// TO - DO : Move this map somewhere else
		std::unordered_map<int, glm::vec3> presets = {
			{1,  glm::vec3(1.0, 0.7,    1.8)},
			{2,  glm::vec3(1.0, 0.35,   0.44)},
			{3,  glm::vec3(1.0, 0.22,   0.20)},
			{4,  glm::vec3(1.0, 0.14,   0.07)},
			{5,  glm::vec3(1.0, 0.09,   0.032)},
			{6,  glm::vec3(1.0, 0.07,   0.017)},
			{7,  glm::vec3(1.0, 0.045,  0.0075)},
			{8,  glm::vec3(1.0, 0.027,  0.0028)},
			{9,  glm::vec3(1.0, 0.022,  0.0019)},
			{10, glm::vec3(1.0, 0.014,  0.0007)},
			{11, glm::vec3(1.0, 0.007,  0.0002)},
			{12, glm::vec3(1.0, 0.0014, 0.000007)}
		};
};