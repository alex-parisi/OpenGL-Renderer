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
#include "model.hpp"
#include "skybox.hpp"

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
		void AddModel(Model& model);
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
		Skybox* GetSkybox();
		void SetSkybox(Skybox& skybox);
		Shader* GetSkyboxShader();
		void SetSkyboxShader(Shader& skyboxShader);

	private:
		// Private functions:
		void RenderScene(Shader& shader);
		void DrawLightbulbs(Camera& camera, InputManager& inputManager);
		// Private attributes:
		// Shaders used in rendering:
		Shader* m_lightingShader;
		Shader* m_shadowShader;
		Shader* m_pointShadowShader;
		Shader* m_lightbulbShader;
		Shader* m_skyboxShader;
		// List of all objects being rendered:
		std::vector<Object*> m_objects;
		// List of all models being rendered:
		std::vector<Model*> m_models;
		// Depth map used in generating shadows:
		unsigned int m_depthMap, m_depthMapFBO;
		// Depth maps used in generating point shadows:
		unsigned int m_cubeMap, m_cubeMapFBO;
		// Directional light, turned on by default
		DirectionalLight m_directionalLight;
		// List of all point lights in the scene:
		std::vector<PointLight*> m_pointLights;
		// Skybox used in rendering:
		Skybox* m_skybox;
};