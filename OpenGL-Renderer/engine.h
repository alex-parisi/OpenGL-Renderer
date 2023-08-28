#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "shader.h"
#include "settings.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <iostream>

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
		bool CreateShader();
		// Callback functions (need to be static to work with GLFW)
		static void FrameBufferSizeCallback(GLFWwindow* window, int width, int height);
		static void MouseCallback(GLFWwindow* window, double xposIn, double yposIn);
		static void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
		// Internal attributes
};