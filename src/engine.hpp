////////////////
// engine.hpp //
////////////////

#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "settings.hpp"
#include "scene.hpp"
#include "shader.hpp"
#include "input.hpp"
#include "camera.hpp"

#include <iostream>
#include <string>

struct CallbackObj {
	Camera* camera;
	InputManager* inputManager;
};

class Engine
{
	public:
		// Constructor & Destructor:
		Engine();
		~Engine();
		// External functions:
		bool Initialize();
		void Execute();
		void Terminate();
		void Configure();
		// Get/Set:
		void SetLightingShader(Shader& lightingShader);
		void SetShadowShader(Shader& shadowShader);
		void SetPointShadowShader(Shader& pointShadowShader);
		void SetLightbulbShader(Shader& lightbulbShader);
		void SetSkybox(Skybox& skybox);
		void SetSkyboxShader(Shader& skyboxShader);
		void SetHDRShader(Shader& hdrShader);
		void SetBlurShader(Shader& blurShader);
		void SetTextShader(Shader& textShader);
		Scene* GetScene();
		// Other:
		void AddObjectToScene(Object& object);
		void AddPointLightToScene(PointLight& pointLight);
		void AddModelToScene(Model& model);
		
	private:
		// Internal functions:
		bool InitializeGLFW();
		bool InitializeGLAD();
		bool CreateWindow();
		void MapCallbacks();
		void ProcessInput(GLFWwindow* window);
		void Render();
		void HandleEvents();
		// Callback functions:
		static void FrameBufferSizeCallback(GLFWwindow* window, int width, int height);
		static void MouseCallback(GLFWwindow* window, double xPosIn, double yPosIn);
		static void ScrollCallback(GLFWwindow* window, double xOffset, double yOffset);
		static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
		// Internal properties:
		GLFWwindow* m_window;
		Scene m_scene;
		InputManager m_inputManager;
		Camera m_camera;
		CallbackObj m_callbackObj;
		float deltaTime, lastFrame;
};