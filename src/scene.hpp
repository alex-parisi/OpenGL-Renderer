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
		void ConfigureHDR();
		void ConfigureBlur();
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
		float GetHeightScale();
		void SetHeightScale(float heightScale);
		Shader* GetHDRShader();
		void SetHDRShader(Shader& hdrShader);
		float GetExposure();
		void SetExposure(float exposure);
		Shader* GetBlurShader();
		void SetBlurShader(Shader& blurShader);

	private:
		// Private functions:
		void RenderScene(Shader& shader, bool useNormalMap, bool useHeightMap);
		void DrawLightbulbs(Camera& camera, InputManager& inputManager);
		void RenderQuad();
		// Private attributes:
		// Shaders used in rendering:
		Shader* m_lightingShader;
		Shader* m_shadowShader;
		Shader* m_pointShadowShader;
		Shader* m_lightbulbShader;
		Shader* m_skyboxShader;
		Shader* m_hdrShader;
		Shader* m_blurShader;
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
		// Scaling used in height map:
		float m_heightScale;
		// buffers used in HDR:
		unsigned int m_hdrFBO;
		unsigned int m_colorBuffers[2];
		unsigned int m_rboDepth;
		// quad buffers
		unsigned int m_quadVAO, m_quadVBO;
		// exposure used in hdr:
		float m_exposure;
		// buffers used in blur:
		unsigned int m_pingpongFBO[2];
		unsigned int m_pingpongColorbuffers[2];
};