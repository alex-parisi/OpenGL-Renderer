#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>

class Shader
{
public:
	// Constructor/Destructor
	Shader();
	~Shader();
	// Exposed functions
	bool Initialize();
	void SetVertexShaderSource(const char* vertexShaderSource);
	void SetFragmentShaderSource(const char* fragmentShaderSource);
	void Delete();
	void Draw();
	void EnableWireframeMode();
	void DisableWireframeMode();

private:
	// Internal initialization functions
	bool Compile();
	bool Link();
	bool BindVertices();
	// Internal attributes
	const char* vertexShaderSource;
	const char* fragmentShaderSource;
	unsigned int vertexShader;
	unsigned int fragmentShader;
	unsigned int shaderProgram;
	unsigned int VBO, VAO, EBO;
	bool wireFrameOn;
};