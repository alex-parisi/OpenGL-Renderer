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
#include "point_light.hpp"
#include "model.hpp"
#include "input.hpp"
#include "settings.hpp"
#include <vector>
#include <string>

// Structure that gets passed to each callback function. Add pointers to relevant things here.
struct CallbackObj
{
	Camera* camera;
	InputManager* inputManager;
};

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
		InputManager inputManager;
        // Timing attributes:
        float deltaTime;
        float lastFrame;
		// Callback Object
		CallbackObj callbackObj;

	private:
		// Internal initialization functions
		void InitializeGLFW();
		bool InitializeGLAD();
		bool CreateWindow();
		// Callback functions (need to be static to work with GLFW)
		static void FrameBufferSizeCallback(GLFWwindow* window, int width, int height);
		static void MouseCallback(GLFWwindow* window, double xPosIn, double yPosIn);
		static void ScrollCallback(GLFWwindow* window, double xOffset, double yOffset);
		static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
};