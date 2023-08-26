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
	// Functions to get/set camera settings
	void SetCameraPos(glm::vec3 newCameraPos);
	void SetCameraFront(glm::vec3 newCameraFront);
	void SetCameraUp(glm::vec3 newCameraUp);
	glm::vec3 GetCameraPos();
	glm::vec3 GetCameraFront();
	glm::vec3 GetCameraUp();
	// Functions to get the time delta
	float GetDeltaTime();
	// Functions to get/set the look settings
	void SetFirstMouse(bool newFirstMouse);
	void SetYaw(float newYaw);
	void SetPitch(float newPitch);
	void SetLastX(float newLastX);
	void SetLastY(float newLastY);
	void SetFov(float newFov);
	bool GetFirstMouse();
	float GetYaw();
	float GetPitch();
	float GetLastX();
	float GetLastY();
	float GetFov();
	// Utility uniform functions
	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;
	void setMat4(const std::string& name, const glm::mat4& mat) const;

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
	// Timing attributes:
	float deltaTime;
	float lastFrame;
	// Camera attributes:
	glm::vec3 cameraPos;
	glm::vec3 cameraFront;
	glm::vec3 cameraUp;
	// Look attributes:
	bool firstMouse;
	float yaw;
	float pitch;
	float lastX;
	float lastY;
	float fov;
	// Attributes used in loading GLSL code
	std::string vertexGLSLCode, fragmentGLSLCode;
	std::ifstream vertexShaderFile, fragmentShaderFile;
	std::stringstream vertexShaderStream, fragmentShaderStream;
};