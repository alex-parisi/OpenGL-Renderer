///////////////
// scene.hpp //
///////////////

#pragma once

#include "object.hpp"
#include "shader.hpp"
#include "settings.hpp"
#include "camera.hpp"
#include "input.hpp"
#include "light.hpp"

#include <vector>

unsigned int loadTexture(char const* path);

class Scene
{
	public:
		// Constructor & Destructor:
		Scene();
		~Scene();
		// External functions:
		void Render(Camera& camera, InputManager& inputManager);
		void AddObject(Object& object);
		void ConfigureDepthMap();
		void ConfigureShaders();
		// Get/Set:
		void SetLightingShader(Shader& lightingShader);
		Shader* GetLightingShader();
		void SetShadowShader(Shader& shadowShader);
		Shader* GetShadowShader();
		DirectionalLight* GetDirectionalLight();

		unsigned int woodTexture;

	private:
		// Private functions:
		void RenderScene(Shader& shader);
		// Private attributes:
		// Shaders used in rendering:
		Shader* m_lightingShader;
		Shader* m_shadowShader;
		// List of all objects being rendered:
		std::vector<Object*> m_objects;
		// Depth map used in generating shadows:
		unsigned int m_depthMap, m_depthMapFBO;
		// Directional light, turned off by default
		DirectionalLight m_directionalLight;
};