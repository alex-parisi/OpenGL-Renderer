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
		void AddPointLight(PointLight& pointLight);
		void ConfigureDepthMap();
		void ConfigureCubeMap();
		void ConfigureShaders();
		// Get/Set:
		void SetLightingShader(Shader& lightingShader);
		Shader* GetLightingShader();
		void SetShadowShader(Shader& shadowShader);
		Shader* GetShadowShader();
		void SetPointShadowShader(Shader& pointShadowShader);
		Shader* GetPointShadowShader();
		void SetLightbulbShader(Shader& lightbulbShader);
		Shader* GetLightbulbShader();
		DirectionalLight* GetDirectionalLight();

	private:
		// Private functions:
		void RenderScene(Shader& shader);
		void DrawLightbulbs(Camera& camera);
		// Private attributes:
		// Shaders used in rendering:
		Shader* m_lightingShader;
		Shader* m_shadowShader;
		Shader* m_pointShadowShader;
		Shader* m_lightbulbShader;
		// List of all objects being rendered:
		std::vector<Object*> m_objects;
		// Depth map used in generating shadows:
		unsigned int m_depthMap, m_depthMapFBO;
		// Depth maps used in generating point shadows:
		unsigned int m_cubeMap, m_cubeMapFBO;
		// Directional light, turned on by default
		DirectionalLight m_directionalLight;
		// List of all point lights in the scene:
		std::vector<PointLight*> m_pointLights;
};