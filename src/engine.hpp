#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <iostream>
#include "scene.hpp"
#include "camera.hpp"
#include "light.hpp"

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
		// Exposed attributes
		GLFWwindow* window;
        Scene scene;
        Camera camera;
        // Timing attributes:
        float deltaTime;
        float lastFrame;

	private:
		// Internal initialization functions
		void InitializeGLFW();
		bool InitializeGLAD();
		bool CreateWindow();
		// Callback functions (need to be static to work with GLFW)
		static void FrameBufferSizeCallback(GLFWwindow* window, int width, int height);
		static void MouseCallback(GLFWwindow* window, double xposIn, double yposIn);
		static void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
};