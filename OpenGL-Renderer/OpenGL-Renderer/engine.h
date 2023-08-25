#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "shader.h"
#include <vector>

class Engine
{
	public:
		// Constructor/Destructor
		Engine();
		~Engine();
		// Exposed functions
		bool Initialize();
		void MapCallbacks();
		void Execute();
		void Terminate();
		void ProcessInput(GLFWwindow* window);
		void Render();
		void HandleEvents();
		void EnableWireFrameMode();
		void DisableWireFrameMode();
		// Exposed attributes
		GLFWwindow* window;
		Shader shader;

	private:
		// Internal initialization functions
		void InitializeGLFW();
		bool InitializeGLAD();
		bool CreateWindow();
		bool CreateShader(const char* vertexShaderSource, const char* fragmentShaderSource);
		// Callback functions (need to be static to work with GLFW)
		static void FrameBufferSizeCallback(GLFWwindow* window, int width, int height);
		// Internal attributes
};