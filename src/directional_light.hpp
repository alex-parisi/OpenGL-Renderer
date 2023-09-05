#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "settings.hpp"

class DirectionalLight
{
	public:
		// Constructor & Destructor
		DirectionalLight();
		~DirectionalLight();
		// Get/Set
		glm::vec3 GetPosition();
		glm::vec3 GetDirection();
		glm::vec3 GetAmbient();
		glm::vec3 GetDiffuse();
		glm::vec3 GetSpecular();
		void SetPosition(glm::vec3 newPosition);
		void SetDirection(glm::vec3 newDirection);
		void SetAmbient(glm::vec3 newAmbient);
		void SetDiffuse(glm::vec3 newDiffuse);
		void SetSpecular(glm::vec3 newSpecular);
		void ConfigureDepthMap();
		unsigned int depthMap, depthMapFBO;

	private:
		glm::vec3 position;
		glm::vec3 direction;
		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;
};