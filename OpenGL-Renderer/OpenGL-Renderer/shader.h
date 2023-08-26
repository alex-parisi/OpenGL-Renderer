#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

class Shader
{
public:
	// Constructor/Destructor
	Shader();
	~Shader();
	// Exposed functions
	bool Initialize(const char* vertexPath, const char* fragmentPath);
	void Delete();
	void Draw();
	void EnableWireframeMode();
	void DisableWireframeMode();
	// Utility uniform functions
	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;

private:
	// Internal initialization functions
	bool LoadGLSLCode(const char* vertexPath, const char* fragmentPath);
	bool Compile();
	bool Link();
	bool BindVertices();
	bool LoadTexture();
	// Internal attributes
	const char* vertexShaderSource;
	const char* fragmentShaderSource;
	unsigned int vertexShader;
	unsigned int fragmentShader;
	unsigned int shaderProgram;
	unsigned int VBO, VAO, EBO;
	unsigned int texture1, texture2;
	bool wireFrameOn;
	// Attributes used in loading GLSL code
	std::string vertexGLSLCode, fragmentGLSLCode;
	std::ifstream vertexShaderFile, fragmentShaderFile;
	std::stringstream vertexShaderStream, fragmentShaderStream;
};