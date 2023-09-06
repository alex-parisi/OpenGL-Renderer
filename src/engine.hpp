#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "settings.hpp"

#include <iostream>
#include <string>

class Engine
{
	public:
		// Constructor & Destructor:
		Engine();
		~Engine();
		// External functions:
		void Execute();
		void Terminate();
		// Get/Set:
		bool GetIsSetup();
		
	private:
		// Internal functions:
		void InitializeGLFW();
		void InitializeGLAD();
		void CreateWindow();
		void MapCallbacks();
		// Callback functions:
		static void FrameBufferSizeCallback(GLFWwindow* window, int width, int height);
		// Internal properties:
		GLFWwindow* window;
		EngineError engineError;
		bool isSetup;
};

class EngineError
{
	public:
		// Constructor & Destructor:
		EngineError();
		~EngineError();
		// Get/Set:
		void SetErrorCode(std::string type, Error newCode);
		Error GetErrorCode(std::string type);
	private:
		// Internal functions:
		// Internal properties:
		Error GLAD;
		Error WINDOW_CREATION;
};

const enum Error
{
	NONE = 0,
	WARNING = 1,
	ERROR = 2
};