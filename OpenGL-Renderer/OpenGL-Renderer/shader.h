#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
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

private:
	// Internal initialization functions
	bool LoadGLSLCode(const char* vertexPath, const char* fragmentPath);
	bool Compile();
	bool Link();
	bool BindVertices();
	// Internal attributes
	const char* vertexShaderSource;
	const char* fragmentShaderSource;
	unsigned int vertexShader;
	unsigned int fragmentShader;
	unsigned int shaderProgram;
	unsigned int VBO, VAO;
	bool wireFrameOn;
	// Attributes used in loading GLSL code
	std::string vertexGLSLCode, fragmentGLSLCode;
	std::ifstream vertexShaderFile, fragmentShaderFile;
	std::stringstream vertexShaderStream, fragmentShaderStream;
};