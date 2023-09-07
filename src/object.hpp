////////////////
// object.hpp //
////////////////

#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.hpp"

class Object
{
	public:
		// Constructor(s) & Destructor:
		Object();
		Object(const float* vertices, int N);
		~Object();
		// External functions:
		void Render(Shader& shader);
		// Get/Set:
		glm::mat4 GetModel();
		void SetModel(glm::mat4 model);
		unsigned int GetVAO();
		unsigned int GetVBO();
		
	private:
		glm::mat4 m_model;
		unsigned int m_VAO, m_VBO;
		int m_N;
};