///////////////
// scene.hpp //
///////////////

#pragma once

#include "object.hpp"
#include "shader.hpp"
#include "settings.hpp"
#include "camera.hpp"

#include <vector>

unsigned int loadTexture(char const* path);

class Scene
{
	public:
		// Constructor & Destructor:
		Scene();
		~Scene();
		// External functions:
		void Render(Camera& camera);
		void AddObject(Object& object);
		void ConfigureDepthMap();
		void ConfigureShaders();
		// Get/Set:
		void SetLightingShader(Shader& lightingShader);
		Shader* GetLightingShader();
		void SetShadowShader(Shader& shadowShader);
		Shader* GetShadowShader();

		unsigned int woodTexture;

	private:
		// Private functions:
		void RenderScene(Shader& shader);
		// Private attributes:
		Shader* m_lightingShader;
		Shader* m_shadowShader;
		std::vector<Object*> m_objects;
		unsigned int m_depthMap, m_depthMapFBO;
};