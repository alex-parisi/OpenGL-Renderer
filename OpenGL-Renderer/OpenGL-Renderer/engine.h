#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

void FrameBufferSizeCallback(GLFWwindow* window, int width, int height);

class App
{
	public:
		App();
		~App();

		bool Initialize();
		void MapCallbacks();
		void Execute();
		void Terminate();
		void ProcessInput(GLFWwindow* window);
		void Render();
		void HandleEvents();

		GLFWwindow* window;

	private:
		void InitializeGLFW();
		bool InitializeGLAD();
		bool CreateWindow();
};