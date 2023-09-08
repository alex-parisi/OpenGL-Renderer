#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "stb_image.h"
#include "shader.hpp"
#include "camera.hpp"
#include <vector>
#include <string>
#include <iostream>

class Skybox
{
	public:
		// Constructor & Destructor
		Skybox();
		~Skybox();
		// Public Functions:
		void Load(std::vector<std::string> facePaths, const float* vertices, int N);
		unsigned int LoadCubemap(std::vector<std::string> facePaths);
		void Render(Shader& shader, Camera& camera);
		// Public Attributes:
		unsigned int VAO, VBO;
		unsigned int texture;
		bool loaded;

};