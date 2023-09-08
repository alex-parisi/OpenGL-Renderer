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

#include <string>

class Object
{
	public:
		// Constructor(s) & Destructor:
		Object();
		Object(std::string name, const float* vertices, int N);
		Object(std::string name, const float* vertices, int N, unsigned int texture);
		~Object();
		// External functions:
		void Render(Shader& shader);
		// Get/Set:
		glm::mat4 GetModel();
		void SetModel(glm::mat4 model);
		unsigned int GetVAO();
		unsigned int GetVBO();
		std::string GetName();
		void SetName(std::string name);
		void SetTexScaling(float texScaling);
		void SetTexture(unsigned int texture);
		unsigned int GetTexture();
		
		
	private:
		glm::mat4 m_model;
		unsigned int m_VAO, m_VBO;
		int m_N;
		std::string m_name;
		float m_texScaling;
		unsigned int m_texture;
};